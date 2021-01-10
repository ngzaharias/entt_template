#include "Engine/EnginePCH.h"
#include "Engine/FlipbookAsset.h"

#include "Engine/AssetHelpers.h"
#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"

bool render::FlipbookLoader::create(const core::AssetEntry& entry) const
{
	static const char* s_AssetType = core::ToAssetType(core::EAssetType::Flipbook);

	rapidjson::Document document;
	rapidjson::Value asset_guid;
	rapidjson::Value asset_type;
	rapidjson::Value texture_guid;
	rapidjson::Value sprite_count;
	rapidjson::Value sprite_size_x;
	rapidjson::Value sprite_size_y;

	document.SetObject();
	asset_guid.SetString(entry.m_Guid.ToChar(), document.GetAllocator());
	asset_type.SetString(s_AssetType, document.GetAllocator());
	texture_guid.SetString(str::strNullGuid.ToChar(), document.GetAllocator());
	sprite_count.SetInt(0);
	sprite_size_x.SetInt(0);
	sprite_size_y.SetInt(0);

	document.AddMember("asset_guid", asset_guid, document.GetAllocator());
	document.AddMember("asset_type", asset_type, document.GetAllocator());
	document.AddMember("texture_guid", texture_guid, document.GetAllocator());
	document.AddMember("sprite_count", sprite_count, document.GetAllocator());
	document.AddMember("sprite_size_x", sprite_size_x, document.GetAllocator());
	document.AddMember("sprite_size_y", sprite_size_y, document.GetAllocator());

	return json::SaveDocument(entry.m_Filepath.ToChar(), document);
}

core::AssetPtr<render::FlipbookAsset> render::FlipbookLoader::load(const core::AssetEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath.ToChar(), document);

	const char* texture_guid = json::ParseString(document, "texture_guid", str::strNullGuid.ToChar());
	const int32 sprite_count = json::ParseInt(document, "sprite_count", 0);
	const int32 sprite_size_x = json::ParseInt(document, "sprite_size_x", 0);
	const int32 sprite_size_y = json::ParseInt(document, "sprite_size_y", 0);

	render::FlipbookAsset* Asset = new render::FlipbookAsset();
	Asset->m_Guid = entry.m_Guid;
	Asset->m_Filepath = entry.m_Filepath;
	Asset->m_TextureGuid = str::Name::Create(texture_guid);
	Asset->m_SpriteCount = sprite_count;
	Asset->m_SpriteSize = { sprite_size_x, sprite_size_y };

	return core::AssetPtr<render::FlipbookAsset>(Asset);
}