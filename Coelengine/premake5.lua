project "Coelengine"
	kind "StaticLib"
	language "C++"
	
	targetdir("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Build/bin/intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.hpp"
	pchsource "src/pch.cpp"
	
	files {
		"src/**.hpp",
		"src/**.cpp",
		"include/**.hpp",
	}
	
	includedirs {
		"%{wks.location}/Coelengine/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.clm}"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "configurations:Debug"
		defines "CONFIG_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "CONFIG_RELEASE"
		optimize "On"
	
	filter "system:windows"
		systemversion "latest"
		defines "CONFIG_PLATFORM_WINDOWS"
		links {
			"GLFW",
			"glad"
		}
		excludes {
			--Platform specific code
			"src/Coel/Platform/Linux/**.hpp",
			"src/Coel/Platform/Linux/**.cpp",

			"src/Coel/Platform/MacOS/**.hpp",
			"src/Coel/Platform/MacOS/**.cpp",

			--Renderer specific code
			"src/Coel/Graphics/Metal/**.hpp",
			"src/Coel/Graphics/Metal/**.cpp"
		}
	filter "system:linux"
		defines "CONFIG_PLATFORM_LINUX"
		excludes {
			--Platform specific code
			"src/Coel/Platform/Windows/**.hpp",
			"src/Coel/Platform/Windows/**.cpp",

			"src/Coel/Platform/MacOS/**.hpp",
			"src/Coel/Platform/MacOS/**.cpp",

			--Renderer specific code
			"src/Coel/Graphics/DirectX/**.hpp",
			"src/Coel/Graphics/DirectX/**.cpp",

			"src/Coel/Graphics/Metal/**.hpp",
			"src/Coel/Graphics/Metal/**.cpp"
		}
	filter "system:macosx"
		defines "CONFIG_PLATFORM_MACOS"
		excludes {
			--Platform specific code
			"src/Coel/Platform/Windows/**.hpp",
			"src/Coel/Platform/Windows/**.cpp",

			"src/Coel/Platform/Linux/**.hpp",
			"src/Coel/Platform/Linux/**.cpp",

			--Renderer specific code
			"src/Coel/Graphics/DirectX/**.hpp",
			"src/Coel/Graphics/DirectX/**.cpp"
		}
		