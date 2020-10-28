#pragma once

template<class Type, int32 t_Capacity>
ObjectPool<Type, t_Capacity>::ObjectPool()
	: m_FreeObjects()
	, m_UsedObjects()
	, m_Capacity(0)
{
	Grow(t_Capacity);
}

template<class Type, int32 t_Capacity>
ObjectPool<Type, t_Capacity>::~ObjectPool()
{
	while (!m_FreeObjects.empty())
	{
		delete m_FreeObjects.front();
		m_FreeObjects.pop();
	}

	for (Type* object : m_UsedObjects)
		delete object;
}

template<class Type, int32 t_Capacity>
void ObjectPool<Type, t_Capacity>::Grow(int32 amount)
{
	m_Capacity += amount;
	for (int32 i = 0; i < amount; ++i)
		m_FreeObjects.push(new Type());
}

template<class Type, int32 t_Capacity>
auto ObjectPool<Type, t_Capacity>::RequestObject() -> Type*
{
#ifndef NDEBUG
	constexpr int32 warningSize = t_Capacity * 10;
	if (m_Capacity + t_Capacity >= warningSize)
		printf("ObjectPool %s has increased 10x its starting size\n", typeid(Type).name());
	if (m_FreeObjects.empty())
		printf("ObjectPool %s has increased its size from %d -> %d\n", typeid(Type).name(), m_Capacity, m_Capacity + t_Capacity);
#endif

	if (m_FreeObjects.empty())
		Grow(t_Capacity);

	Type* object = m_FreeObjects.front();
	m_FreeObjects.pop();
	m_UsedObjects.insert(object);
	return object;
}

template<class Type, int32 t_Capacity>
void ObjectPool<Type, t_Capacity>::ReleaseObject(Type* object)
{
	auto itr = m_UsedObjects.find(object);
	m_UsedObjects.erase(itr);
	m_FreeObjects.push(object);
}