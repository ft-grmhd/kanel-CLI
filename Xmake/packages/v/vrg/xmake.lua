package("vrg")
	set_kind("library", { headeronly = true })
	set_homepage("https://github.com/rdentato/vrg")
	set_license("MIT")

	add_urls("https://github.com/rdentato/vrg.git")

	on_install("windows", "linux", "mingw", "macosx", "iphoneos", "android", "bsd", function(package)
		os.cp("vrg.h", package:installdir("include"))
	end)

	on_test(function(package)
		local defines = "VRGCLI"

		assert(package:check_csnippets({test = [[
			#include <vrg.h>
			#include <stdio.h>
			int main(int argc, char *argv[])
			{
				vrgcli("My Awesome CLI Program v1.0")
				{
					vrgarg("-h\tDisplay help")
					{
						vrgusage();
					}
					vrgarg("-f [filename]\tSpecify an optional filename")
					{
						printf("Filename: %s\n", vrgarg);
					}
					vrgarg("input\tInput file")
					{
						printf("Processing input file: %s\n", vrgarg);
					}
					vrgarg()
					{
						vrgusage("Unexpected argument: '%s'\n", vrgarg);
					}
				}
				return 0;
			}
		]]}, { configs = { defines = defines } }))
	end)

