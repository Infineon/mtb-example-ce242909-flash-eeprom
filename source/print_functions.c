/******************************************************************************
* File Name:   print_functions.c
*
* Description: This source file contains the necessary APIs to configure a USIC
* block as UART for debug messages. It uses the default UART pins available on 
* the kits.
*
* Related Document: See README.md
*
******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*****************************************************************************/
#include "cybsp.h"
#include "print_functions.h"

/*****************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ****************************************************************************/
static uint8_t hex_to_ascii(uint8_t data);
static void print_byte(uint8_t data);

/*******************************************************************************
* Function Name: print_buffer
********************************************************************************
* Summary:
* This function prints a buffer in hexadecimal format.
*
* Parameters:
* buf - pointer to the buffer to print
* len - length of the buffer
*
* Return:
* void
*
*******************************************************************************/
void print_buffer_hex(uint8_t *buf, uint32_t len)
{
    uint32_t i;
    Cy_USIC_CH_TXFIFO_Flush(CYBSP_DEBUG_UART_HW);


    for(i = 0 ; i < len; i++)
    {
        print_byte(buf[i]);
    }
    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)'\r');
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));
    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)'\n');
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));
}

/*******************************************************************************
* Function Name: print_buffer_text
********************************************************************************
* Summary:
* This function prints a buffer in ascii format.
*
* Parameters:
* buf - pointer to the buffer to print
* len - length of the buffer
*
* Return:
* void
*
*******************************************************************************/
void print_buffer_text(uint8_t *buf, uint32_t len)
{
    uint32_t i;
    Cy_USIC_CH_TXFIFO_Flush(CYBSP_DEBUG_UART_HW);

    for(i = 0 ; i < len; i++)
    {
        Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)buf[i]);
        while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));
    }
}

/*******************************************************************************
* Function Name: print_byte
********************************************************************************
* Summary:
* This function prints a buffer in hexadecimal format.
*
* Parameters:
* data - data byte to print
*
* Return:
* void
*
*******************************************************************************/
static void print_byte(uint8_t data)
{
    uint8_t tempVar;

    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)'0');
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));
    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)'x');
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));

    /* Print upper nibble first */
    tempVar = (data & 0xF0) >> 4;
    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)hex_to_ascii(tempVar));
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));

    tempVar = (data & 0x0F);
    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)hex_to_ascii(tempVar));
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));

    Cy_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, (uint16_t)' ');
    while((Cy_UART_CH_GetStatusFlag(CYBSP_DEBUG_UART_HW) & CY_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY));
}

/*******************************************************************************
* Function Name: hex_to_ascii
********************************************************************************
* Summary:
* This function converts a byte into ascii format.
*
* Parameters:
* data - data byte to convert to ascii
*
* Return:
* void
*
*******************************************************************************/
static uint8_t hex_to_ascii(uint8_t data)
{
    if(data < 10)
    {
        return (data + 48);
    }
    else
    {
        return (data + 55);
    }
}
/*CODE_BLOCK_END*/