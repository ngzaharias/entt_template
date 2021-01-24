#pragma once

#include "Editor/AssetPopup.h"

// #todo: implement asset browser in a pop-up
// #todo: copy and highlight for name and guid

template<typename Type>
void widget::TypeOverload(core::AssetHandle<Type>& handle, const Attributes attributes /*= {}*/)
{
	editor::AssetPopup::SelectOne(handle);
}
