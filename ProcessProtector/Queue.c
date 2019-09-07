/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "queue.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, ProcessProtectorQueueInitialize)
#endif
static VOID EvtIoPDPControlDevice(WDFQUEUE Queue, WDFREQUEST Request)
{

	WDF_REQUEST_PARAMETERS Params;
	WDFREQUEST req;
	NTSTATUS status;
	WDFDEVICE  hDevice;
	DEVICE_CONTEXT* dev_ctx = NULL;

	//buffer  
	char* in_buf = NULL;
	char* out_buf = NULL;

	//从WDFREQUEST里面获取参数  
	WDF_REQUEST_PARAMETERS_INIT(&Params);
	WdfRequestGetParameters(Request, &Params);

	//DbgPrint(("EvtIoPDPControlDevice, type: %x\n", Params.Type));

	//获取input缓冲，如果request类型是WdfRequestTypeRead，那么就返回STATUS_INVALID_DEVICE_REQUEST  
	//因为WdfRequestTypeRead类型的Request没有input buffer，只有outputbuffer。  
	status = WdfRequestRetrieveInputBuffer(Request, 10, &in_buf, NULL);

	DbgPrint(("Get input buffer, ret: %x, buffer: %x", status, in_buf));

	//获取output缓冲，如果是WdfRequestTypeWrite，则没有输出缓冲，因为write只有输入缓冲。  
	status = WdfRequestRetrieveOutputBuffer(Request, 10, &out_buf, NULL);

	DbgPrint(("Get output buffer, ret: %x, buffer: %x", status, out_buf));

	//获取Queue关联的设备对象  
	hDevice = WdfIoQueueGetDevice(Queue);

	//获取设备对象的上下文  
	dev_ctx = WdfObjectGetTypedContext(hDevice, DEVICE_CONTEXT);

	DbgPrint(("Get the context of current device, %x", dev_ctx));


	switch (Params.Type)
	{
	case WdfRequestTypeRead:
	{
		if (out_buf != NULL)
		{
			KdPrint(("write the data in context buffer into READ request\n"));
			//将设备上下文里面的数据写入Read请求的缓冲里面  
			RtlCopyMemory(out_buf, dev_ctx->PrivateDeviceData, 4);
			WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 4);
		}
	}
		break;
	case WdfRequestTypeWrite:
	{
		if (in_buf != NULL)
		{
			pid = *(long*)in_buf;
			KdPrint(("input buffer,pid: %ld\n", pid));
			
			//将write请求的数据写入设备上下文  
			RtlCopyMemory(dev_ctx->PrivateDeviceData, in_buf, 4);

			WdfRequestComplete(Request, STATUS_SUCCESS);
		}
	}
		break;
	default:
		WdfRequestComplete(Request, STATUS_SUCCESS);
		break;
	}





}
NTSTATUS
ProcessProtectorQueueInitialize(
    _In_ WDFDEVICE Device
    )
/*++

Routine Description:


     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.

     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG    queueConfig;

    PAGED_CODE();
    
    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );
	queueConfig.PowerManaged = WdfFalse;
	queueConfig.EvtIoDefault = EvtIoPDPControlDevice;
    //queueConfig.EvtIoDeviceControl = ProcessProtectorEvtIoDeviceControl;
    queueConfig.EvtIoStop = ProcessProtectorEvtIoStop;

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if( !NT_SUCCESS(status) ) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }
	DbgPrint("queue初始化成功！");
    return status;
}

VOID
ProcessProtectorEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
/*++

Routine Description:

    This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request.

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.

    Request - Handle to a framework request object.

    OutputBufferLength - Size of the output buffer in bytes

    InputBufferLength - Size of the input buffer in bytes

    IoControlCode - I/O control code.

Return Value:

    VOID

--*/
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d", 
                Queue, Request, (int) OutputBufferLength, (int) InputBufferLength, IoControlCode);

	DbgPrint("收到请求%d", InputBufferLength);
	
    WdfRequestComplete(Request, STATUS_SUCCESS);

    return;
}

VOID
ProcessProtectorEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
/*++

Routine Description:

    This event is invoked for a power-managed queue before the device leaves the working state (D0).

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.

    Request - Handle to a framework request object.

    ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags
                  that identify the reason that the callback function is being called
                  and whether the request is cancelable.

Return Value:

    VOID

--*/
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d", 
                Queue, Request, ActionFlags);

    //
    // In most cases, the EvtIoStop callback function completes, cancels, or postpones
    // further processing of the I/O request.
    //
    // Typically, the driver uses the following rules:
    //
    // - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
    //   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
    //   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
    //   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
    //
    //   Before it can call these methods safely, the driver must make sure that
    //   its implementation of EvtIoStop has exclusive access to the request.
    //
    //   In order to do that, the driver must synchronize access to the request
    //   to prevent other threads from manipulating the request concurrently.
    //   The synchronization method you choose will depend on your driver's design.
    //
    //   For example, if the request is held in a shared context, the EvtIoStop callback
    //   might acquire an internal driver lock, take the request from the shared context,
    //   and then release the lock. At this point, the EvtIoStop callback owns the request
    //   and can safely complete or requeue the request.
    //
    // - If the driver has forwarded the I/O request to an I/O target, it either calls
    //   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
    //   further processing of the request and calls WdfRequestStopAcknowledge with
    //   a Requeue value of FALSE.
    //
    // A driver might choose to take no action in EvtIoStop for requests that are
    // guaranteed to complete in a small amount of time.
    //
    // In this case, the framework waits until the specified request is complete
    // before moving the device (or system) to a lower power state or removing the device.
    // Potentially, this inaction can prevent a system from entering its hibernation state
    // or another low system power state. In extreme cases, it can cause the system
    // to crash with bugcheck code 9F.
    //

    return;
}

