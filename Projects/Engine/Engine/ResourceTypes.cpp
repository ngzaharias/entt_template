#include "Engine/ResourceTypes.h"

#include "Engine/StringHelpers.h"
#include "Engine/StringView.h"

core::EResourceType core::ToResourceType(const char* string)
{
	if (str::Equals(string, "physics_material"))
		return core::EResourceType::PhysicsMaterial;
	if (str::Equals(string, "sound"))
		return core::EResourceType::Sound;
	if (str::Equals(string, "texture"))
		return core::EResourceType::Texture;
	return core::EResourceType::Unknown;
}

const char* core::ToResourceType(core::EResourceType resourceType)
{
	switch (resourceType)
	{
	case core::EResourceType::PhysicsMaterial:
		return "physics_material";
	case core::EResourceType::Sound:
		return "sound";
	case core::EResourceType::Texture:
		return "texture";
	}
	return "unknown";
}
