workspace "ZEngine"
	architecture "x86_64"
	configurations { "Debug", "Release" }
	language "C++"
	cppdialect "C++17"
	platforms { "x64" }
	startproject "Editor"

	targetdir "%{wks.location}/Build/%{prj.name}/%{cfg.buildcfg}_%{cfg.platform}/"
	objdir "%{wks.location}/Intermediate/%{prj.name}/%{cfg.buildcfg}_%{cfg.platform}/"

	defines { "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING" }

	filter "Debug"
		defines { "_DEBUG" }
		optimize "Off"
		runtime "Debug"
		symbols "On"
	filter "Release"
		defines { "NDEBUG" }
		optimize "Speed"
		runtime "Release"
		symbols "Off"
	filter {} -- disable the filter

	-- projects
	include "Projects/Imgui/premake5.lua"
	include "Projects/Engine/premake5.lua"
	include "Projects/Game/premake5.lua"
	include "Projects/Editor/premake5.lua"

	group("ut")
		include "Projects/Engine_ut/premake5.lua"