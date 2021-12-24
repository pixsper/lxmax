// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

#include "lx_common.h"
#include "lx_net.h"
#include "lx_universe_config.h"

#ifdef WIN_VERSION

#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
		default:
			break;

		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(instance);

#elif MAC_VERSION

__attribute__((constructor)) void DllMain()
{

#endif

            common_symbols_init();
            lx_common_symbols_init();
            lx_network_adapter_register_class();
            lx_universe_config_register_class();

#ifdef WIN_VERSION

	}
	return TRUE;
}

#elif MAC_VERSION

}

#endif
