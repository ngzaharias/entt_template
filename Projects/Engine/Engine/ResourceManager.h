#pragma once

#include <Engine/Path.h>
#include <Engine/PhysicsMaterialResource.h>
#include <Engine/SoundResource.h>
#include <Engine/TextureResource.h>

#include <entt/resource/handle.hpp>

namespace
{
	class PhysicsManager;
}

namespace core
{
	class ResourceManager final
	{
	public:
		ResourceManager(physics::PhysicsManager& physicsManager);
		~ResourceManager();

		void Initialize();
		void Destroy();

		template<class TResource>
		entt::resource_handle<TResource> GetResource(const str::Path& filepath);
		template<class TResource>
		entt::resource_handle<TResource> LoadResource(const str::Path& filepath);

		//////////////////////////////////////////////////////////////////////////
		// Physics Material
		template<>
		physics::MaterialHandle GetResource<physics::MaterialResource>(const str::Path& filepath);
		template<>
		physics::MaterialHandle LoadResource<physics::MaterialResource>(const str::Path& filepath);

		//////////////////////////////////////////////////////////////////////////
		// Sound
		template<>
		audio::SoundHandle GetResource<audio::SoundResource>(const str::Path& filepath);
		template<>
		audio::SoundHandle LoadResource<audio::SoundResource>(const str::Path& filepath);

		//////////////////////////////////////////////////////////////////////////
		// Texture
		template<>
		render::TextureHandle GetResource<render::TextureResource>(const str::Path& filepath);
		template<>
		render::TextureHandle LoadResource<render::TextureResource>(const str::Path& filepath);

	private:
		physics::PhysicsManager& m_PhysicsManager;

		physics::MaterialCache m_PhysicsMaterialCache;
		audio::SoundCache m_SoundCache;
		render::TextureCache m_TextureCache;
	};
}

#include "ResourceManager.inl"