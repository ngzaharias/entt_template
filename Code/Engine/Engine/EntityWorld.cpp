#include "EnginePCH.h"
#include "Engine/EntityWorld.h"

#include "Engine/System.h"

void ecs::EntityWorld::Initialise()
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry& entry : m_SystemEntries)
		entry.m_System->Initialise();
}

void ecs::EntityWorld::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry& entry : m_SystemEntries)
		entry.m_System->Update(gameTime);
}

void ecs::EntityWorld::Destroy()
{
	PROFILE_FUNCTION();

	m_ComponentEntries.clear();
	for (ecs::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Destroy();
		delete entry.m_System;
	}
	m_SystemEntries.clear();
}