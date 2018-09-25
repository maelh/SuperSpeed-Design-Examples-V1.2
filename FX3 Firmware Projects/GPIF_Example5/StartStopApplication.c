/*
 * StartStopApplication.c
 *
 *      Author: john@usb-by-example.com
 */

#include "Application.h"
#include "SlaveFifoWrite.h"				// File generated by GPIF Designer
char* CyFxGpifConfigName = { "SlaveFifoWrite" };

// Declare external functions
extern void CheckStatus(char* StringPtr, CyU3PReturnStatus_t Status);

// Declare external data
extern CyBool_t glIsApplicationActive;		// Set true once device is enumerated
extern uint32_t ClockValue;					// Used to select GPIF speed

CyU3PDmaChannel GPIF2USB_Handle, USB2GPIF_Handle;

const char* BusSpeed[] = { "Not Connected", "Full ", "High ", "Super" };
const uint16_t EpSize[] = { 0, 64, 512, 1024 };

CyU3PReturnStatus_t StartGPIF(void)
{
	CyU3PReturnStatus_t Status;
	Status = CyU3PGpifLoad(&CyFxGpifConfig);
	DebugPrint(7, "\r\nUsing GPIF:%s", CyFxGpifConfigName);
	CheckStatus("GpifLoad", Status);
	Status = CyU3PGpifSocketConfigure(1, GPIF_CONSUMER_SOCKET, 3, CyFalse, 1);
	CheckStatus("SetWatermark", Status);
	Status = CyU3PGpifSMStart(START, ALPHA_START);
	return Status;
}

void StartApplication(void)
// USB has been enumerated, time to start the application running
{
	CyU3PEpConfig_t epConfig;
	CyU3PDmaChannelConfig_t dmaConfig;
	CyU3PReturnStatus_t Status;
    CyU3PPibClock_t pibClock;

    CyU3PUSBSpeed_t usbSpeed = CyU3PUsbGetSpeed();
    // Display the enumerated device bus speed
    DebugPrint(4, "\r\n@StartApplication, running at %sSpeed", BusSpeed[usbSpeed]);

    // Start GPIF clocks, they need to be running before we attach a DMA channel to GPIF
    pibClock.clkDiv = ClockValue>>1;
    pibClock.clkSrc = CY_U3P_SYS_CLK;
    pibClock.isHalfDiv = (ClockValue & 1);
    pibClock.isDllEnable = CyFalse;		// Disable Dll since this application is synchronous
    Status = CyU3PPibInit(CyTrue, &pibClock);
 	CheckStatus("Start GPIF Clock", Status);
 	DebugPrint(4, "\r\nGPIF Clock = %d MHz = %d MB/s", 800/ClockValue, 3200/ClockValue);

    // Based on the Bus Speed configure the endpoint packet size
	CyU3PMemSet((uint8_t *)&epConfig, 0, sizeof(epConfig));
	epConfig.enable = CyTrue;
	epConfig.epType = CY_U3P_USB_EP_BULK;
	epConfig.burstLen = (usbSpeed == CY_U3P_SUPER_SPEED) ? (ENDPOINT_BURST_LENGTH) : 1;
	epConfig.pcktSize = EpSize[usbSpeed];

	// Setup and flush the producer endpoint
	Status = CyU3PSetEpConfig(USB_PRODUCER_ENDPOINT, &epConfig);
	CheckStatus("Setup USB_PRODUCER_ENDPOINT", Status);

	// Create a AUTO channel for the USB to GPIF transfer, USB detects and COMMITs the last short packet
	CyU3PMemSet((uint8_t *)&dmaConfig, 0, sizeof(dmaConfig));
	dmaConfig.size           = DMA_BUFFER_SIZE;			// Use same size buffers for all USB Speeds
	dmaConfig.count          = DMA_BUFFER_COUNT;
	dmaConfig.prodSckId		 = USB_PRODUCER_ENDPOINT_SOCKET;
	dmaConfig.consSckId		 = GPIF_CONSUMER_SOCKET;
	dmaConfig.dmaMode        = CY_U3P_DMA_MODE_BYTE;
	Status = CyU3PDmaChannelCreate(&USB2GPIF_Handle, CY_U3P_DMA_TYPE_AUTO, &dmaConfig);
	CheckStatus("GPIF2USB DmaChannelCreate", Status);

	Status = CyU3PUsbFlushEp(USB_PRODUCER_ENDPOINT);
	CheckStatus("Flush USB_PRODUCER_ENDPOINT", Status);

	// Start the DMA Channel with transfer size to Infinite
	Status = CyU3PDmaChannelSetXfer(&USB2GPIF_Handle, 0);
	CheckStatus("USB2GPIF DmaChannelStart", Status);

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

    // Disable GPIF, close the DMA channels, flush and disable the endpoints
    CyU3PGpifDisable(CyTrue);
    Status = CyU3PPibDeInit();
    CheckStatus("Stop GPIF Block", Status);
    Status = CyU3PDmaChannelDestroy(&USB2GPIF_Handle);
    CheckStatus("USB2GPIF DmaChannelDestroy", Status);
    Status = CyU3PUsbFlushEp(USB_PRODUCER_ENDPOINT);
    CheckStatus("Flush USB_PRODUCER_ENDPOINT", Status);
	CyU3PMemSet((uint8_t *)&epConfig, 0, sizeof(&epConfig));
    Status = CyU3PSetEpConfig(USB_PRODUCER_ENDPOINT, &epConfig);
	CheckStatus("Disable USB_PRODUCER_ENDPOINT", Status);

    // OK, Application is now stopped
    glIsApplicationActive = CyFalse;
}

