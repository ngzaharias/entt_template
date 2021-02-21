#pragma once

#include <Engine/Types.h>

#include <queue>
#include <set>

template<class Type, int32 t_Capacity>
class ObjectPool
{
	static_assert(t_Capacity > 0, "Invalid capacity.");

public:
	ObjectPool();
	~ObjectPool();

	void Grow(int32 amount);

	auto RequestObject()->Type*;
	void ReleaseObject(Type* object);

private:
	std::set<Type*> m_UsedObjects;
	std::queue<Type*> m_FreeObjects;
	uint32 m_Capacity = 0;
};

#include "ObjectPool.inl"