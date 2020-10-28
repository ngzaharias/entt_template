#pragma once

#include "CircularBuffer.hpp"

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <queue>
#include <vector>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

namespace sf
{
	class Time;
}

namespace debug
{
	struct ComponentInfo
	{
		std::string Name;
		entt::id_type TypeId;
	};

	struct ComponentSettings
	{
		std::set<entt::id_type> FilterTypes;
		std::string FilterText = "";
		bool IsAutoRefreshEnabled = false;
		bool IsRefreshRequested = true;
	};

	struct EntityInfo
	{
		std::string Name;
		entt::entity Entity;
	};

	struct EntitySettings
	{
		std::string FilterText = "";
		bool IsAutoRefreshEnabled = true;
		bool IsRefreshRequested = false;
		bool IsShowingOrphans = false;
	};

	struct Selection
	{
		static constexpr int Capacity = 10;
		enum class Action { Select, Undo, Redo };
		using TRequest = std::optional<std::pair<Action, entt::entity>>;
		using THistory = CircularBuffer<entt::entity, Capacity>;

		entt::entity Current = entt::null;
		TRequest Request;
		THistory Undos = { };
		THistory Redos = { };
	};

	class EnttDebugger
	{
		using WidgetCallback = std::function<void(entt::registry&, entt::entity&)>;

	public:
		EnttDebugger();
		~EnttDebugger();

		void Initialise(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);
		void Render(entt::registry& registry);

		void Select(const entt::entity& entity);
		void Undo();
		void Redo();

	private:
		template <class Component>
		void RegisterWidget(WidgetCallback&& callback);

		// main sections
		void RenderComponents(entt::registry& registry);
		void RenderEntities(entt::registry& registry);
		void RenderSelected(entt::registry& registry);

		// sub sections
		void RenderUndoRedo(entt::registry& registry);

	private:
		ComponentSettings m_ComponentSettings;
		std::vector<ComponentInfo> m_ComponentInfo;
		std::map<entt::id_type, WidgetCallback> m_ComponentWidgets;

		EntitySettings m_EntitySettings;
		std::vector<EntityInfo> m_EntityInfo;
		std::set<entt::entity> m_EntityOrphans;

		Selection m_Selection;

		bool m_IsWindowVisible;
	};
}

#include "EnttDebugger.inl"