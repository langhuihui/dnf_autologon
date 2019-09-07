/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_ProcessProtector,
    0x479dfea9,0xe5f0,0x48b5,0xb5,0x01,0xfb,0x49,0xae,0x45,0x24,0xaf);
// {479dfea9-e5f0-48b5-b501-fb49ae4524af}
