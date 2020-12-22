#pragma once

#include <Engine/ResourceTypes.h>

namespace core
{
	class ResourceManager;
}

namespace editor
{
	class AssetPopup
	{
	public:
		template<class TResource>
		static void Resource(core::ResourceHandle<TResource>& handle);
	};
}

#include "AssetPopup.inl"