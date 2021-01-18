#pragma once

#include <Engine/Application.h>

namespace editor
{
	class Application final : public core::Application
	{
	public:
		Application();
		~Application();

	private:
		void Register() override;
		bool Initialise() override;
		bool Update(const core::GameTime& gameTime) override;
		void Destroy() override;

	private:
	};
}
