#pragma once

#include <rapidjson/document.h>

#include <array>
#include <optional>
#include <variant>

namespace serialize
{
	class Reader final
	{
		using Document = rapidjson::Document;

	public:

		Reader(const char* data);

		template<typename Type>
		void Visit(Type& value);

		template<typename Key, typename Val>
		void Visit(Map<Key, Val>& value);
		template<typename Type>
		void Visit(Set<Type>& value);
		template<typename Type>
		void Visit(Array<Type>& value);

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
		Document m_Document = { };
		int32 m_Index = 0;
	};
}

#include "Reader.inl"