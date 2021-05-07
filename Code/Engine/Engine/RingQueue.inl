#pragma once

#include <utility>

template<class Type, int32 t_Capacity>
template <typename ... Types>
constexpr RingQueue<Type, t_Capacity>::RingQueue(Types&&... values)
	: m_Array{ std::forward<Types>(values)... }
	, m_Count(sizeof...(Types))
	, m_Head(0)
	, m_Tail(m_Count - 1)
{
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::operator[](const int32 index) -> Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::operator[](const int32 index) const -> const Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int32 t_Capacity>
constexpr bool RingQueue<Type, t_Capacity>::IsEmpty() const
{
	return m_Count == 0;
}

template<class Type, int32 t_Capacity>
constexpr bool RingQueue<Type, t_Capacity>::IsFull() const
{
	return m_Count == t_Capacity;
}

template<class Type, int32 t_Capacity>
constexpr int32 RingQueue<Type, t_Capacity>::GetCapacity() const
{
	return t_Capacity;
}

template<class Type, int32 t_Capacity>
constexpr int32 RingQueue<Type, t_Capacity>::GetCount() const
{
	return m_Count;
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::Get(const int32 index) -> Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::Get(const int32 index) const -> const Type&
{
	return m_Array[(m_Head + index) % t_Capacity];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::GetFirst() -> Type&
{
	return m_Array[m_Head];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::GetFirst() const -> const Type&
{
	return m_Array[m_Head];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::GetLast() -> Type&
{
	return m_Array[m_Tail];
}

template<class Type, int32 t_Capacity>
constexpr auto RingQueue<Type, t_Capacity>::GetLast() const -> const Type&
{
	return m_Array[m_Tail];
}

template<class Type, int32 t_Capacity>
void RingQueue<Type, t_Capacity>::RemoveAll()
{
	m_Count = 0;
	m_Head = 0;
	m_Tail = -1;
}

template<class Type, int32 t_Capacity>
auto RingQueue<Type, t_Capacity>::Pop()->Type
{
	assert(m_Count > 0);
	const Type& value = m_Array[m_Tail];
	m_Tail = (--m_Tail + t_Capacity) % t_Capacity;
	m_Count--;
	return value;
}

template<class Type, int32 t_Capacity>
auto RingQueue<Type, t_Capacity>::Push(const Type& value)->Type&
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
