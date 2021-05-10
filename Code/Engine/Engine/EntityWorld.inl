#pragma once

#include <Engine/TypeId.h>

// #todo: m_ComponentEntries: convert to map
// #todo: m_ComponentEntries: assert on duplicates
// #todo: m_SystemEntries: convert to map
// #todo: m_SystemEntries: assert on duplicates

template<class TSystem>
TSystem& ecs::World::GetSystem() const
{
	const core::TypeId& typeId = core::ToTypeId<TSystem>();

	const auto first = m_SystemEntries.begin();
	const auto last = m_SystemEntries.end();
	auto result = std::find_if(first, last, [&](const SystemEntry& entry)
	{
		return entry.m_TypeId == typeId;
	});

	assert(result != last);
	return *dynamic_cast<TSystem*>(result->m_System);
}

template<class TComponent>
void ecs::World::RegisterComponent()
{
	static_assert(std::is_assignable<TComponent, TComponent>::value, "Component Type isn't assignable!");

	ComponentEntry entry =
	{
		core::ToTypeId<TComponent>()
	};
	m_ComponentEntries.push_back(entry);
}

template<class TSystem, typename... TArgs>
void ecs::World::RegisterSystem(TArgs&&... args)
{
	SystemEntry entry =
	{
		core::ToTypeId<TSystem>()
		, new TSystem(std::forward<TArgs>(args)...)
	};

	entry.m_System->m_World = this;
	m_SystemEntries.push_back(entry);
}