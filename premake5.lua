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

	files 
	{ 
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/Code/%{prj.name}/%{prj.name}/**.inl",
	}

	vpaths 
	{ 
		["Source/"] = { "Code/**.h", "Code/**.cpp", "Code/**.inl" },
	}

	-- projects
	include "Code/Imgui/premake5.lua"
	include "Code/Engine/premake5.lua"
	include "Code/Game/premake5.lua"
	include "Code/Editor/premake5.lua"

	group "ut"
		include "Code/Engine_ut/premake5.lua"
	group ""

project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	location "%{wks.location}/Projects/ZERO_CHECK"
