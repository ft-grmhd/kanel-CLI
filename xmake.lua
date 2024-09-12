-- Copyright (C) 2024 kanel
-- This file is part of "kanel-CLI"
-- For conditions of distribution and use, see copyright notice in LICENSE

-- Credits to SirLynix (https://github.com/SirLynix) for this xmake.lua
-- Inspired by https://github.com/NazaraEngine/NazaraEngine

add_repositories("local-repo Xmake")
add_repositories("nazara-engine-repo https://github.com/NazaraEngine/xmake-repo")

set_languages("clatest")

add_rules("mode.debug", "mode.release")
set_allowedplats("windows", "mingw", "linux", "macosx", "wasm")
set_allowedmodes("debug", "release")
set_defaultmode("release")

set_project("kanelcli")

if is_mode("debug") then
	add_defines("KANEL_CLI_DEBUG")
elseif is_mode("release") then
	add_defines("KANEL_CLI_RELEASE")
	set_fpmodels("precise")
	add_vectorexts("sse", "sse2", "sse3", "ssse3")
	add_vectorexts("mmx")
	add_vectorexts("neon")
	add_vectorexts("avx", "avx2", "avx512")
end

add_includedirs("Includes")

set_objectdir("build/Objs/$(os)_$(arch)")
set_targetdir("build/Bin/$(os)_$(arch)")
set_rundir("build/Bin/$(os)_$(arch)")
set_dependir("build/.deps")

set_optimize("fastest")

local gpu_backends = {
	Vulkan = {
		option = "vulkan",
		deps = {"kanelcli_gpu"},
		packages = {"kvf", "nzsl", "vulkan-headers", "vulkan-memory-allocator"},
		dir = "GPU/",
		custom = function()
			add_defines("VK_NO_PROTOTYPES")
			add_defines("KVF_NO_KHR")
		end
	}
}

local modules = {
	GPU = {
		option = "gpu",
		deps = {},
		overrideDir = "GPU/RHI/",
		custom = function()
			if has_config("embed_gpu_backends", "static") then
				add_defines("KANEL_CLI_EMBEDDED_RHI_BACKENDS")
				for name, module in table.orderpairs(gpu_backends) do
					if not module.option or has_config(module.option) then
						ModuleTargetConfig(name, module)
					end
				end
			end
		end
	},
}

local sanitizers = {
	asan = "address",
	lsan = "leak",
	tsan = "thread",
}

for opt, policy in table.orderpairs(sanitizers) do
	option(opt, { description = "Enable " .. opt, default = false })

	if has_config(opt) then
		set_policy("build.sanitizer." .. policy, true)
	end
end

if is_plat("wasm") then
	gpu_backends.Vulkan = nil
end

if not has_config("embed_gpu_backends", "static") then
	-- Register renderer backends as separate modules
	for name, module in pairs(gpu_backends) do
		if (modules[name] ~= nil) then
			os.raise("overriding module " .. name)
		end

		modules[name] = module
	end
end

for name, module in table.orderpairs(modules) do
	if module.option then
		option(module.option, { description = "Enables the " .. name .. " module", default = true, category = "Modules" })
	end
end

includes("Xmake/actions/**.lua")

add_rules("build.gpu_plugins")

option("compile_shaders", { description = "Compile nzsl shaders into an includable binary version", default = true })
option("static", { description = "Build the engine statically (implies embed_gpu_backends)", default = is_plat("wasm") or false })
option("embed_gpu_backends", { description = "Embed GPU backend code into libkanelcli_gpu instead of loading them dynamically", default = is_plat("wasm") or false })

if has_config("vulkan") and not is_plat("wasm") then
	add_requires("vulkan-headers", "vulkan-memory-allocator", "kvf")
	add_requires("nzsl >=2023.12.31", { debug = is_mode("debug"), configs = { symbols = not is_mode("release"), shared = false } })
end

function ModuleTargetConfig(name, module)
	add_defines("KANEL_CLI_" .. name:upper() .. "_BUILD")
	if is_mode("debug") then
		add_defines("KANEL_CLI_" .. name:upper() .. "_DEBUG")
	end

	if is_plat("wasm") or has_config("static") then
		add_defines("KANEL_CLI_".. name:upper() .. "_STATIC", { public = true })
	end

	-- Add header and source files
	for _, ext in ipairs({".h", ".inl"}) do
		if module.overrideDir then
			add_headerfiles("Includes/" .. module.overrideDir .. "/**" .. ext)
			add_headerfiles("Sources/" .. module.overrideDir .. "/**" .. ext, { prefixdir = "private", install = false })
		elseif module.dir then
			add_headerfiles("Includes/" .. module.dir .. name .. "/**" .. ext)
			add_headerfiles("Sources/" .. module.dir .. name .. "/**" .. ext, { prefixdir = "private", install = false })
		else
			add_headerfiles("Includes/" .. name .. "/**" .. ext)
			add_headerfiles("Sources/" .. name .. "/**" .. ext, { prefixdir = "private", install = false })
		end
	end

	if module.overrideDir then
		remove_headerfiles("Sources/" .. module.overrideDir .. "/Resources/**.h")
	elseif module.dir then
		remove_headerfiles("Sources/" .. module.dir .. name .. "/Resources/**.h")
	else
		remove_headerfiles("Sources/" .. name .. "/Resources/**.h")
	end

	-- Add extra files for projects
	for _, ext in ipairs({".nzsl"}) do
		if module.overrideDir then
			add_extrafiles("Includes/" .. module.overrideDir .. "/**" .. ext)
			add_extrafiles("Sources" .. module.overrideDir .. "/**" .. ext)
		elseif module.dir then
			add_extrafiles("Includes/" .. module.dir .. name .. "/**" .. ext)
			add_extrafiles("Sources" .. module.dir .. name .. "/**" .. ext)
		else
			add_extrafiles("Includes/" .. name .. "/**" .. ext)
			add_extrafiles("Sources" .. name .. "/**" .. ext)
		end
	end

	if module.packages then
		add_packages(table.unpack(module.packages))
	end

	if module.publicPackages then
		for _, pkg in ipairs(module.publicPackages) do
			add_packages(pkg, { public = true })
		end
	end

	if module.deps then
		local moduleName = "kanelcli_" .. name
		table.remove_if(module.deps, function(dep)
			return module.deps[dep] == moduleName
		end)
		if #module.deps > 0 then
			add_deps(table.unpack(module.deps))
		end
	end

	if module.overrideDir then
		add_files("Sources/" .. module.overrideDir .. "/**.c")
	elseif module.dir then
		add_files("Sources/" .. module.dir .. name .. "/**.c")
	else
		add_files("Sources/" .. name .. "/**.c")
	end

	if module.custom then
		module.custom()
	end
end

for name, module in pairs(modules) do
	if module.option and not has_config(module.option) then
		goto continue
	end

	target("kanelcli_" .. string.lower(name), function()
		set_group("Modules")

		-- handle shared/static kind
		if is_plat("wasm") or has_config("static") then
			set_kind("static")
		else
			set_kind("shared")
		end

		add_includedirs("Sources")
		add_rpathdirs("$ORIGIN")

		on_clean(function(target)
			if target:objectfiles() then
				for _, file in ipairs(target:objectfiles()) do
					if os.exists(file) then
						print("Removing " .. file)
						os.rm(file)
					end
				end
			end
			if target:targetfile() and os.exists(target:targetfile()) then
				print("Removing " .. target:targetfile())
				os.rm(target:targetfile())
			end
		end)

		ModuleTargetConfig(name, module)
	end)

	::continue::
end

target("kanel_cli")
	set_kind("binary")

	add_includedirs("Includes")
	add_includedirs("Sources")
	add_rpathdirs("$ORIGIN")

	for _, dir in ipairs(os.dirs("Sources/*")) do
		for name, _ in pairs(modules) do
			if dir == "Sources/" .. name then
				goto continue
			end
		end
		add_files(dir .. "/**.c")
		::continue::
	end

	on_clean(function(target)
		if target:objectfiles() then
			for _, file in ipairs(target:objectfiles()) do
				if os.exists(file) then
					print("Removing " .. file)
					os.rm(file)
				end
			end
		end
		if target:targetfile() and os.exists(target:targetfile()) then
			print("Removing " .. target:targetfile())
			os.rm(target:targetfile())
		end
	end)
target_end() -- optional

rule("build.gpu_plugins")
	on_load(function(target)
		if has_config("static") then
			return
		end

		local deps = table.wrap(target:get("deps"))

		if target:kind() == "binary" and table.contains(deps, "kanelcli_gpu") then
			for name, _ in pairs(gpu_backends) do
				local depName = "kanelcli_" .. string.lower(name)
				if not table.contains(deps, depName) then -- don't overwrite dependency
					target:add("deps", depName, {inherit = false})
				end
			end
		end
	end)
