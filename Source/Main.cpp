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
	static physx::PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gError);

	bool recordMemoryAllocations = true;

	physx::PxPvd* visualDebugger = physx::PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	visualDebugger->connect(*transport, physx::PxPvdInstrumentationFlag::ePROFILE);

	physx::PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, visualDebugger);

	application = new GameApplication();
	application->Execute(agrc, argv);
	delete(application);
	application = nullptr;

	physics->release();
	visualDebugger->disconnect();

	transport->release();
	visualDebugger->release();
	gFoundation->release();

	return 0;
}