project "Coelengine"
	kind "StaticLib"
	language "C++"
	
	targetdir("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Build/bin/intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "clpch.hpp"
	pchsource "src/clpch.cpp"
	
	files {
		"src/**.hpp",
		"src/**.cpp",
	}
	
	includedirs {
		"%{IncludeDir.Coelengine}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.clm}"
	}
	defines {
		"GLFW_INCLUDE_NONE"
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
		defines "COEL_PLATFORM_WINDOWS"
		excludes {
			"src/Coel/Platform/Linux/**.hpp",
			"src/Coel/Platform/Linux/**.cpp"
		}
		links {
			"GLFW",
			"glad",
			"clm"
		}
	filter "system:linux"
		cppdialect "C++17"
		defines "COEL_PLATFORM_LINUX"
		excludes {
			"src/Coel/Platform/Windows/**.hpp",
			"src/Coel/Platform/Windows/**.cpp"
		}
	filter "system:macosx"
		defines "COEL_PLATFORM_MACOS"