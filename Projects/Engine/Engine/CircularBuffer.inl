#pragma once

#include <utility>

template<class Type, int t_Capacity>
template <typename ... Types>
constexpr CircularBuffer<Type, t_Capacity>::CircularBuffer(Types&&... values)
	: m_Array{ std::forward<Types>(values)... }
	, m_Count(sizeof...(Types))
	, m_Head(0)
	, m_Tail(m_Count - 1)
{
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::operator[](const int index) -> Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::operator[](const int index) const -> const Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int t_Capacity>
constexpr bool CircularBuffer<Type, t_Capacity>::IsEmpty() const
{
	return m_Count == 0;
}

template<class Type, int t_Capacity>
constexpr bool CircularBuffer<Type, t_Capacity>::IsFull() const
{
	return m_Count == t_Capacity;
}

template<class Type, int t_Capacity>
constexpr int CircularBuffer<Type, t_Capacity>::GetCapacity() const
{
	return t_Capacity;
}

template<class Type, int t_Capacity>
constexpr int CircularBuffer<Type, t_Capacity>::GetCount() const
{
	return m_Count;
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::Get(const int index) -> Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::Get(const int index) const -> const Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::GetFirst() -> Type&
{
	return m_Array[m_Head];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::GetFirst() const -> const Type&
{
	return m_Array[m_Head];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::GetLast() -> Type&
{
	return m_Array[m_Tail];
}

template<class Type, int t_Capacity>
constexpr auto CircularBuffer<Type, t_Capacity>::GetLast() const -> const Type&
{
	return m_Array[m_Tail];
}

template<class Type, int t_Capacity>
void CircularBuffer<Type, t_Capacity>::RemoveAll()
{
	m_Count = 0;
	m_Head = 0;
	m_Tail = -1;
}

template<class Type, int t_Capacity>
auto CircularBuffer<Type, t_Capacity>::Pop()->Type
{
	assert(m_Count > 0);
	const Type& value = m_Array[m_Tail];
	m_Tail = (--m_Tail + t_Capacity) % t_Capacity;
	m_Count--;
	return value;
}

template<class Type, int t_Capacity>
auto CircularBuffer<Type, t_Capacity>::Push(const Type& value)->Type&
{
	m_Tail = ++m_Tail % t_Capacity;
	m_Array[m_Tail] = value;
	if (++m_Count > t_Capacity)
	{
		m_Count = t_Capacity;
		m_Head = ++m_Head % t_Capacity;
	}
	return m_Array[m_Tail];
}
