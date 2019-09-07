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

DEFINE_GUID (GUID_DEVINTERFACE_ProctectDriver,
    0xea4367a1,0x6882,0x4ff7,0x8c,0x39,0x1e,0x3e,0x6e,0xc4,0xf3,0xcc);
// {ea4367a1-6882-4ff7-8c39-1e3e6ec4f3cc}
