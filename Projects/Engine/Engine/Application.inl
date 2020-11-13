#pragma once

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

template<class TSystem, typename... TArgs>
void core::Application::RegisterSystem(TArgs&&... args)
{
	SystemEntry entry =
	{
		new TSystem(std::forward<TArgs>(args)...)
		, entt::type_info<TSystem>::id()
	};
	m_SystemEntries.push_back(entry);
}