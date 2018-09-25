/*
 * USB_Descriptors.c	This file contains the USB enumeration descriptors for the application and a routine that sets them
 *						This example uses the "Bulk Loop" descriptors
 */

#include "Application.h"

extern void CheckStatus(char* StringPtr, CyU3PReturnStatus_t Status);

/* Standard device descriptor for USB 3.0 */
const uint8_t CyFxUSB30DeviceDscr[] __attribute__ ((aligned (32))) =
{
    0x12,                           /* Descriptor size */
    CY_U3P_USB_DEVICE_DESCR,        /* Device descriptor type */
    0x00,0x03,                      /* USB 3.0 */
    0x00,                           /* Device class */
    0x00,                           /* Device sub-class */
    0x00,                           /* Device protocol */
    0x09,                           /* Maxpacket size for EP0 : 2^9 */
    0xB4,0x04,                      /* Vendor ID */
    0xF0,0x00,                      /* Product ID */
    0x00,0x00,                      /* Device release number */
    0x01,                           /* Manufacture string index */
    0x02,                           /* Product string index */
    0x00,                           /* Serial number string index */
    0x01                            /* Number of configurations */
};

/* Standard device descriptor for USB 2.0 */
const uint8_t CyFxUSB20DeviceDscr[] __attribute__ ((aligned (32))) =
{
    0x12,                           /* Descriptor size */
    CY_U3P_USB_DEVICE_DESCR,        /* Device descriptor type */
    0x10,0x02,                      /* USB 2.10 */
    0x00,                           /* Device class */
    0x00,                           /* Device sub-class */
    0x00,                           /* Device protocol */
    0x40,                           /* Maxpacket size for EP0 : 64 bytes */
    0xB4,0x04,                      /* Vendor ID */
    0xF0,0x00,                      /* Product ID */
    0x00,0x00,                      /* Device release number */
    0x01,                           /* Manufacture string index */
    0x02,                           /* Product string index */
    0x00,                           /* Serial number string index */
    0x01                            /* Number of configurations */
};

/* Binary device object store descriptor */
const uint8_t CyFxUSBBOSDscr[] __attribute__ ((aligned (32))) =
{
    0x05,                           /* Descriptor size */
    CY_U3P_BOS_DESCR,               /* Device descriptor type */
    0x16,0x00,                      /* Length of this descriptor and all sub descriptors */
    0x02,                           /* Number of device capability descriptors */

    /* USB 2.0 extension */
    0x07,                           /* Descriptor size */
    CY_U3P_DEVICE_CAPB_DESCR,       /* Device capability type descriptor */
    CY_U3P_USB2_EXTN_CAPB_TYPE,     /* USB 2.0 extension capability type */
    0x02,0x00,0x00,0x00,            /* Supported device level features: LPM support  */

    /* SuperSpeed device capability */
    0x0A,                           /* Descriptor size */
    CY_U3P_DEVICE_CAPB_DESCR,       /* Device capability type descriptor */
    CY_U3P_SS_USB_CAPB_TYPE,        /* SuperSpeed device capability type */
    0x00,                           /* Supported device level features  */
    0x0E,0x00,                      /* Speeds supported by the device : SS, HS and FS */
    0x03,                           /* Functionality support */
    0x00,                           /* U1 Device Exit latency */
    0x00,0x00                       /* U2 Device Exit latency */
};

/* Standard device qualifier descriptor */
const uint8_t CyFxUSBDeviceQualDscr[] __attribute__ ((aligned (32))) =
{
    0x0A,                           /* Descriptor size */
    CY_U3P_USB_DEVQUAL_DESCR,       /* Device qualifier descriptor type */
    0x00,0x02,                      /* USB 2.0 */
    0x00,                           /* Device class */
    0x00,                           /* Device sub-class */
    0x00,                           /* Device protocol */
    0x40,                           /* Maxpacket size for EP0 : 64 bytes */
    0x01,                           /* Number of configurations */
    0x00                            /* Reserved */
};

/* Standard super speed configuration descriptor */
const uint8_t CyFxUSBSSConfigDscr[] __attribute__ ((aligned (32))) =
{
    /* Configuration descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_CONFIG_DESCR,        /* Configuration descriptor type */
    0x2C,0x00,                      /* Length of this descriptor and all sub descriptors */
    0x01,                           /* Number of interfaces */
    0x01,                           /* Configuration number */
    0x00,                           /* COnfiguration string index */
    0x80,                           /* Config characteristics - Bus powered */
    0x32,                           /* Max power consumption of device (in 8mA unit) : 400mA */

    /* Interface descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_INTRFC_DESCR,        /* Interface Descriptor type */
    0x00,                           /* Interface number */
    0x00,                           /* Alternate setting number */
    0x02,                           /* Number of end points */
    0xFF,                           /* Interface class */
    0x00,                           /* Interface sub class */
    0x00,                           /* Interface protocol code */
    0x00,                           /* Interface descriptor string index */

    /* Endpoint descriptor for producer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_PRODUCER,              		/* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x00,0x04,                      /* Max packet size = 1024 bytes */
    0x00,                           /* Servicing interval for data transfers : 0 for bulk */

    /* Super speed endpoint companion descriptor for producer EP */
    0x06,                           /* Descriptor size */
    CY_U3P_SS_EP_COMPN_DESCR,       /* SS endpoint companion descriptor type */
    0x00,                           /* Max no. of packets in a burst : 0: burst 1 packet at a time */
    0x00,                           /* Max streams for bulk EP = 0 (No streams) */
    0x00,0x00,                      /* Service interval for the EP : 0 for bulk */

    /* Endpoint descriptor for consumer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_CONSUMER,              		/* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x00,0x04,                      /* Max packet size = 1024 bytes */
    0x00,                           /* Servicing interval for data transfers : 0 for Bulk */

    /* Super speed endpoint companion descriptor for consumer EP */
    0x06,                           /* Descriptor size */
    CY_U3P_SS_EP_COMPN_DESCR,       /* SS endpoint companion descriptor type */
    0x00,                           /* Max no. of packets in a burst : 0: burst 1 packet at a time */
    0x00,                           /* Max streams for bulk EP = 0 (No streams) */
    0x00,0x00                       /* Service interval for the EP : 0 for bulk */
};

/* Standard high speed configuration descriptor */
const uint8_t CyFxUSBHSConfigDscr[] __attribute__ ((aligned (32))) =
{
    /* Configuration descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_CONFIG_DESCR,        /* Configuration descriptor type */
    0x20,0x00,                      /* Length of this descriptor and all sub descriptors */
    0x01,                           /* Number of interfaces */
    0x01,                           /* Configuration number */
    0x00,                           /* Configuration string index */
    0x80,                           /* Configuration characteristics - bus powered */
    0x32,                           /* Max power consumption of device (in 2mA unit) : 100mA */

    /* Interface descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_INTRFC_DESCR,        /* Interface Descriptor type */
    0x00,                           /* Interface number */
    0x00,                           /* Alternate setting number */
    0x02,                           /* Number of endpoints */
    0xFF,                           /* Interface class */
    0x00,                           /* Interface sub class */
    0x00,                           /* Interface protocol code */
    0x00,                           /* Interface descriptor string index */

    /* Endpoint descriptor for producer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_PRODUCER,         		     /* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x00,0x02,                      /* Max packet size = 512 bytes */
    0x00,                           /* Servicing interval for data transfers : 0 for bulk */

    /* Endpoint descriptor for consumer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_CONSUMER,        		      /* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x00,0x02,                      /* Max packet size = 512 bytes */
    0x00                            /* Servicing interval for data transfers : 0 for bulk */
};

/* Standard full speed configuration descriptor */
const uint8_t CyFxUSBFSConfigDscr[] __attribute__ ((aligned (32))) =
{
    /* Configuration descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_CONFIG_DESCR,        /* Configuration descriptor type */
    0x20,0x00,                      /* Length of this descriptor and all sub descriptors */
    0x01,                           /* Number of interfaces */
    0x01,                           /* Configuration number */
    0x00,                           /* COnfiguration string index */
    0x80,                           /* Config characteristics - bus powered */
    0x32,                           /* Max power consumption of device (in 2mA unit) : 100mA */

    /* Interface descriptor */
    0x09,                           /* Descriptor size */
    CY_U3P_USB_INTRFC_DESCR,        /* Interface descriptor type */
    0x00,                           /* Interface number */
    0x00,                           /* Alternate setting number */
    0x02,                           /* Number of endpoints */
    0xFF,                           /* Interface class */
    0x00,                           /* Interface sub class */
    0x00,                           /* Interface protocol code */
    0x00,                           /* Interface descriptor string index */

    /* Endpoint descriptor for producer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_PRODUCER,        		    /* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x40,0x00,                      /* Max packet size = 64 bytes */
    0x00,                           /* Servicing interval for data transfers : 0 for bulk */

    /* Endpoint descriptor for consumer EP */
    0x07,                           /* Descriptor size */
    CY_U3P_USB_ENDPNT_DESCR,        /* Endpoint descriptor type */
    EP_CONSUMER,              		/* Endpoint address and description */
    CY_U3P_USB_EP_BULK,             /* Bulk endpoint type */
    0x40,0x00,                      /* Max packet size = 64 bytes */
    0x00                            /* Servicing interval for data transfers : 0 for bulk */
};

/* Standard language ID string descriptor */
const uint8_t CyFxUSBStringLangIDDscr[] __attribute__ ((aligned (32))) =
{
    4,                           	/* Descriptor size */
    CY_U3P_USB_STRING_DESCR,        /* Device descriptor type */
    9,4                       		/* Language ID supported */
};

/* Standard manufacturer string descriptor */
const uint8_t CyFxUSBManufactureDscr[] __attribute__ ((aligned (32))) =
{
    44,                             /* Descriptor size */
    CY_U3P_USB_STRING_DESCR,        /* Device descriptor type */
    'U',0,'S',0,'B',0,' ',0,'D',0,'e',0,'s',0,'i',0,'g',0,'n',0,' ',0,
    'B',0,'y',0,' ',0,'E',0,'x',0,'a',0,'m',0,'p',0,'l',0,'e',0
};

/* Standard product string descriptor */
const uint8_t CyFxUSBProductDscr[] __attribute__ ((aligned (32))) =
{
    32,                           	/* Descriptor size */
    CY_U3P_USB_STRING_DESCR,        /* Device descriptor type */
    'C',0,'P',0,'L',0,'D',0,' ',0,'P',0,'r',0,'o',0,
    'g',0,'r',0,'a',0,'m',0,'m',0,'e',0,'r',0
};

/* Microsoft OS Descriptor. */
const uint8_t CyFxUsbOSDscr[] __attribute__ ((aligned (32))) =
{
    14,
    CY_U3P_USB_STRING_DESCR,
    'O',0,'S',0,' ',0,'D',0,'e',0,'s',0,'c',0
};

/* Place this buffer as the last buffer so that no other variable / code shares
 * the same cache line. Do not add any other variables / arrays in this file.
 * This will lead to variables sharing the same cache line. */
const uint8_t CyFxUsbDscrAlignBuffer[32] __attribute__ ((aligned (32)));


CyU3PReturnStatus_t SetUSBdescriptors(void)
{
	CyU3PReturnStatus_t OverallStatus, Status;
	OverallStatus = Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_DEVICE_DESCR, NULL, (uint8_t *)CyFxUSB30DeviceDscr);
    CheckStatus("SET_SS_DEVICE_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_HS_DEVICE_DESCR, NULL, (uint8_t *)CyFxUSB20DeviceDscr);
    CheckStatus("SET_HS_DEVICE_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_BOS_DESCR, NULL, (uint8_t *)CyFxUSBBOSDscr);
    CheckStatus("SET_SS_BOS_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_DEVQUAL_DESCR, NULL, (uint8_t *)CyFxUSBDeviceQualDscr);
    CheckStatus("SET_DEVQUAL_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBSSConfigDscr);
    CheckStatus("SET_SS_CONFIG_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_HS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBHSConfigDscr);
    CheckStatus("SET_HS_CONFIG_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_FS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBFSConfigDscr);
    CheckStatus("SET_FS_CONFIG_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 0, (uint8_t *)CyFxUSBStringLangIDDscr);
    CheckStatus("SET_STRING0_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 1, (uint8_t *)CyFxUSBManufactureDscr);
    CheckStatus("SET_STRING1_DESCR", Status);
    OverallStatus |= Status = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 2, (uint8_t *)CyFxUSBProductDscr);
    CheckStatus("SET_STRING2_DESCR", Status);

    return OverallStatus;
}
