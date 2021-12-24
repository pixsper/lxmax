// Copyright (c) 2023 Pixsper Ltd. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.

// Adapted from Serial Communications Library - https://github.com/wjwwood/serial
// Copyright (c) 2012 William Woodall, John Harrison

#include "lx_serial.h"

#ifdef MAC_VERSION

#include <sys/param.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>

#include <cstdint>
#include <string>

#define HARDWARE_ID_STRING_LENGTH 128

std::string cfstring_to_string(CFStringRef cf_string);
std::string get_device_path(io_object_t& serial_port);
std::string get_class_name(io_object_t& obj);
io_registry_entry_t get_parent_iousb_device(io_object_t& serial_port);
std::string get_string_property(io_object_t& device, const char* property);
uint16_t get_int_property(io_object_t& device, const char* property);
std::string rtrim(const std::string& str);

std::string cfstring_to_string(CFStringRef cf_string)
{
    char cstring[MAXPATHLEN];
    std::string result;

    if (cf_string)
    {
        Boolean success = CFStringGetCString(cf_string, cstring, sizeof(cstring), kCFStringEncodingASCII);
        if (success)
            result = cstring;
    }

    return result;
}

std::string get_device_path(io_object_t& serial_port)
{
    CFTypeRef callout_path;
    std::string device_path;

    callout_path = IORegistryEntryCreateCFProperty(serial_port, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);

    if (callout_path)
    {
        if (CFGetTypeID(callout_path) == CFStringGetTypeID())
            device_path = cfstring_to_string(static_cast<CFStringRef>(callout_path));

        CFRelease(callout_path);
    }

    return device_path;
}

std::string get_class_name(io_object_t& obj)
{
    std::string result;
    io_name_t class_name;
    kern_return_t kern_result = IOObjectGetClass(obj, class_name);

    if (kern_result == KERN_SUCCESS)
        result = class_name;

    return result;
}

io_registry_entry_t get_parent_iousb_device(io_object_t& serial_port)
{
    io_object_t device = serial_port;
    io_registry_entry_t parent = 0;
    io_registry_entry_t result = 0;
    kern_return_t kern_result = KERN_FAILURE;
    std::string name = get_class_name(device);

    // Walk the IO Registry tree looking for this devices parent IOUSBDevice.
    while (name != "IOUSBDevice")
    {
        kern_result = IORegistryEntryGetParentEntry(device, kIOServicePlane, &parent);

        if (kern_result != KERN_SUCCESS)
        {
            result = 0;
            break;
        }

        device = parent;

        name = get_class_name(device);
    }

    if (kern_result == KERN_SUCCESS)
        result = device;

    return result;
}

std::string get_string_property(io_object_t& device, const char* property)
{
    std::string property_name;

    if (device)
    {
        CFStringRef property_as_cf_string = CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingASCII);
        CFTypeRef name_as_cf_string = IORegistryEntryCreateCFProperty(device, property_as_cf_string, kCFAllocatorDefault, 0);

        if (name_as_cf_string)
        {
            if (CFGetTypeID(name_as_cf_string) == CFStringGetTypeID())
                property_name = cfstring_to_string(static_cast<CFStringRef>(name_as_cf_string));

            CFRelease(name_as_cf_string);
        }

        if (property_as_cf_string)
            CFRelease(property_as_cf_string);
    }

    return property_name;
}

uint16_t get_int_property(io_object_t& device, const char* property)
{
    uint16_t result = 0;

    if (device)
    {
        CFStringRef property_as_cfstring = CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingASCII);
        CFTypeRef number = IORegistryEntryCreateCFProperty(device, property_as_cfstring, kCFAllocatorDefault, 0);

        if (property_as_cfstring)
            CFRelease(property_as_cfstring);

        if (number)
        {
            if (CFGetTypeID(number) == CFNumberGetTypeID())
            {
                bool success = CFNumberGetValue(static_cast<CFNumberRef>(number), kCFNumberSInt16Type, &result);
                if (!success)
                    result = 0;
            }

            CFRelease(number);
        }

    }

    return result;
}

std::string rtrim(const std::string& str)
{
    std::string result = str;
    std::string whitespace = " \t\f\v\n\r";
    std::size_t found = result.find_last_not_of(whitespace);

    if (found != std::string::npos)
        result.erase(found + 1);
    else
        result.clear();

    return result;
}

t_hashtab* lx_serial_port_enumerate()
{
    t_hashtab *devices_found = hashtab_new(0);
    hashtab_flags(devices_found, OBJ_FLAG_DATA);

    CFMutableDictionaryRef classes_to_match;
    io_iterator_t serial_port_iterator;
    io_object_t serial_port;
    mach_port_t master_port;
    kern_return_t kern_result;

    kern_result = IOMasterPort(MACH_PORT_NULL, &master_port);

    if (kern_result != KERN_SUCCESS)
        return devices_found;

    classes_to_match = IOServiceMatching(kIOSerialBSDServiceValue);

    if (classes_to_match == NULL)
        return devices_found;

    CFDictionarySetValue(classes_to_match,
                         CFSTR(kIOSerialBSDTypeKey),
                         CFSTR(kIOSerialBSDAllTypes));

    kern_result = IOServiceGetMatchingServices(master_port, classes_to_match, &serial_port_iterator);

    if (KERN_SUCCESS != kern_result)
        return devices_found;

    while ((serial_port = IOIteratorNext(serial_port_iterator)))
    {
        std::string device_path = get_device_path(serial_port);
        io_registry_entry_t parent = get_parent_iousb_device(serial_port);
        IOObjectRelease(serial_port);

        if (device_path.empty())
            continue;

        t_lx_serial_port *port_info = (t_lx_serial_port *) sysmem_newptr(sizeof(t_lx_serial_port));
        port_info->port = gensym(device_path.c_str());
        port_info->description = _lxsym_not_applicable;
        port_info->hardware_id = _lxsym_not_applicable;

        std::string device_name = rtrim(get_string_property(parent, "USB Product Name"));
        std::string vendor_name = rtrim(get_string_property(parent, "USB Vendor Name"));
        std::string description = rtrim(vendor_name + " " + device_name);
        if (!description.empty())
            port_info->description = gensym(description.c_str());

        std::string serial_number = rtrim(get_string_property(parent, "USB Serial Number"));
        uint16_t vendor_id = get_int_property(parent, "idVendor");
        uint16_t product_id = get_int_property(parent, "idProduct");

        if (vendor_id && product_id)
        {
            char cstring[HARDWARE_ID_STRING_LENGTH];

            if (serial_number.empty())
                serial_number = "None";

            int ret = snprintf(cstring, HARDWARE_ID_STRING_LENGTH, "USB VID:PID=%04x:%04x SNR=%s",
                               vendor_id, product_id, serial_number.c_str());

            if ((ret >= 0) && (ret < HARDWARE_ID_STRING_LENGTH))
                port_info->hardware_id = gensym(cstring);
        }

        hashtab_store(devices_found, port_info->port, (t_object *)port_info);
    }

    IOObjectRelease(serial_port_iterator);

    return devices_found;
}

#endif