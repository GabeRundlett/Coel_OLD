workspace "Coel"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Coelengine"] = "%{wks.location}/Coelengine/src"
IncludeDir["dep"] = "%{wks.location}/Coelengine/dep"
IncludeDir["GLFW"] = "%{IncludeDir.dep}/GLFW/include"
IncludeDir["glad"] = "%{IncludeDir.dep}/glad/include"
IncludeDir["clm"] = "%{IncludeDir.dep}/clm/src"

include "Coelengine/dep/GLFW"
include "Coelengine/dep/glad"
include "Coelengine/dep/clm"
include "Coelengine"
include "Coeleditor"