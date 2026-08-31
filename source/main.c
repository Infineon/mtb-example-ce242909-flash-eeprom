/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C1 MCU: Flash EEPROM Example
*              for ModusToolbox. This code example shows how to use a portion 
*              of the internal flash memory of PSOC™ Control C1 MCUs as emulated EEPROM 
*              memory, and provides necessary APIs to perform read and write 
*              operations to the emulated EEPROM.
*              
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
#include "cy_utils.h"
#include "print_functions.h"
#include "e_eeprom_example.h"

/*******************************************************************************
* Defines
*******************************************************************************/
#define PROJECT_HEADER_COMMON_LINE  "\r\n**************************************************\r\n"
#define PROJECT_HEADER_NAME         "           FLASH EEPROM EXAMPLE APPLICATION"

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It initializes the BSP and runs the EEPROM example.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Print project name */
    print_buffer_text((uint8_t *)("\x1b[2J\x1b[;H"), sizeof("\x1b[2J\x1b[;H"));
    print_buffer_text((uint8_t *)PROJECT_HEADER_COMMON_LINE, sizeof(PROJECT_HEADER_COMMON_LINE));
    print_buffer_text((uint8_t *)PROJECT_HEADER_NAME, sizeof(PROJECT_HEADER_NAME));
    print_buffer_text((uint8_t *)PROJECT_HEADER_COMMON_LINE, sizeof(PROJECT_HEADER_COMMON_LINE));


    /* For PSOC™ Control C1 Devices:
     * Before starting, make sure that the flash shall not contain any data in
     * the EEPROM area. Else erase the EEPROM area using Cy_FLASH_ErasePages()
     *  API shown below. Enable the below commented code and build.
     *  Once the example runs successfully, this line can be commented
     * from future builds to ensure data retention.
     * */
    //E_EEPROM_erase();


    E_EEPROM_example();
    for (;;)
    {
    }
}

/* [] END OF FILE */
