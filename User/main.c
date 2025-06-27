/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************
 * �û�����ʵ�����ݣ�
 *
 * 1.dap_main.c/h
 * 2.DAP_config.h
 * 3.usb_config.h
 * 4.usb2uart.c/h
 * 5.main.c
 *******************************************************************************/


#include "user_fatfs.h"
#include "debug.h"
#include "hid_custom.h"


/*********************************************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 *********************************************************************************************/
int main (void) {
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);  // �����ж�
    SystemCoreClockUpdate();                          // ����ʱ��

    Delay_Init();
    USART_Printf_Init (115200);  // printf���ڳ�ʼ��

    printf ("SystemClk:%dMHz,ChipID:%08X\r\n\r\n", SystemCoreClock/1000000, DBGMCU_GetCHIPID());//��ӡϵͳ��Ϣ
    //1.�ϵ����е��˴�
    //2.�жϰ���״̬������ֱ����ת��app,���½������������������������
    //3.����״̬������HID��ʼ��״̬
    //4.������λ��ָ�����APP���������������á�
    hid_custom_init(0,0);
    
    while (!usb_device_is_configured());             // �ȴ�USB���ö���������
    printf ("usb_device_is_configured\r\n");

    while (1)  // ����DAP���ݴ���ʹ������ݴ���
    {
        hid_ry_hid_handle();
    }
}

void USBHS_RCC_Init (void) {
    RCC_USBCLK48MConfig (RCC_USBCLK48MCLKSource_USBPHY);
    RCC_USBHSPLLCLKConfig (RCC_HSBHSPLLCLKSource_HSE);
    RCC_USBHSConfig (RCC_USBPLL_Div2);  // V305:RCC_USBPLL_Div3
    RCC_USBHSPLLCKREFCLKConfig (RCC_USBHSPLLCKREFCLK_4M);
    RCC_USBHSPHYPLLALIVEcmd (ENABLE);
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_USBHS, ENABLE);
}

void usb_dc_low_level_init (void) {  // hid_custom_init()������
    USBHS_RCC_Init();
    NVIC_EnableIRQ (USBHS_IRQn);
}