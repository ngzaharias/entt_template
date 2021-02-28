#pragma once

// #todo: m_ComponentEntries: convert to map
// #todo: m_ComponentEntries: assert on duplicates
// #todo: m_SystemEntries: convert to map
// #todo: m_SystemEntries: assert on duplicates

template<class TSystem>
TSystem& core::Application::GetSystem() const
{
	const SystemEntry::TypeId& typeId = entt::type_info<TSystem>::id();

	const auto first = m_SystemEntries.begin();
	const auto last = m_SystemEntries.end();
	auto result = std::find_if(first, last, [&](const SystemEntry& entry)
	{
		return entry.m_TypeId == typeId;
	});

	assert(result != last);
	return *static_cast<TSystem*>(result->m_System);
}

template<class TComponent>
void core::Application::RegisterComponent()
{
	ComponentEntry entry =
	{
		entt::type_info<TComponent>::id()
	};
	m_ComponentEntries.push_back(entry);
}

template<class TSystem, typename... TArgs>
void core::Application::RegisterSystem(TArgs&&... args)
{
	SystemEntry entry =
	{
		entt::type_info<TSystem>::id()
		, new TSystem(std::forward<TArgs>(args)...)
	};
	m_SystemEntries.push_back(entry);
}