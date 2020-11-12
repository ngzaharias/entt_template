#pragma once

#include <entt/core/type_info.hpp>

template <class Component>
void debug::EnttDebugger::RegisterWidget(WidgetCallback&& callback)
{
	m_ComponentWidgets[entt::type_info<Component>::id()] = callback;
}