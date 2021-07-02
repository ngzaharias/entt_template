project "Engine"
	kind "StaticLib"
	dependson { "Core", "Imgui" }
	pchheader "EnginePCH.h"
	pchsource "Engine/EnginePCH.cpp"
	location "%{wks.location}/Projects/Engine"
	
	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/optick/1.3.1/include/",
		"%{wks.location}/3rdParty/PhysX/Include/",
		"%{wks.location}/3rdParty/SFML/Include/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Engine/",
		"%{wks.location}/Code/Imgui/",
	}

	libdirs 
	{
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{ 
		"Core.lib",
	}