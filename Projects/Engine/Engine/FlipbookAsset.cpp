#include "Engine/EnginePCH.h"
#include "Engine/FlipbookAsset.h"

#include "Engine/AssetHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"
#include "Engine/SpriteAsset.h"

bool render::FlipbookLoader::save(const FlipbookAsset& asset, const core::AssetEntry& entry) const
{
	static const char* s_AssetType = core::ToAssetType(core::EAssetType::Flipbook);

	rapidjson::Document document;
	rapidjson::Value asset_guid;
	rapidjson::Value asset_type;
	rapidjson::Value fps;
	rapidjson::Value frames;
	auto& allocator = document.GetAllocator();

	document.SetObject();
	asset_guid.SetString(entry.m_Guid.ToChar(), allocator);
	asset_type.SetString(s_AssetType, allocator);
	fps.SetFloat(asset.m_FPS);
	frames.SetArray();
	for (const render::FlipbookFrame& value : asset.m_Frames)
	{
		str::Name spriteGuid = str::strNullGuid;
		if (value.m_Sprite)
		{
			const render::SpriteAsset& sprite = value.m_Sprite.get();
			spriteGuid = sprite.m_Guid;
		}

		rapidjson::Value frame;
		rapidjson::Value frame_count;
		rapidjson::Value sprite_guid;
		frame.SetObject();
		frame_count.SetInt(value.m_FrameCount);
		sprite_guid.SetString(spriteGuid.ToChar(), allocator);

		frame.AddMember("frame_count", frame_count, allocator);
		frame.AddMember("sprite_guid", sprite_guid, allocator);
		frames.PushBack(frame, allocator);
	}

	document.AddMember("asset_guid", asset_guid, allocator);
	document.AddMember("asset_type", asset_type, allocator);
	document.AddMember("fps", fps, allocator);
	document.AddMember("frames", frames, allocator);

	return json::SaveDocument(entry.m_Filepath, document);
}

core::AssetPtr<render::FlipbookAsset> render::FlipbookLoader::load(const core::AssetEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath, document);

	render::FlipbookAsset* asset = new render::FlipbookAsset();
	asset->m_Guid = entry.m_Guid;
	asset->m_Filepath = entry.m_Filepath;
	asset->m_FPS = json::ParseFloat(document, "fps", 30);
	json::ParseArray(document, "frames", [&asset](const rapidjson::Value& child)
	{
		const char* sprite_guid = json::ParseString(child, "sprite_guid", str::strNullGuid.ToChar());
		const str::Name spriteGuid = str::Name::Create(sprite_guid);

		render::FlipbookFrame frame;
		frame.m_FrameCount = json::ParseInt(child, "frame_count", 1);
		frame.m_Sprite = core::AssetManager::Instance().LoadAsset<render::SpriteAsset>(spriteGuid);
		
		asset->m_Frames.push_back(std::move(frame));

		return true;
	});

	return core::AssetPtr<render::FlipbookAsset>(asset);
}