#pragma once

#include <Engine/Name.h>
#include <Engine/PhysicsMaterialResource.h>
#include <Engine/ResourceTypes.h>
#include <Engine/SoundResource.h>
#include <Engine/TextureResource.h>

#include <map>
#include <entt/resource/handle.hpp>

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
	using ResourceEntryMap = std::map<str::Name, ResourceEntry>;
	
	class ResourceManager final
	{

	public:
		static ResourceManager& Instance();

		ResourceManager(physics::PhysicsManager& physicsManager);
		~ResourceManager();

		void Initialize();
		void Destroy();

		const ResourceEntryMap& GetEntries() const { return m_Entries; }

		template<class TResource>
		void CreateResource(const str::Path& filepath);
		template<>
		void CreateResource<physics::MaterialResource>(const str::Path& filepath);

		template<class TResource>
		void ImportResource(const str::Path& inputPath, const str::Path& outputPath);
		template<>
		void ImportResource<audio::SoundResource>(const str::Path& inputPath, const str::Path& outputPath);
		template<>
		void ImportResource<render::TextureResource>(const str::Path& inputPath, const str::Path& outputPath);

		template<class TResource>
		entt::resource_handle<TResource> LoadResource(const str::Name& guid);
		template<>
		physics::MaterialPtr LoadResource<physics::MaterialResource>(const str::Name& guid);
		template<>
		audio::SoundPtr LoadResource<audio::SoundResource>(const str::Name& guid);
		template<>
		render::TexturePtr LoadResource<render::TextureResource>(const str::Name& guid);

	private:
		void LoadDirectory(const char* directory, const bool isSearchingSubdirectories);
		void LoadFile(const char* filepath);

	private:
		physics::PhysicsManager& m_PhysicsManager;

		ResourceEntryMap m_Entries;

		physics::MaterialCache m_PhysicsMaterialCache;
		audio::SoundCache m_SoundCache;
		render::TextureCache m_TextureCache;
	};
}

#include "ResourceManager.inl"