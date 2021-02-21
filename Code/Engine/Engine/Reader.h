#pragma once

#include <rapidjson/document.h>

#include <array>
#include <map>
#include <optional>
#include <set>
#include <variant>
#include <vector>

namespace serialize
{
	class Reader final
	{
		using Document = rapidjson::Document;

	public:
		enum class Mode
		{
			None,
			Replication,
		};

		explicit Reader(const char* data);
		explicit Reader(const char* data, Mode mode);

		template<typename Type>
		void Visit(Type& value);

		template<typename Type, size_t Size>
		void Visit(std::array<Type, Size>& value);
		template<typename Key, typename Val>
		void Visit(std::map<Key, Val>& value);
		template<typename Type>
		void Visit(std::set<Type>& value);
		template<typename Type>
		void Visit(std::vector<Type>& value);

		template<typename Type>
		void Visit(std::optional<Type>& value);
		template<typename ...Types>
		void Visit(std::variant<Types...>& value);

		void Visit(bool& value);
		void Visit(int32& value);
		void Visit(uint32& value);
		void Visit(double& value);
		void Visit(float& value);
		void Visit(str::Name& value);
		void Visit(str::String& value);
		void Visit(Vector2f& value);
		void Visit(Vector2i& value);
		void Visit(Vector2u& value);
		void Visit(Vector3f& value);
		void Visit(Vector3i& value);

	private:
		const Mode m_Mode = Mode::None;

		Document m_Document = { };
		int32 m_Index = 0;
	};
}

#include "Reader.inl"