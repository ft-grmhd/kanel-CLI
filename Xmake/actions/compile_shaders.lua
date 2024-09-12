-- Copyright (C) 2024 kanel
-- This file is part of "kanel-CLI"
-- For conditions of distribution and use, see copyright notice in LICENSE

task("compile-shaders")
	set_menu({
		-- Settings menu usage
		usage = "xmake compile-shaders [options]",
		description = "Compile shaders",
		options = {
			{nil, "measure", "k", nil, "Measure time taken for every step of the shader compilation." },
			{nil, "benchmark-iteration", "kv", nil, "Benchmark using a number of iterations." },
		}
	})

	on_run(function ()
		import("core.base.option")
		import("core.base.task")
		import("core.project.project")

		task.run("config", {}, {disable_dump = true})

		local nzsl = project.required_package("nzsl")
		local nzslc = path.join(nzsl:installdir(), "bin", "nzslc")

		local envs = nzsl:get("envs")
		if is_plat("mingw") then
			local mingw = toolchain.load("mingw")
			if mingw and mingw:check() then
				for name, value in pairs(mingw:runenvs()) do
					envs[name] = table.join(envs[name] or {}, table.wrap(value))
				end
			end
		end

		print("Compiling shaders...")
		for _, filepath in pairs(os.files("Sources/GPU/*/Resources/**.nzsl")) do
			print(" - Compiling " .. filepath)
			local argv = {"--compile=nzslb-header", "--partial", "--optimize" }
			if option.get("measure") then
				table.insert(argv, "--measure")
			end
			if option.get("benchmark-iteration") then
				table.insert(argv, "--benchmark-iteration=" .. option.get("benchmark-iteration"))
			end
			table.insert(argv, filepath)
			os.vrunv(nzslc, argv, { envs = envs })
		end
	end)
