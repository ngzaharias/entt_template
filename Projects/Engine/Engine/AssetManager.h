#pragma once

#include <Engine/AssetHandle.h>
#include <Engine/AssetTypes.h>
#include <Engine/EntityTemplateAsset.h>
#include <Engine/Name.h>
#include <Engine/PhysicsMaterialAsset.h>
#include <Engine/SoundAsset.h>
#include <Engine/TextureAsset.h>

#include <map>

namespace editor
{
	class AssetBrowser;
}

namespace physics
{
	class PhysicsManager;
}

namespace core
{
	using AssetEntryMap = std::map<str::Name, AssetEntry>;
	
	class AssetManager final
	{
		friend class editor::AssetBrowser;

	public:
		static AssetManager& Instance();

		AssetManager(physics::PhysicsManager& physicsManager);
		~AssetManager();

		void Initialize();
		void Destroy();

		const AssetEntryMap& GetEntries() const { return m_AssetEntryMap; }

		template<class Type>
		void CreateAsset(const str::Path& filepath);
		template<>
		void CreateAsset<physics::MaterialAsset>(const str::Path& filepath);

		template<class Type>
		void ImportAsset(const str::Path& inputPath, const str::Path& outputPath);
		template<>
		void ImportAsset<audio::SoundAsset>(const str::Path& inputPath, const str::Path& outputPath);
		template<>
		void ImportAsset<render::TextureAsset>(const str::Path& inputPath, const str::Path& outputPath);

		template<class Type>
		core::AssetHandle<Type> LoadAsset(const str::Name& guid);
		template<>
		core::EntityTemplateHandle LoadAsset<core::EntityTemplateAsset>(const str::Name& guid);
		template<>
		physics::MaterialHandle LoadAsset<physics::MaterialAsset>(const str::Name& guid);
		template<>
		audio::SoundHandle LoadAsset<audio::SoundAsset>(const str::Name& guid);
		template<>
		render::TextureHandle LoadAsset<render::TextureAsset>(const str::Name& guid);

	private:
		void LoadDirectory(const char* directory, const bool isSearchingSubdirectories);
		void LoadFile(const char* filepath);

	private:
		physics::PhysicsManager& m_PhysicsManager;

		core::AssetEntryMap m_AssetEntryMap;

		core::EntityTemplateCache m_EntityTemplateCache;
		physics::MaterialCache m_PhysicsMaterialCache;
		audio::SoundCache m_SoundCache;
		render::TextureCache m_TextureCache;
	};
}

#include "AssetManager.inl"