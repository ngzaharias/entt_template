project "Engine_ut"
	kind "ConsoleApp"
	dependson { "Core", "Engine" }
	location "%{wks.location}/Projects/Engine_ut"

	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/SFML/Include/",
		"%{wks.location}/Code/Core/",
		"%{wks.location}/Code/Engine/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
	}
	
	links 
	{ 
		"Core.lib",
		"Engine.lib",
	}
