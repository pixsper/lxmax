// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

// Adapted from Serial Communications Library - https://github.com/wjwwood/serial
// Copyright (c) 2012 William Woodall, John Harrison

#include "lx_serial.h"

#ifdef WIN_VERSION

#include <tchar.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>

#define LX_SERIAL_PORT_NAME_MAX_LENGTH 256
#define LX_SERIAL_FRIENDLY_NAME_MAX_LENGTH 256
#define LX_SERIAL_HARDWARE_ID_MAX_LENGTH 256

t_hashtab* lx_serial_port_enumerate()
{
	t_hashtab* devices_found = hashtab_new(0);
	hashtab_flags(devices_found, OBJ_FLAG_DATA);

	const HDEVINFO device_info_set = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);

	unsigned int device_info_set_index = 0;
	SP_DEVINFO_DATA device_info_data;

	device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

	while(SetupDiEnumDeviceInfo(device_info_set, device_info_set_index, &device_info_data))
	{
		++device_info_set_index;

		// Get port name

		HKEY key = SetupDiOpenDevRegKey(device_info_set, &device_info_data, DICS_FLAG_GLOBAL,
			0, DIREG_DEV, KEY_READ);

		TCHAR port_name[LX_SERIAL_PORT_NAME_MAX_LENGTH];
		DWORD port_name_length = LX_SERIAL_PORT_NAME_MAX_LENGTH;

		const LONG return_code = RegQueryValueEx(key, _T("PortName"), NULL, NULL, 
		                                         (LPBYTE)port_name, &port_name_length);

		RegCloseKey(key);

		if (return_code != EXIT_SUCCESS)
			continue;

		if (port_name_length > 0 && port_name_length <= LX_SERIAL_PORT_NAME_MAX_LENGTH)
			port_name[port_name_length-1] = '\0';
		else
			port_name[0] = '\0';

		// Ignore parallel ports

		if (_tcsstr(port_name, _T("LPT")) != NULL)
			continue;

		// Get port friendly name

		TCHAR friendly_name[LX_SERIAL_FRIENDLY_NAME_MAX_LENGTH];
		DWORD friendly_name_length = 0;

		const bool got_friendly_name = SetupDiGetDeviceRegistryProperty(
					device_info_set,
					&device_info_data,
					SPDRP_FRIENDLYNAME,
					NULL,
					(PBYTE)friendly_name,
			LX_SERIAL_FRIENDLY_NAME_MAX_LENGTH,
					&friendly_name_length);

		if (got_friendly_name == true && friendly_name_length > 0)
			friendly_name[friendly_name_length - 1] = '\0';
		else
			friendly_name[0] = '\0';

		// Get hardware ID

		TCHAR hardware_id[LX_SERIAL_HARDWARE_ID_MAX_LENGTH];
		DWORD hardware_id_length = 0;

		const bool got_hardware_id = SetupDiGetDeviceRegistryProperty(
					device_info_set,
					&device_info_data,
					SPDRP_HARDWAREID,
					NULL,
					(PBYTE)hardware_id,
			LX_SERIAL_HARDWARE_ID_MAX_LENGTH,
					&hardware_id_length);

		if (got_hardware_id == true && hardware_id_length > 0)
			hardware_id[hardware_id_length-1] = '\0';
		else
			hardware_id[0] = '\0';


		t_lx_serial_port* port_entry = (t_lx_serial_port*)sysmem_newptr(sizeof(t_lx_serial_port));
		port_entry->port = gensym(port_name);
		port_entry->description = gensym(friendly_name);
		port_entry->hardware_id = gensym(hardware_id);

		hashtab_store(devices_found, port_entry->port, (t_object*)port_entry);
	}

	SetupDiDestroyDeviceInfoList(device_info_set);

	return devices_found;
}

#endif