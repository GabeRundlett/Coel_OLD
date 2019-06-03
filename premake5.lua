workspace "Coel"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Coelengine"] = "%{wks.location}/Coelengine/include"
IncludeDir["dep"] = "%{wks.location}/Coelengine/dep"
IncludeDir["GLFW"] = "%{IncludeDir.dep}/GLFW/include"
IncludeDir["glad"] = "%{IncludeDir.dep}/glad/include"
IncludeDir["clm"] = "%{IncludeDir.dep}/clm/src"

group "Dependencies"
	include "Coelengine/dep/GLFW"
	include "Coelengine/dep/glad"
	include "Coelengine/dep/clm"

group ""
	include "Coelengine"
	include "Coeleditor"
