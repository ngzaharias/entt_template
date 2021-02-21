#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>

#include <SFML/Audio/SoundBuffer.hpp>

namespace core
{
	struct AssetEntry;
}

namespace audio
{
	struct SoundAsset : public core::Asset
	{
		sf::SoundBuffer m_SoundBuffer;
		str::Path m_SourceFile;
	};

	class SoundLoader : public core::AssetLoader<SoundLoader, SoundAsset>
	{
	public:
		bool Import(const str::Path& inputPath, const str::Path& outputPath) const;
		core::AssetPtr<SoundAsset> load(const core::AssetEntry& entry) const;
	};

	using SoundCache = core::AssetCache<SoundAsset>;
	using SoundHandle = core::AssetHandle<SoundAsset>;
}