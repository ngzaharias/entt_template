#include "EnginePCH.h"
#include "Engine/SpriteAsset.h"

#include "Engine/AssetHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"

bool render::SpriteLoader::save(const render::SpriteAsset& asset, const core::AssetEntry& entry) const
{
	static const char* s_AssetType = core::ToAssetType(core::EAssetType::Sprite);

	str::Name textureGuid = str::strNullGuid;
	if (asset.m_Texture)
	{
		const render::TextureAsset& textureAsset = asset.m_Texture.get();
		textureGuid = textureAsset.m_Guid;
	}

	rapidjson::Document document;
	rapidjson::Value asset_guid;
	rapidjson::Value asset_type;
	rapidjson::Value texture_guid;
	rapidjson::Value rectangle_left;
	rapidjson::Value rectangle_top;
	rapidjson::Value rectangle_width;
	rapidjson::Value rectangle_height;

	document.SetObject();
	asset_guid.SetString(entry.m_Guid.ToChar(), document.GetAllocator());
	asset_type.SetString(s_AssetType, document.GetAllocator());
	texture_guid.SetString(textureGuid.ToChar(), document.GetAllocator());
	rectangle_left.SetInt(asset.m_RectanglePos.x);
	rectangle_top.SetInt(asset.m_RectanglePos.y);
	rectangle_width.SetInt(asset.m_RectangleSize.x);
	rectangle_height.SetInt(asset.m_RectangleSize.y);

	document.AddMember("asset_guid", asset_guid, document.GetAllocator());
	document.AddMember("asset_type", asset_type, document.GetAllocator());
	document.AddMember("texture_guid", texture_guid, document.GetAllocator());
	document.AddMember("rectangle_left", rectangle_left, document.GetAllocator());
	document.AddMember("rectangle_top", rectangle_top, document.GetAllocator());
	document.AddMember("rectangle_width", rectangle_width, document.GetAllocator());
	document.AddMember("rectangle_height", rectangle_height, document.GetAllocator());

	return json::SaveDocument(entry.m_Filepath, document);
}

core::AssetPtr<render::SpriteAsset> render::SpriteLoader::load(const core::AssetEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath, document);

	const char* texture_guid = json::ParseString(document, "texture_guid", str::strNullGuid.ToChar());
	const str::Name textureGuid = NAME(texture_guid);

	render::SpriteAsset* asset = new render::SpriteAsset();
	asset->m_Guid = entry.m_Guid;
	asset->m_Filepath = entry.m_Filepath;
	asset->m_RectanglePos =
	{ 
		json::ParseUint(document, "rectangle_left", 0)
		, json::ParseUint(document, "rectangle_top", 0)
	};
	asset->m_RectangleSize =
	{
		json::ParseUint(document, "rectangle_width", 0)
		, json::ParseUint(document, "rectangle_height", 0)
	};
	asset->m_Texture = core::AssetManager::Instance().LoadAsset<render::TextureAsset>(textureGuid);

	return core::AssetPtr<render::SpriteAsset>(asset);
}