#pragma once

#include <rapidjson/document.h>

namespace serialize
{
	class Reader final
	{
		using Document = rapidjson::Document;
		using Value = rapidjson::Value;

	public:
		explicit Reader(const char* data);

		template<typename Type>
		void Visit(Type& value);

		template<typename Type>
		void Visit(std::vector<Type>& value);

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
		// read
		Document	m_Document = { };
		Value		m_Data = { };
		int32		m_Index = 0;
	};
}

#include "Reader.inl"