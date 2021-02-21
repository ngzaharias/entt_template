project "Engine_ut"
	kind "ConsoleApp"
	dependson { "Engine" }

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
		"%{wks.location}/Projects/Engine/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/" 
	}
	
	links 
	{ 
		"Engine.lib" 
	}
