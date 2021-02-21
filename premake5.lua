workspace "ZEngine"
	configurations { "Debug", "Release" }
	language "C++"
	startproject "Editor"

include "./Projects/Editor/premake5.lua"
include "./Projects/Engine/premake5.lua"
include "./Projects/Engine_ut/premake5.lua"
include "./Projects/Game/premake5.lua"
include "./Projects/Imgui/premake5.lua"