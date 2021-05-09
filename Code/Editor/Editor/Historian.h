#pragma once

#include <Engine/JsonTypes.h>
#include <Engine/RingQueue.h>
#include <Engine/System.h>
#include <Engine/TypeList.h>

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace editor
{
	struct Transaction;

	struct Record
	{
		Record() = default;
		Record(const Record& rhs);
		void operator=(const Record& rhs) noexcept;

		entt::entity m_Entity = entt::null;
		json::Document m_Document = { };
	};

	class Historian final : public ecs::System
	{
		using Database = Map<entt::entity, editor::Record>;
		using Records = RingQueue<Record, 50>;
		using Transactions = Array<Transaction>;

	public:
		Historian();
		~Historian();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		void PushRecord(const Record& recordOld, const Record& recordNew);
		void RedoRecord(entt::registry& registry);
		void UndoRecord(entt::registry& registry);

		template <typename ...Types>
		void CopyToEntity(const editor::Record& record, entt::registry& registry, core::TypeList<Types...> types);

		template <typename ...Types>
		void CopyToRecord(const entt::registry& registry, editor::Record& record, core::TypeList<Types...> types);
		
		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	public:
		entt::sigh<void()> m_OnRedoRecord = { };
		entt::sigh<void()> m_OnUndoRecord = { };

	private:
		void Render();

	private:
		Database m_Pending = { };
		Records m_UndoRecords = { };
		Records m_RedoRecords = { };

		bool m_IsVisible = true;
	};
}

#include "Editor/Historian.inl"