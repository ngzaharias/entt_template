#pragma once

template <class Component>
void debug::EnttDebugger::RegisterWidget(WidgetCallback&& callback)
{
	m_ComponentWidgets[entt::type_info<Component>::id()] = callback;
}