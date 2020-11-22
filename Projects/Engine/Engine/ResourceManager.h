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
	class ResourceManager final
	{
		friend editor::AssetBrowser;

	public:
		ResourceManager(physics::PhysicsManager& physicsManager);
		~ResourceManager();

		void Initialize();
		void Destroy();

		template<class TResource>
		void CreateResource(const str::Path& filepath);
		template<>
		void CreateResource<physics::MaterialResource>(const str::Path& filepath);

		template<class TResource>
		entt::resource_handle<TResource> LoadResource(const str::Name& name);
		template<>
		physics::MaterialHandle LoadResource<physics::MaterialResource>(const str::Name& name);
		template<>
		audio::SoundHandle LoadResource<audio::SoundResource>(const str::Name& name);
		template<>
		render::TextureHandle LoadResource<render::TextureResource>(const str::Name& name);

	private:
		void LoadDirectory(const char* directory, const bool isSearchingSubdirectories);

	private:
		physics::PhysicsManager& m_PhysicsManager;

		std::map<str::Name, ResourceEntry> m_ResourceEntries;
		physics::MaterialCache m_PhysicsMaterialCache;
		audio::SoundCache m_SoundCache;
		render::TextureCache m_TextureCache;
	};
}

#include "ResourceManager.inl"