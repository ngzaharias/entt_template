project "Imgui"
	kind "StaticLib"
	location "%{wks.location}/Projects/Imgui"
	
	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.inl",
		"%{wks.location}/3rdParty/imgui/*.h",
		"%{wks.location}/3rdParty/imgui/*.cpp",
		"%{wks.location}/3rdParty/imgui/*.inl",
		"%{wks.location}/3rdParty/imgui-sfml/*.h",
		"%{wks.location}/3rdParty/imgui-sfml/*.cpp",
		"%{wks.location}/3rdParty/imgui-sfml/*.inl",
		"%{wks.location}/3rdParty/ImGuizmo/*.h",
		"%{wks.location}/3rdParty/ImGuizmo/*.cpp",
		"%{wks.location}/3rdParty/ImGuizmo/*.inl",
	}

	includedirs 
	{ 
		"%{wks.location}/3rdParty/",
		"%{wks.location}/3rdParty/imgui/",
		"%{wks.location}/3rdParty/imgui-sfml/",
		"%{wks.location}/3rdParty/ImGuizmo/",
		"%{wks.location}/3rdParty/SFML/Include/",
		"%{wks.location}/Code/Imgui/",
	}
