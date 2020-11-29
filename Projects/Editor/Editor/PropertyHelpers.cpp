#include "Editor/PropertyHelpers.h"

#include <Engine/StringTable.h>

const char* editor::PropertyName(const entt::meta_data& metaData, const char* _default /*= nullptr*/)
{
	if (!metaData)
		return _default;

	const entt::meta_prop& metaProp = metaData.prop(core::strName);
	if (!metaProp)
		return _default;

	return metaProp.value().cast<const char*>();
}

const char* editor::PropertyName(const entt::meta_type& metaType, const char* _default /*= nullptr*/)
{
	if (!metaType)
		return _default;

	const entt::meta_prop& metaProp = metaType.prop(core::strName);
	if (!metaProp)
		return _default;

	return metaProp.value().cast<const char*>();
}


size_t editor::PropertyOffset(const entt::meta_data& metaData, const size_t _default /*= 0*/)
{
	if (!metaData)
		return _default;

	const entt::meta_prop& metaProp = metaData.prop(core::strOffset);
	if (!metaProp)
		return _default;

	return metaProp.value().cast<size_t>();
}

size_t editor::PropertyStride(const entt::meta_data& metaData, const size_t _default /*= 0*/)
{
	if (!metaData)
		return _default;

	const entt::meta_prop& metaProp = metaData.prop(core::strStride);
	if (!metaProp)
		return _default;

	return metaProp.value().cast<size_t>();
}
