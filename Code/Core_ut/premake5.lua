project "Core_ut"
	kind "ConsoleApp"
	dependson { "Core" }
	location "%{wks.location}/Projects/Core_ut"

	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/Code/Core/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/" 
	}
	
	links 
	{ 
		"Core.lib" 
	}
