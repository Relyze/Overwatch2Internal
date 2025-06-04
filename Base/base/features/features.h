#pragma once
#include "aim/aim.h"
#include "visuals/visuals.h"
#include "misc/misc.h"

namespace features
{
	static bool wait_for_unhold = false;
	static bool wait_for_unhold_trigger = false;

	extern void doVisuals();
	extern void doAim();
	extern void doMisc();
}
