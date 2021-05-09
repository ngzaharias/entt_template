#pragma once

#include <Engine/JsonTypes.h>

namespace serialize
{
	class Reader final
	{
	public:
		Reader(const char* data);

		template<typename Type>
		void Visit(Type& value);

		template<typename Type>
		void Visit(Array<Type>& value);
		template<typename Key, typename Val>
		void Visit(Map<Key, Val>& value);
		template<typename Type>
		void Visit(Set<Type>& value);

		template<typename Type>
		void Visit(Nullable<Type>& value);
		template<typename ...Types>
		void Visit(Variant<Types...>& value);

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
		json::Document m_Document = { };
		int32 m_Index = 0;
	};
}

#include "Reader.inl"