project "Coeleditor"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Build/bin/intermediates/" .. outputdir .. "/%{prj.name}")
	
	files {
		"**.hpp",
		"**.cpp",
	}
	
	includedirs {
		"%{IncludeDir.Coelengine}",
		"%{IncludeDir.clm}"
	}
	
	filter "configurations:Debug"
		defines "CONFIG_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "CONFIG_RELEASE"
		optimize "On"

	filter "system:windows"
		links {
			"Coelengine",
			"clm"
		}
	filter "system:linux"
		links {
			"Coelengine",
			"clm",
			"GLFW",
			"glad",
			"X11",
			"dl",
			"pthread"
		}
	filter "system:macosx"
