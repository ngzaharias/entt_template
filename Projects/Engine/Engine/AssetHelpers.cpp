#include "Engine/EnginePCH.h"
#include "Engine/AssetHelpers.h"

#include "Engine/StringHelpers.h"
#include "Engine/StringView.h"

core::EAssetType core::ToAssetType(const str::StringView& string)
{
	if (str::Equals(string, "entity_template"))
		return core::EAssetType::EntityTemplate;
	if (str::Equals(string, "physics_material"))
		return core::EAssetType::PhysicsMaterial;
	if (str::Equals(string, "sound"))
		return core::EAssetType::Sound;
	if (str::Equals(string, "texture"))
		return core::EAssetType::Texture;

	assert(false);
	return core::EAssetType::Unknown;
}

const char* core::ToAssetType(core::EAssetType type)
{
	switch (type)
	{
	case core::EAssetType::EntityTemplate:
		return "entity_template";
	case core::EAssetType::PhysicsMaterial:
		return "physics_material";
	case core::EAssetType::Sound:
		return "sound";
	case core::EAssetType::Texture:
		return "texture";
	}

	assert(false);
	return "unknown";
}
