#pragma once

#include <Engine/Guid.h>
#include <Engine/Path.h>
#include <Engine/PhysicsMaterialResource.h>
#include <Engine/SoundResource.h>
#include <Engine/TextureResource.h>

#include <map>
#include <entt/resource/handle.hpp>

namespace
{
	class PhysicsManager;
}

namespace core
{
	struct ResourceEntry
	{
		str::Guid m_Guid;
		str::Path m_Filepath;
		entt::id_type m_TypeId;
	};

	class ResourceManager final
	{
	public:
		ResourceManager(physics::PhysicsManager& physicsManager);
		~ResourceManager();

		void Initialize();
		void Destroy();

		template<class TResource>
		entt::resource_handle<TResource> GetResource(const str::Guid& guid);
		template<class TResource>
		entt::resource_handle<TResource> LoadResource(const str::Guid& guid);

		//////////////////////////////////////////////////////////////////////////
		// Physics Material
		template<>
		physics::MaterialHandle GetResource<physics::MaterialResource>(const str::Guid& guid);
		template<>
		physics::MaterialHandle LoadResource<physics::MaterialResource>(const str::Guid& guid);

		//////////////////////////////////////////////////////////////////////////
		// Sound
		template<>
		audio::SoundHandle GetResource<audio::SoundResource>(const str::Guid& guid);
		template<>
		audio::SoundHandle LoadResource<audio::SoundResource>(const str::Guid& guid);

		//////////////////////////////////////////////////////////////////////////
		// Texture
		template<>
		render::TextureHandle GetResource<render::TextureResource>(const str::Guid& guid);
		template<>
		render::TextureHandle LoadResource<render::TextureResource>(const str::Guid& guid);

	private:
		void LoadResources(const char* directory, const bool isSearchingSubdirectories);

	private:
		physics::PhysicsManager& m_PhysicsManager;

		std::map<str::Guid, ResourceEntry> m_ResourceMap;
		physics::MaterialCache m_PhysicsMaterialCache;
		audio::SoundCache m_SoundCache;
		render::TextureCache m_TextureCache;
	};
}

#include "ResourceManager.inl"