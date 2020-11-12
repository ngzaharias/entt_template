#pragma once

#include <Engine/Types.h>

#include <assert.h>

template<class Type, int32 t_Capacity>
class CircularBuffer final
{
	static_assert(t_Capacity > 0, "Invalid capacity.");

public:
	constexpr CircularBuffer() = default;

	template <typename... Types>
	constexpr CircularBuffer(Types&&... values);

	constexpr auto operator[](const int index)->Type&;
	constexpr auto operator[](const int index) const -> const Type&;

	constexpr auto IsEmpty() const -> bool;
	constexpr auto IsFull() const -> bool;

	constexpr auto GetCapacity() const->int32;
	constexpr auto GetCount() const->int32;

	constexpr auto Get(const int index)->Type&;
	constexpr auto Get(const int index) const -> const Type&;

	constexpr auto GetFirst()->Type&;
	constexpr auto GetFirst() const -> const Type&;

	constexpr auto GetLast()->Type&;
	constexpr auto GetLast() const -> const Type&;

	void RemoveAll();
	auto Pop()->Type;
	auto Push(const Type& value)->Type&;

private:
	Type m_Array[t_Capacity] = { };
	int32 m_Count = 0;
	int32 m_Head = 0;
	int32 m_Tail = -1;
};

#include "CircularBuffer.inl"