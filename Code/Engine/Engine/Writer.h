#pragma once

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <array>
#include <map>
#include <set>
#include <variant>
#include <vector>

namespace serialize
{
	class Writer final
	{
		using PrettyWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;
		using StringBuffer = rapidjson::StringBuffer;

	public:
		enum class Mode
		{
			None,
			Replication,
		};

		explicit Writer();
		explicit Writer(Mode mode);

		str::StringView Conclude();

		template<typename Type>
		void Visit(const Type& value);

		template<typename Type, size_t Size>
		void Visit(const std::array<Type, Size>& value);
		template<typename Key, typename Val>
		void Visit(const std::map<Key, Val>& value);
		template<typename Type>
		void Visit(const std::set<Type>& value);
		template<typename Type>
		void Visit(const std::vector<Type>& value);

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
		const Mode m_Mode = Mode::None;

		PrettyWriter m_Writer;
		StringBuffer m_Buffer = { };
	};
}

#include "Writer.inl"