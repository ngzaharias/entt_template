#pragma once

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <array>
#include <variant>

namespace serialize
{
	class Writer final
	{
		using PrettyWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;
		using StringBuffer = rapidjson::StringBuffer;

	public:
		Writer();

		str::StringView Conclude();

		template<typename Type>
		void Visit(const Type& value);

		template<typename Type>
		void Visit(const Array<Type>& value);
		template<typename Key, typename Val>
		void Visit(const Map<Key, Val>& value);
		template<typename Type>
		void Visit(const Set<Type>& value);

		template<typename Type>
		void Visit(const std::optional<Type>& value);
		template<typename ...Types>
		void Visit(const std::variant<Types...>& value);

		void Visit(const bool& value);
		void Visit(const int32& value);
		void Visit(const uint32& value);
		void Visit(const double& value);
		void Visit(const float& value);
		void Visit(const str::Name& value);
		void Visit(const str::String& value);
		void Visit(const Vector2f& value);
		void Visit(const Vector2i& value);
		void Visit(const Vector2u& value);
		void Visit(const Vector3f& value);
		void Visit(const Vector3i& value);

	private:
		PrettyWriter m_Writer;
		StringBuffer m_Buffer = { };
	};
}

#include "Writer.inl"