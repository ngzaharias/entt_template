#include "EnginePCH.h"
#include "Engine/FlipbookSystem.h"

#include "Engine/FlipbookComponent.h"

#include <entt/entt.hpp>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;
}

render::FlipbookSystem::FlipbookSystem()
{
}

render::FlipbookSystem::~FlipbookSystem()
{
}

void render::FlipbookSystem::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	const auto view = registry.view<render::FlipbookComponent>();
	for (const ecs::Entity& renderEntity : view)
	{
		auto& flipbookComponent = view.get<render::FlipbookComponent>(renderEntity);
		if (!flipbookComponent.m_Flipbook)
			continue;

		if (!flipbookComponent.m_IsPlaying)
			continue;

		const render::FlipbookAsset& flipbookAsset = flipbookComponent.m_Flipbook.get();
		if (flipbookAsset.m_Frames.empty())
			continue;

		const int32 indexCount = static_cast<int32>(flipbookAsset.m_Frames.size());
		const int32 indexMax = indexCount - 1;

		const float timeOld = flipbookComponent.m_Time;
		const float timeNew = flipbookComponent.m_Time + gameTime.asSeconds();
		const float timeMax = indexMax / flipbookComponent.m_FPS;

		if (timeNew < timeMax)
		{
			flipbookComponent.m_Time = timeNew;
		}
		else if (flipbookComponent.m_IsLooping)
		{
			flipbookComponent.m_Time = timeNew - timeMax;
		}
		else
		{
			flipbookComponent.m_Time = timeMax;
		}

		// #note: we subtract the epsilon because otherwise it will wrap back to 0 when time == timeMax
		flipbookComponent.m_Time = std::clamp(flipbookComponent.m_Time, 0.f, timeMax - s_TimeMaxEpsilon);
		flipbookComponent.m_Index = static_cast<int32>(flipbookComponent.m_Time * flipbookComponent.m_FPS);
		flipbookComponent.m_Index %= indexMax;
	}
}
