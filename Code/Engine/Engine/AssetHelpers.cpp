#include "EnginePCH.h"
#include "Engine/AssetHelpers.h"

#include "Engine/StringHelpers.h"
#include "Engine/StringView.h"

core::EAssetType core::ToAssetType(const str::StringView& string)
{
	if (str::Equals(string, "flipbook"))
		return core::EAssetType::Flipbook;
	if (str::Equals(string, "physics_material"))
		return core::EAssetType::PhysicsMaterial;
	if (str::Equals(string, "sound"))
		return core::EAssetType::Sound;
	if (str::Equals(string, "sprite"))
		return core::EAssetType::Sprite;
	if (str::Equals(string, "template"))
		return core::EAssetType::Template;
	if (str::Equals(string, "texture"))
		return core::EAssetType::Texture;

	assert(false);
	return core::EAssetType::Unknown;
}

const char* core::ToAssetType(core::EAssetType type)
{
	switch (type)
	{
	case core::EAssetType::Flipbook:
		return "flipbook";
	case core::EAssetType::PhysicsMaterial:
		return "physics_material";
	case core::EAssetType::Sound:
		return "sound";
	case core::EAssetType::Sprite:
		return "sprite";
	case core::EAssetType::Template:
		return "template";
	case core::EAssetType::Texture:
		return "texture";
	}

	assert(false);
	return "unknown";
}
