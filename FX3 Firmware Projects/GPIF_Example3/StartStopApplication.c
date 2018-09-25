/*
 * StartStopApplication.c
 *
 *      Author: john@usb-by-example.com
 */

#include "Application.h"
#include "CPLDinControl.h"				// File generated by GPIF Designer
char* CyFxGpifConfigName = { "CPLDinControl" };

// Declare external functions
extern void CheckStatus(char* StringPtr, CyU3PReturnStatus_t Status);

// Declare external data
extern CyBool_t glIsApplicationActive;		// Set true once device is enumerated
extern uint32_t ClockValue;					// Used to select GPIF speed

// Global data owned by this module
CyU3PDmaMultiChannel glDualGPIF2USB_Handle;
uint32_t DataMissedCounter;

const char* BusSpeed[] = { "Not Connected", "Full ", "High ", "Super" };
const uint16_t EpSize[] = { 0, 64, 512, 1024 };

void GpifCallBack(CyU3PGpifEventType Event, uint8_t State)
{
	if (Event == CYU3P_GPIF_EVT_SM_INTERRUPT) DebugPrint(4, "\r\nData missed = %d", ++DataMissedCounter);
}

CyU3PReturnStatus_t StartGPIF(void)
{
	CyU3PReturnStatus_t Status;
	Status = CyU3PGpifLoad(&CyFxGpifConfig);
	DebugPrint(7, "\r\nUsing GPIF:%s", CyFxGpifConfigName);
	CyU3PGpifRegisterCallback(GpifCallBack);
	CheckStatus("GpifLoad", Status);
	Status = CyU3PGpifSMStart(START, ALPHA_START);
	return Status;
}

void StartApplication(void)
// USB has been enumerated, time to start the application running
{
	CyU3PEpConfig_t epConfig;
    CyU3PDmaMultiChannelConfig_t dmaMultiConfig;
	CyU3PReturnStatus_t Status;
    CyU3PPibClock_t pibClock;


    CyU3PUSBSpeed_t usbSpeed = CyU3PUsbGetSpeed();
    // Tell the enumerated device bus speed to developer and to CPLD
    DebugPrint(4, "\r\n@StartApplication, running at %sSpeed", BusSpeed[usbSpeed]);

    // Start GPIF clocks, they need to be running before we attach a DMA channel to GPIF
    pibClock.clkDiv = ClockValue>>1;
    pibClock.clkSrc = CY_U3P_SYS_CLK;
    pibClock.isHalfDiv = (ClockValue & 1);
    pibClock.isDllEnable = CyFalse;		// Disable Dll since this application is synchronous
    Status = CyU3PPibInit(CyTrue, &pibClock);
 	CheckStatus("Start GPIF Clock", Status);
 	DebugPrint(4, "\r\nGPIF Clock = %d MHz = %d MB/s", 800/ClockValue, 3200/ClockValue);

    CyU3PMemSet((uint8_t *)&epConfig, 0, sizeof(epConfig));
	epConfig.enable = CyTrue;
	epConfig.epType = CY_U3P_USB_EP_BULK;
	epConfig.burstLen = (usbSpeed == CY_U3P_SUPER_SPEED) ? (ENDPOINT_BURST_LENGTH) : 1;
	epConfig.pcktSize = EpSize[usbSpeed];

	// Setup and flush the Consumer endpoint
	Status = CyU3PSetEpConfig(CONSUMER_ENDPOINT, &epConfig);
	CheckStatus("CyU3PSetEpConfig_Enable", Status);
	CyU3PUsbFlushEp(CONSUMER_ENDPOINT);

	// Create a multi-DMA AUTO channel for the GPIF to USB transfer
	CyU3PMemSet((uint8_t *)&dmaMultiConfig, 0, sizeof(dmaMultiConfig));
	dmaMultiConfig.size           = DMA_BUFFER_SIZE;			// Use same size buffers for all USB Speeds
	dmaMultiConfig.count          = DMA_BUFFER_COUNT;
	dmaMultiConfig.validSckCount  = 2;		// Number of producer sockets
	dmaMultiConfig.prodSckId[0]   = (CyU3PDmaSocketId_t)PING_PRODUCER_SOCKET;
	dmaMultiConfig.prodSckId[1]   = (CyU3PDmaSocketId_t)PONG_PRODUCER_SOCKET;
	dmaMultiConfig.consSckId[0]   = (CyU3PDmaSocketId_t)CONSUMER_ENDPOINT_SOCKET;
	dmaMultiConfig.dmaMode        = CY_U3P_DMA_MODE_BYTE;
	Status = CyU3PDmaMultiChannelCreate(&glDualGPIF2USB_Handle, CY_U3P_DMA_TYPE_AUTO_MANY_TO_ONE, &dmaMultiConfig);
	CheckStatus("DmaMultiChannelCreate", Status);
	// Start the DMA Channel with transfer size to Infinite
	Status = CyU3PDmaMultiChannelSetXfer(&glDualGPIF2USB_Handle, 0, 0);
	CheckStatus("DmaMultiChannelStart", Status);

	// Load, configure and start the GPIF state machine
    Status = StartGPIF();
	CheckStatus("GpifStart", Status);

	// OK, Application can now run
    glIsApplicationActive = CyTrue;
}

void StopApplication(void)
// USB connection has been lost, time to stop the application running
{
    CyU3PEpConfig_t epConfig;
    CyU3PReturnStatus_t Status;

    // Disable GPIF, close the DMA channel, flush and disable the endpoint
    CyU3PGpifDisable(CyTrue);
    Status = CyU3PPibDeInit();
    CheckStatus("Turn off GPIF Clocks", Status);
	Status = CyU3PDmaMultiChannelDestroy(&glDualGPIF2USB_Handle);
    CheckStatus("DmaChannelDestroy", Status);

    Status = CyU3PUsbFlushEp(CONSUMER_ENDPOINT);
    CheckStatus("FlushEndpoint", Status);
    CyU3PMemSet((uint8_t *)&epConfig, 0, sizeof(&epConfig));
    Status = CyU3PSetEpConfig(CONSUMER_ENDPOINT, &epConfig);
	CheckStatus("SetEndpointConfig_Disable", Status);

    // OK, Application is now stopped
    glIsApplicationActive = CyFalse;
}

