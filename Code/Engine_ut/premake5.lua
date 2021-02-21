project "Engine_ut"
	kind "ConsoleApp"
	dependson { "Engine" }
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
		"%{wks.location}/Code/Engine/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/" 
	}
	
	links 
	{ 
		"Engine.lib" 
	}
