project "Engine"
	kind "StaticLib"
	dependson { "Imgui" }
	pchheader "EnginePCH.h"
	pchsource "Engine/EnginePCH.cpp"
	
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
		"%{wks.location}/Projects/Engine/",
		"%{wks.location}/Projects/Imgui/",
	}