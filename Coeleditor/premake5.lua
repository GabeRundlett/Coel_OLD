project "Coeleditor"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Build/bin/intermediates/" .. outputdir .. "/%{prj.name}")
	
	files {
		"src/**.hpp",
		"src/**.cpp",
	}

	includedirs {
		"%{wks.location}/Coelengine/src"
	}

	links {
		"Coelengine"
	}

	filter "configurations:Debug"
		defines "COEL_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "COEL_RELEASE"
		optimize "On"
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
	filter "system:linux"
		links {
			"dl",
			"X11",
			"pthread",
			"GLFW",
			"glad",
			"clm"
		}
	filter "system:macosx"