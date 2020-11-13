#pragma once

#include <Engine/Application.h>

namespace game
{
	class Application final : public core::Application
	{
	public:
		Application();
		~Application();

	private:
		void Register() override;
		bool Initialise() override;
		bool Update(const sf::Time& time) override;
		void Destroy() override;
	};
}
