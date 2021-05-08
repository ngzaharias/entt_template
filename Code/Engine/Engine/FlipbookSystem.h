#pragma once

#include <Engine/System.h>

namespace render
{
	class FlipbookSystem : public ecs::System
	{
	public:
		FlipbookSystem();
		~FlipbookSystem();

		void Update(const core::GameTime& gameTime) override;
	};
};