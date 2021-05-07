#pragma once

template<class Type>
struct Iterator final
{
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Type;
	using pointer = Type*;
	using reference = Type&;

	Iterator(Type* pointer) : m_Pointer(pointer) {}

	constexpr Type& operator*() const { return *m_Pointer; }
	constexpr Type* operator->() { return m_Pointer; }

	constexpr auto operator++() -> Iterator& { m_Pointer++; return *this; }
	constexpr auto operator++(int) -> Iterator { Iterator tmp = *this; ++(*this); return tmp; }

	constexpr auto operator--() -> Iterator& { m_Pointer--; return *this; }
	constexpr auto operator--(int) -> Iterator { Iterator tmp = *this; --(*this); return tmp; }

	constexpr bool operator== (const Iterator& rhs) { return m_Pointer == rhs.m_Pointer; }
	constexpr bool operator!= (const Iterator& rhs) { return m_Pointer != rhs.m_Pointer; }

private:
	Type* m_Pointer;
};
