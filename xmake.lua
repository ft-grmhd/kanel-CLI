-- Copyright (C) 2024 kanel
-- This file is part of "kanel-CLI"
-- For conditions of distribution and use, see copyright notice in LICENSE

-- Credits to SirLynix (https://github.com/SirLynix) for this xmake.lua
-- Inspired by https://github.com/NazaraEngine/NazaraEngine

set_version("0.0.1beta")

add_repositories("local-repo Xmake")
add_repositories("nazara-engine-repo https://github.com/NazaraEngine/xmake-repo")

set_languages("clatest", "cxx20")

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

add_includedirs("Kanel/Runtime/Includes")
add_includedirs("Kanel/Build")

set_objectdir("build/Objs/$(os)_$(arch)")
set_targetdir("build/Bin/$(os)_$(arch)")
set_rundir("build/Bin/$(os)_$(arch)")
set_dependir("build/.deps")

set_optimize("fastest")

set_configdir("Kanel/Build/")
add_configfiles("Kanel/Build/Config.h.in")

local gpu_backends = {
	Vulkan = {
		option = "vulkan",
		deps = {"kanel_gpu"},
		packages = {"nzsl", "vulkan-headers", "vulkan-memory-allocator"},
		dir = "GPU/",
		custom = function()
			add_defines("VK_NO_PROTOTYPES")
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
option("unitybuild", { description = "Build the engine using unity build", default = false })

add_requires("vrg")

if has_config("vulkan") and not is_plat("wasm") then
	add_requires("vulkan-headers", "vulkan-memory-allocator")
	add_requires("nzsl >=2023.12.31", { debug = is_mode("debug"), configs = { symbols = not is_mode("release"), shared = false } })

	-- When cross-compiling, compile shaders using host shader compiler
	if has_config("compile_shaders") and is_cross() then
		add_requires("nzsl~host", { kind = "binary", host = true })
	end
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
			add_headerfiles("Kanel/Runtime/Includes/Modules/" .. module.overrideDir .. "/**" .. ext)
			add_headerfiles("Kanel/Runtime/Sources/Modules/" .. module.overrideDir .. "/**" .. ext, { prefixdir = "private", install = false })
		elseif module.dir then
			add_headerfiles("Kanel/Runtime/Includes/Modules/" .. module.dir .. name .. "/**" .. ext)
			add_headerfiles("Kanel/Runtime/Sources/Modules/" .. module.dir .. name .. "/**" .. ext, { prefixdir = "private", install = false })
		else
			add_headerfiles("Kanel/Runtime/Includes/Modules/" .. name .. "/**" .. ext)
			add_headerfiles("Kanel/Runtime/Sources/Modules/" .. name .. "/**" .. ext, { prefixdir = "private", install = false })
		end
	end

	if module.overrideDir then
		remove_headerfiles("Kanel/Runtime/Sources/Modules/" .. module.overrideDir .. "/Resources/**.h")
	elseif module.dir then
		remove_headerfiles("Kanel/Runtime/Sources/Modules/" .. module.dir .. name .. "/Resources/**.h")
	else
		remove_headerfiles("Kanel/Runtime/Sources/Modules/" .. name .. "/Resources/**.h")
	end

	-- Add extra files for projects
	for _, ext in ipairs({".nzsl"}) do
		if module.overrideDir then
			add_extrafiles("Kanel/Runtime/Includes/Modules/" .. module.overrideDir .. "/**" .. ext)
			add_extrafiles("Kanel/Runtime/Sources/Modules/" .. module.overrideDir .. "/**" .. ext)
		elseif module.dir then
			add_extrafiles("Kanel/Runtime/Includes/Modules/" .. module.dir .. name .. "/**" .. ext)
			add_extrafiles("Kanel/Runtime/Sources/Modules" .. module.dir .. name .. "/**" .. ext)
		else
			add_extrafiles("Kanel/Runtime/Includes/Modules/" .. name .. "/**" .. ext)
			add_extrafiles("Kanel/Runtime/Sources/Modules" .. name .. "/**" .. ext)
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
		add_files("Kanel/Runtime/Sources/Modules/" .. module.overrideDir .. "/**.c")
		add_files("Kanel/Runtime/Sources/Modules/" .. module.overrideDir .. "/**.cpp")
	elseif module.dir then
		add_files("Kanel/Runtime/Sources/Modules/" .. module.dir .. name .. "/**.c")
		add_files("Kanel/Runtime/Sources/Modules/" .. module.dir .. name .. "/**.cpp")
	else
		add_files("Kanel/Runtime/Sources/Modules/" .. name .. "/**.c")
		add_files("Kanel/Runtime/Sources/Modules/" .. name .. "/**.cpp")
	end

	if has_config("compile_shaders") then
		add_rules("@nzsl/compile.shaders", { inplace = true })
		local dir = ""
		if module.overrideDir then
			dir = "Kanel/Runtime/Sources/Modules/" .. module.overrideDir
		elseif module.dir then
			dir = "Kanel/Runtime/Sources/Modules/" .. module.dir .. name
		else
			dir = "Kanel/Runtime/Sources/Modules/" .. name
		end
		for _, filepath in pairs(table.join(os.files(dir .. "/Resources/**.nzsl"), os.files(dir .. "/Resources/**.nzslb"))) do
			add_files(filepath)
		end
	end

	if module.custom then
		module.custom()
	end
end

for name, module in pairs(modules) do
	if module.option and not has_config(module.option) then
		goto continue
	end

	target("kanel_" .. name:lower(), function()
		set_group("Modules")

		-- handle shared/static kind
		if is_plat("wasm") or has_config("static") then
			set_kind("static")
		else
			set_kind("shared")
			add_cflags("-fPIC")
		end

		add_ldflags("-Wl,--export-dynamic")

		add_includedirs("Kanel/Runtime/Sources")
		add_rpathdirs("$ORIGIN")

		if has_config("unitybuild") then
			add_rules("c.unity_build", { batchsize = 12 })
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

		ModuleTargetConfig(name, module)
	end)

	::continue::
end

target("kanel_cli")
	set_kind("binary")

	add_includedirs("Kanel/Runtime/Includes")
	add_includedirs("Kanel/Runtime/Sources")
	add_rpathdirs("$ORIGIN")

	add_ldflags("-Wl,--export-dynamic")

	if is_plat("linux") then
		add_syslinks("dl")
	end

	add_packages("vrg")

	if has_config("unitybuild") then
		add_rules("c.unity_build", { batchsize = 0 })
	end

	for _, dir in ipairs(os.dirs("Kanel/Runtime/Sources/*")) do
		if dir ~= "Kanel/Runtime/Sources/Modules" then
			add_files(dir .. "/**.c", { unity_group = dir })
			add_files(dir .. "/**.cpp")
		end
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
target_end()

rule("build.gpu_plugins")
	on_load(function(target)
		if has_config("static") then
			return
		end

		local deps = table.wrap(target:get("deps"))

		if target:kind() == "binary" and table.contains(deps, "kanelcli_gpu") then
			for name, _ in pairs(gpu_backends) do
				local depName = "kanelcli_" .. name:lower()
				if not table.contains(deps, depName) then -- don't overwrite dependency
					target:add("deps", depName, {inherit = false})
				end
			end
		end
	end)
rule_end()
