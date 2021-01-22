#include "Engine/EnginePCH.h"
#include "Engine/FlipbookSystem.h"

#include "Engine/FlipbookComponent.h"

#include <entt/entt.hpp>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.0000001f;
}

render::FlipbookSystem::FlipbookSystem()
{
}

render::FlipbookSystem::~FlipbookSystem()
{
}

void render::FlipbookSystem::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	const auto view = registry.view<render::FlipbookComponent>();
	for (const entt::entity& renderEntity : view)
	{
		auto& flipbookComponent = view.get<render::FlipbookComponent>(renderEntity);

		const render::FlipbookAsset& flipbookAsset = flipbookComponent.m_Flipbook.get();
		if (flipbookAsset.m_Frames.empty())
			continue;

		const int32 indexCount = static_cast<int32>(flipbookAsset.m_Frames.size());

		const float timeOld = flipbookComponent.m_Time;
		const float timeNew = flipbookComponent.m_Time + gameTime.asSeconds();
		const float timeMax = indexCount / flipbookAsset.m_FPS;

		if (timeNew < timeMax)
		{
			flipbookComponent.m_Time = timeNew;
		}
		else if (flipbookAsset.m_IsLooping)
		{
			flipbookComponent.m_Time = timeNew - timeMax;
		}
		else
		{
			flipbookComponent.m_Time = timeMax;
		}

		// #note: we subtract the epsilon because otherwise it will wrap back to 0 when time == timeMax
		flipbookComponent.m_Time = std::clamp(flipbookComponent.m_Time, 0.f, timeMax - s_TimeMaxEpsilon);
		flipbookComponent.m_Index = static_cast<int32>(flipbookComponent.m_Time * flipbookAsset.m_FPS);
		flipbookComponent.m_Index %= indexCount;
	}
}
