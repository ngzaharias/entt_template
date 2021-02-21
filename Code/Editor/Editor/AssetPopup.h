#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetHandle.h>

namespace editor
{
	class AssetPopup
	{
	public:
		template<typename Type>
		static void SelectOne(core::AssetHandle<Type>& handle);
	};
}

#include "AssetPopup.inl"