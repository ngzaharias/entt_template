#include "Application.hpp"
#include "GameApplication.hpp"

#include <direct.h>
#include <iostream>
#include <entt/entt.hpp>
#include <PhysX/PxPhysicsAPI.h>

Application* application = nullptr;

namespace physx
{
	class UserAllocatorCallback final : public physx::PxAllocatorCallback
	{
	public:
		void* allocate(size_t size, const char*, const char*, int) override
		{
			void* ptr = platformAlignedAlloc(size);
			PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
			return ptr;
		}

		void deallocate(void* ptr) override
		{
			platformAlignedFree(ptr);
		}
	};

	class UserErrorCallback final : public physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			printf("%d %s %s %d", code, message, file, line);
		}
	};
}

int main(int agrc, char* argv[])
{
	_chdir("../../");

	static physx::PxDefaultAllocator gAllocator;
	static physx::UserErrorCallback gError;
	physx::PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gError);

	application = new GameApplication();
	application->Execute(agrc, argv);
	delete(application);
	application = nullptr;

	foundation->release();

	return 0;
}