#pragma once

#include "Editor/AssetPopup.h"

// #todo: implement asset browser in a pop-up
// #todo: copy and highlight for name and guid

template<typename Type>
void widget::TypeAsIs(core::AssetHandle<Type>& handle)
{
	editor::AssetPopup::SelectOne(handle);
}
