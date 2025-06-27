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
 * 用户自行实现内容：
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
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);  // 设置中断
    SystemCoreClockUpdate();                          // 设置时钟

    Delay_Init();
    USART_Printf_Init (115200);  // printf串口初始化

    printf ("SystemClk:%dMHz,ChipID:%08X\r\n\r\n", SystemCoreClock/1000000, DBGMCU_GetCHIPID());//打印系统信息
    //1.上电运行到此处
    //2.判断按键状态，弹起直接跳转到app,按下进入程序升级或者下载器设置
    //3.按下状态：进入HID初始化状态
    //4.更具上位机指令进行APP升级或下载器设置。
    hid_custom_init(0,0);
    
    while (!usb_device_is_configured());             // 等待USB完成枚举配置完成
    printf ("usb_device_is_configured\r\n");

    while (1)  // 进入DAP数据处理和串口数据处理
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

void usb_dc_low_level_init (void) {  // hid_custom_init()调用了
    USBHS_RCC_Init();
    NVIC_EnableIRQ (USBHS_IRQn);
}