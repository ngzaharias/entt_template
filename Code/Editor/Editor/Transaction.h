#pragma once

#include <Editor/Address.h>

#include <Engine/JsonTypes.h>

namespace editor
{
	enum class ETransactionType
	{
		Unknown,

		Create,
		Delete,
		Edit,

		Translate,
		Rotate,
		Scale,
	};

	struct Transaction
	{
		template<typename Type>
		Transaction(const editor::Address& address, const Type& value)
			: Transaction(address.GetFull(), value)
		{
		}

		template<typename Type>
		Transaction(const char* address, const Type& value)
		{
			//static_assert(false, "Unsupported Type!");
		}

		Transaction(const char* address, const bool& value);
		Transaction(const char* address, const float& value);
		Transaction(const char* address, const int32& value);
		Transaction(const char* address, const uint32& value);
		Transaction(const char* address, const Vector2f& value);
		Transaction(const char* address, const Vector2i& value);
		Transaction(const char* address, const Vector2u& value);
		Transaction(const char* address, const Vector3f& value);
		Transaction(const char* address, const Vector3i& value);
		Transaction(const char* address, const str::String& value);
		Transaction(const char* address, const json::Object& value);

		void ApplyTo(json::Document& document) const;

	public:
		str::String		 m_Address = { };
		json::Document	 m_Document = { };
		ETransactionType m_Type = ETransactionType::Unknown;
	};
}