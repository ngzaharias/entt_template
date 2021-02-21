project "Engine"
	kind "StaticLib"
	defines { "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING" }
	pchheader "EnginePCH.h"
	pchsource "Engine/EnginePCH.cpp"
	dependson { "Imgui" }

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

	filter "configurations:Debug"
		defines { "_DEBUG" }
	filter "configurations:Release"
		defines { "NDEBUG" }
