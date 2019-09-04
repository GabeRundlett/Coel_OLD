workspace "coel"
	architecture "x64"
	configurations { "Debug", "DebugOptimized", "Release" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
inc = {}
inc["engine"] = "%{wks.location}/engine/src"
inc["glad"] = "%{wks.location}/glad/include"
inc["glfw"] = "%{wks.location}/glfw/include"
inc["stb"] = "%{wks.location}/stb/src"
inc["math"] = "%{wks.location}/math/src"

startproject "test"

include "engine"
include "math"
include "test"

flags { "MultiProcessorCompile" }
filter "configurations:DebugOptimized"
	flags { "LinkTimeOptimization" }
filter "configurations:Release"
	flags { "LinkTimeOptimization" }
