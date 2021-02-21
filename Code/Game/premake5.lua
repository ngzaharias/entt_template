project "Game"
	kind "WindowedApp"
	dependson { "Engine", "Imgui" }
	pchheader "GamePCH.h"
	pchsource "Game/GamePCH.cpp"
	location "%{wks.location}/Projects/Game"

	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/PhysX/Include/",
		"%{wks.location}/3rdParty/SFML/Include/",
		"%{wks.location}/Code/Engine/",
		"%{wks.location}/Code/Game/",
		"%{wks.location}/Code/Imgui/",
	}

	libdirs 
	{
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/PhysX/Library/%{cfg.buildcfg}/",
		"%{wks.location}/3rdParty/SFML/Library/%{cfg.buildcfg}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Imgui/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{ 
		"Engine.lib",
		"Imgui.lib",

		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXPvdSDK_static_64.lib",

		"flac.lib",
		"freetype.lib",
		"gdi32.lib",
		"ogg.lib",
		"openal32.lib",
		"opengl32.lib",
		"vorbis.lib",
		"vorbisenc.lib",
		"vorbisfile.lib",
		"winmm.lib",
		"ws2_32.lib",
		"winmm.lib",
	}

	filter "Debug"
		links 
		{ 
			"sfml-audio-d.lib",
			"sfml-graphics-d.lib",
			"sfml-main-d.lib",
			"sfml-network-d.lib",
			"sfml-system-d.lib",
			"sfml-window-d.lib",
		}
	filter "Release"
		links 
		{ 
			"sfml-audio.lib",
			"sfml-graphics.lib",
			"sfml-main.lib",
			"sfml-network.lib",
			"sfml-system.lib",
			"sfml-window.lib",
		}
	filter {} -- disable the filter

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/3rdParty/*.dll $(OutDir)",
		"{COPY} %{wks.location}/3rdParty/PhysX/Binary/%{cfg.buildcfg}/*.dll $(OutDir)",
		"{COPY} %{wks.location}/3rdParty/SFML/Binary/%{cfg.buildcfg}/*.dll $(OutDir)",
	}