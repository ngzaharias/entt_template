#pragma once

#include <Engine/Name.h>
#include <Engine/Path.h>
#include <Engine/SoundResource.h>
#include <Engine/TextureResource.h>

#include <map>
#include <entt/entt.hpp>

namespace core
{
	class ResourceManager final
	{
	public:
		ResourceManager();
		~ResourceManager();

		void Initialize();
		void Destroy();

		template<class TResource>
		entt::resource_handle<TResource> GetResource(const str::Path& filepath);
		template<class TResource>
		entt::resource_handle<TResource> LoadResource(const str::Path& filepath);

		//////////////////////////////////////////////////////////////////////////
		// Sound
		template<>
		SoundHandle GetResource<core::SoundResource>(const str::Path& filepath);
		template<>
		SoundHandle LoadResource<core::SoundResource>(const str::Path& filepath);

		//////////////////////////////////////////////////////////////////////////
		// Texture
		template<>
		TextureHandle GetResource<core::TextureResource>(const str::Path& filepath);
		template<>
		TextureHandle LoadResource<core::TextureResource>(const str::Path& filepath);

	private:
		core::SoundCache m_SoundCache;
		core::TextureCache m_TextureCache;
	};
}

#include "ResourceManager.inl"