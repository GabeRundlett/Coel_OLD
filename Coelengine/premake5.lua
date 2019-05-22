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
			--Platform specific code
			"src/Coel/Platform/Linux/**.hpp",
			"src/Coel/Platform/Linux/**.cpp",

			"src/Coel/Platform/MacOS/**.hpp",
			"src/Coel/Platform/MacOS/**.cpp",

			--Renderer specific code
			"src/Coel/Graphics/Metal/**.hpp",
			"src/Coel/Graphics/Metal/**.cpp"
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
		cppdialect "C++17"
		defines "COEL_PLATFORM_MACOS"
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