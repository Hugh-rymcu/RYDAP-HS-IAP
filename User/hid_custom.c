/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "hid_custom.h"

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP 0x81
#ifdef CONFIG_USB_HS
#define HIDRAW_IN_EP_SIZE 1024
#define HIDRAW_IN_INTERVAL 4
#else
#define HIDRAW_IN_EP_SIZE 64
#define HIDRAW_IN_INTERVAL 10
#endif
/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP 0x02
#ifdef CONFIG_USB_HS
#define HIDRAW_OUT_EP_SIZE 1024
#define HIDRAW_OUT_EP_INTERVAL 4
#else
#define HIDRAW_OUT_EP_SIZE 64
#define HIDRAW_OUT_EP_INTERVAL 10
#endif

#define USBD_VID 0x0D28
#define USBD_PID 0x0204
#define USBD_MAX_POWER 100
#define USBD_LANGID_STRING 1033

/*!< config descriptor size */
#define USB_HID_CONFIG_DESC_SIZ (9 + 9 + 9 + 7 + 7)

/*!< custom hid report descriptor size */
#define HID_CUSTOM_REPORT_DESC_SIZE 38

#ifdef CONFIG_USBDEV_ADVANCE_DESC
static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT (USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01)};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT (USB_HID_CONFIG_DESC_SIZ, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    /************** Descriptor of Custom interface *****************/
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x02,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                        /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID,     /* bDescriptorType: HID */
    0x11,                        /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_CUSTOM_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_IN_EP,                 /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL (HIDRAW_IN_EP_SIZE),    /* wMaxPacketSize: 4 Byte max */
    HIDRAW_IN_INTERVAL,           /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_OUT_EP,                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL (HIDRAW_OUT_EP_SIZE),   /* wMaxPacketSize: 4 Byte max */
    HIDRAW_OUT_EP_INTERVAL,       /* bInterval: Polling Interval */
    /* 73 */
};

static const uint8_t device_quality_descriptor[] = {
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
};

static const char *string_descriptors[] = {
    (const char[]){0x09, 0x04}, /* Langid */
    "CherryUSB", /* Manufacturer */
    "CherryUSB HID DEMO", /* Product */
    "2022123456", /* Serial Number */
};

static const uint8_t *device_descriptor_callback (uint8_t speed) {
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback (uint8_t speed) {
    return config_descriptor;
}

static const uint8_t *device_quality_descriptor_callback (uint8_t speed) {
    return device_quality_descriptor;
}

static const char *string_descriptor_callback (uint8_t speed, uint8_t index) {
    if (index > 3) {
        return NULL;
    }
    return string_descriptors[index];
}

const struct usb_descriptor hid_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = device_quality_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback};
#else
/*!< global descriptor */
static const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT (USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT (USB_HID_CONFIG_DESC_SIZ, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    /************** Descriptor of Custom interface *****************/
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x02,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                        /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID,     /* bDescriptorType: HID */
    0x11,                        /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_CUSTOM_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_IN_EP,                 /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL (HIDRAW_IN_EP_SIZE),    /* wMaxPacketSize: 4 Byte max */
    HIDRAW_IN_INTERVAL,           /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_OUT_EP,                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL (HIDRAW_OUT_EP_SIZE),   /* wMaxPacketSize: 4 Byte max */
    HIDRAW_OUT_EP_INTERVAL,       /* bInterval: Polling Interval */
    /* 73 */
    /*
     * string0 descriptor
     */
    USB_LANGID_INIT (USBD_LANGID_STRING),
    /*
     * string1 descriptor
     */
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    /*
     * string2 descriptor
     */
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'R', 0x00,                  /* wcChar0 */
    'Y', 0x00,                  /* wcChar1 */
    'M', 0x00,                  /* wcChar2 */
    'C', 0x00,                  /* wcChar3 */
    'U', 0x00,                  /* wcChar4 */
    ' ', 0x00,                  /* wcChar5 */
    'R', 0x00,                  /* wcChar6 */
    'Y', 0x00,                  /* wcChar7 */
    'D', 0x00,                  /* wcChar8 */
    'A', 0x00,                  /* wcChar9 */
    'P', 0x00,                  /* wcChar10 */
    '-', 0x00,                  /* wcChar11 */
    'H', 0x00,                  /* wcChar12 */
    'S', 0x00,                  /* wcChar13 */
    ' ', 0x00,                  /* wcChar14 */
    ' ', 0x00,                  /* wcChar15 */
    ' ', 0x00,                  /* wcChar16 */
    ' ', 0x00,                  /* wcChar17 */
    /*
     * string3 descriptor
     */
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    /*
     * device qualifier descriptor
     */
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
#endif
    0x00};
#endif

/*!< custom hid report descriptor */
static const uint8_t hid_custom_report_desc[HID_CUSTOM_REPORT_DESC_SIZE] = {
#ifdef CONFIG_USB_HS
    /* USER CODE BEGIN 0 */
    0x06, 0x00, 0xff, /* USAGE_PAGE (Vendor Defined Page 1) */
    0x09, 0x01,       /* USAGE (Vendor Usage 1) */
    0xa1, 0x01,       /* COLLECTION (Application) */
    0x85, 0x02,       /*   REPORT ID (0x02) */
    0x09, 0x02,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x25, 0xff,       /*LOGICAL_MAXIMUM (255) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x96, 0xff, 0x03, /*   REPORT_COUNT (63) */
    0x81, 0x02,       /*   INPUT (Data,Var,Abs) */
    /* <___________________________________________________> */
    0x85, 0x01,       /*   REPORT ID (0x01) */
    0x09, 0x03,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x25, 0xff,       /*   LOGICAL_MAXIMUM (255) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x96, 0xff, 0x03, /*   REPORT_COUNT (63) */
    0x91, 0x02,       /*   OUTPUT (Data,Var,Abs) */
    /* USER CODE END 0 */
    0xC0 /*     END_COLLECTION	             */
#else
    /* USER CODE BEGIN 0 */
    0x06, 0x00, 0xff, /* USAGE_PAGE (Vendor Defined Page 1) */
    0x09, 0x01,       /* USAGE (Vendor Usage 1) */
    0xa1, 0x01,       /* COLLECTION (Application) */
    0x85, 0x02,       /*   REPORT ID (0x02) */
    0x09, 0x01,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
    0x95, 0x40 - 1,   /*   REPORT_COUNT (63) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x81, 0x02,       /*   INPUT (Data,Var,Abs) */
    /* <___________________________________________________> */
    0x85, 0x01,       /*   REPORT ID (0x01) */
    0x09, 0x01,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
    0x95, 0x40 - 1,   /*   REPORT_COUNT (63) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x91, 0x02,       /*   OUTPUT (Data,Var,Abs) */
    /* USER CODE END 0 */
    0xC0 /*     END_COLLECTION	             */
#endif
};

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[HIDRAW_OUT_EP_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t send_buffer[HIDRAW_IN_EP_SIZE];

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t custom_state;
static volatile uint8_t custom_out_flag;  // 1:收到数据，0:没收到数据

void usbd_event_handler (uint8_t event) {
    switch (event) {
    case USBD_EVENT_RESET:
        break;
    case USBD_EVENT_CONNECTED:
        break;
    case USBD_EVENT_DISCONNECTED:
        break;
    case USBD_EVENT_RESUME:
        break;
    case USBD_EVENT_SUSPEND:
        break;
    case USBD_EVENT_CONFIGURED:
        /* setup first out ep read transfer */
        usbd_ep_start_read (HIDRAW_OUT_EP, read_buffer, HIDRAW_OUT_EP_SIZE);
        break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
        break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
        break;

    default:
        break;
    }
}

static void usbd_hid_custom_in_callback (uint8_t ep, uint32_t nbytes) {
    (void)ep;
    USB_LOG_RAW ("actual in len:%d\r\n", (unsigned int)nbytes);
    custom_state = HID_STATE_IDLE;
}

static void usbd_hid_custom_out_callback (uint8_t ep, uint32_t nbytes) {
    USB_LOG_RAW ("actual out len:%d\r\n", (unsigned int)nbytes);
    // set flag
    custom_out_flag = 1;
    //  for(uint32_t i=0;i<1024;i++)
    //  {
    //      USB_LOG_RAW("%02x ", read_buffer[i]);
    //  }
    // for(uint32_t i=0;i<1024;i++)read_buffer[i] = i&0xff;
    // read_buffer[0] = 0x02; /* IN: report id */
    // usbd_ep_start_write(HIDRAW_IN_EP, read_buffer, nbytes);
    // usbd_ep_start_read(ep, read_buffer, HIDRAW_IN_EP_SIZE);
    // 1.收到主机发来数据进入该函数
    // 2.处理接收到的read_buffer[1-1023]的数据
    // 3.处理完之后，设置send_buffer内容，准备好回复主机的数据
    // 4.调用usbd_ep_start_write(HIDRAW_IN_EP, send_buffer, nbytes)
    // 5.主机将读取到回复的数据
    // 6.调用usbd_ep_start_read(ep, read_buffer, HIDRAW_IN_EP_SIZE)
    // 7.设备可以收到主机发来的数据
}

static struct usbd_endpoint custom_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = HIDRAW_IN_EP};

static struct usbd_endpoint custom_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = HIDRAW_OUT_EP};

/* function ------------------------------------------------------------------*/
/**
 * @brief            hid custom init
 * @pre              none
 * @param[in]        none
 * @retval           none
 */
struct usbd_interface intf0;

void hid_custom_init (uint8_t busid, uintptr_t reg_base) {
#ifdef CONFIG_USBDEV_ADVANCE_DESC
    usbd_desc_register (busid, &hid_descriptor);
#else
    usbd_desc_register (hid_descriptor);
#endif
    usbd_add_interface (usbd_hid_init_intf (busid, &intf0, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE));
    usbd_add_endpoint (&custom_in_ep);
    usbd_add_endpoint (&custom_out_ep);

    usbd_initialize();
}

typedef enum {
    FIRMWARE_UPGRADE = 0xAABB,
    SETUP_UPGRADE = 0xCCDD,
    LOAD_UPGRADE = 0xEEFF,
    UNKNOW_TYPE
} HID_DATA_TYPE;

void firmware_upgrade_handle (void);
void setup_upgrade_handle (void);
void load_uprade_handle (void);

//--------------------------数据处理------------------------------------
// 判断：固件升级(firmware.bin)，设置更新(setup.ry)，下载bin更新(load.bin)
// 每包数据1024Byte,read_buffer[1024]
//---------------------------------------------------------------------
// read_buffer[0] =0x01为固定值，buffer[1-1023]为有效值
// 定义：read_buffer[1-2]表示数据类型，0xAABB:固件升级(firmware)
//                                  0xCCDD:设置更新(setup)
//                                  0xEEFF:下载bin更新(load)
// read_buffer[3-4]:有效数据长度(1019Byte)
// read_buffer[5-1023]:有效数据
//--------------------------------------------------------------------
// 根据数据类型read_buffer[1-2]进不同流程
// 有效数据长度read_buffer[3-4]判断数据是否传输完成，判断方法：当长度小于最
// 大时表示传输完成，因此要求当文件总长度能被有效最大整除时，最后需要多发一个0长度包
// 根据长度将数据存储到文件：firmware.bin，setup.ry，load.bin
// firmware.bin接收完成，进入擦除流程，完成后删除文件。
//--------------------------------------------------------------------
void hid_data_process (void) {
    USB_LOG_RAW ("hid_ry_hid_handle\r\n");
    USB_LOG_RAW ("%s", read_buffer);

    HID_DATA_TYPE hid_data_type = UNKNOW_TYPE;
    hid_data_type = (read_buffer[1] << 8) + read_buffer[2];
    switch (hid_data_type) {
    case FIRMWARE_UPGRADE:
        firmware_upgrade_handle();
        break;
    case SETUP_UPGRADE:
        setup_upgrade_handle();
        break;
    case LOAD_UPGRADE:
        load_uprade_handle();
        break;
    default:
        break;
    }
}

void hid_ry_hid_handle (void) {
    if (custom_out_flag == 1) {
        custom_out_flag = 0;
        hid_data_process();

        for (int i = 0; i < 1024; i++) {
            send_buffer[i] = i & 0xff;
        }
        send_buffer[0] = 0x02; /* IN: report id */
        usbd_ep_start_write (HIDRAW_IN_EP, send_buffer, HIDRAW_IN_EP_SIZE);
        usbd_ep_start_read (HIDRAW_OUT_EP, read_buffer, HIDRAW_OUT_EP_SIZE);
    }
}