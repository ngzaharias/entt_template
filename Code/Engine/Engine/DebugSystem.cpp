#include "EnginePCH.h"
#include "Engine/DebugSystem.h"

#include "Engine/ColourHelpers.h"
#include "Engine/Screen.h"

#include <entt/entt.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

namespace
{
	const Vector3f s_Red = Vector3f(255.f, 0.f, 0.f);
	const Vector3f s_Green = Vector3f(0.f, 255.f, 0.f);
}

debug::DebugSystem::DebugSystem(sf::RenderTarget& renderTarget)
	: m_RenderTarget(renderTarget)
{
}

debug::DebugSystem::~DebugSystem()
{
}

void debug::DebugSystem::Initialise()
{
	m_Font.loadFromFile("C:/Personal/ZEngine/AssetsRaw/Fonts/F_Kenpixel.ttf");
}

void debug::DebugSystem::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// fps
	{
		m_FPSCounter.AddFrame(gameTime.asSeconds());
		str::String string = "FPS: " + std::to_string(m_FPSCounter.GetAverage());

		const float fpsValue = static_cast<float>(m_FPSCounter.GetAverage());
		float fpsAlpha = math::Remap(fpsValue, 12.f, 60.f, 0.f, 1.f);
		fpsAlpha = math::Clamp(fpsAlpha, 0.f, 1.f);
		const Vector3f colourf = colour::LerpRGB(s_Red, s_Green, fpsAlpha);
		const ColourRGB colour = { 
			static_cast<uint8>(colourf.x),
			static_cast<uint8>(colourf.y),
			static_cast<uint8>(colourf.z) };

		sf::Text text;
		text.setFillColor(colour);
		text.setFont(m_Font);
		text.setPosition(sf::Vector2f(Screen::width * -0.5f, Screen::height * 0.5f));
		text.setScale(sf::Vector2f(1.f, -1.f) * 0.5f);
		text.setString(string);
		m_RenderTarget.draw(text);
	}
}
