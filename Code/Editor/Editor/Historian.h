#pragma once

#include <Engine/RingQueue.h>
#include <Engine/System.h>
#include <Engine/TypeList.h>

#include <map>
#include <vector>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <rapidjson/document.h>

namespace editor
{
	struct Transaction;

	struct Record
	{
		using Entity = entt::entity;
		using Document = rapidjson::Document;

		Record() = default;
		Record(const Record& rhs);
		void operator=(const Record& rhs) noexcept;

		Entity	 m_Entity = entt::null;
		Document m_Document = { };
	};

	class Historian final : public core::System
	{
		using Database = std::map<entt::entity, editor::Record>;
		using Records = RingQueue<Record, 50>;
		using Transactions = std::vector<Transaction>;

	public:
		Historian();
		~Historian();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

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
		void Render(entt::registry& registry);

	private:
		Database m_Pending = { };
		Records m_UndoRecords = { };
		Records m_RedoRecords = { };

		bool m_IsVisible = true;
	};
}

#include "Editor/Historian.inl"