project "Core"
	kind "StaticLib"
	location "%{wks.location}/Projects/Core"
	
	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/Code/Core/",
	}