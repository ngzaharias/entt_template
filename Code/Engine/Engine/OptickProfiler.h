#pragma once

#ifdef USE_OPTICK

#include <optick.h>

#define PROFILE_FUNCTION() OPTICK_EVENT()
#define PROFILE_TICK(name) OPTICK_FRAME(name)

#else

#define PROFILE_FUNCTION()

#endif