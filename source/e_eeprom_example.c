/******************************************************************************
* File Name:   e_eeprom_example.c
*
* Description: This Source file implements the example application to use 
* APIs in psc1_e_eeprom.c file function for PSOC™ Control C1 MCU. 
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

/*****************************************************************************
 * HEADER FILES
 ****************************************************************************/
#include "psc1_e_eeprom.h"
#include "e_eeprom_example.h"
#include "print_functions.h"

/*****************************************************************************
 * MACROS
 ****************************************************************************/
#define E_EEPROM_BLOCK_NUM     1U
#define E_EEPROM_BLOCK1_SIZE   64U
/* Macros for the messages to print */
#define E_EEPROM_EXISTING_DATA_STRING          "Existing data in EEPROM\r\n"
#define E_EEPROM_NEW_DATA_STRING               "New data in EEPROM\r\n"
#define E_EEPROM_INIT_ERROR_STRING             "EEPROM initialization failed\r\n"
#define E_EEPROM_READ_ERROR_STRING             "Failed to read data from EEPROM\r\n"
#define E_EEPROM_WRITE_ERROR_STRING            "Failed to write data to EEPROM\r\n"
#define E_EEPROM_INCONSISTENT_BLOCK_STRING     "E_EEPROM block is inconsistent, writing sample data (0x00)\r\n"

/*****************************************************************************
 * GLOBAL DATA
 ****************************************************************************/
/* Variable to flag if E_EEPROM block is inconsistent */
uint8_t E_EEPROM_inconsistent = 0U; /* 0U = Inconsistent, 1U = Consistent */
/* Structure to store user data block informations */
E_EEPROM_PSC1_CACHE_t block_info_buf[E_EEPROM_BLOCK_NUM];
E_EEPROM_PSC1_DATA_t  E_EEPROM_PSC1_config =
{
        .block_info = block_info_buf
};
/**
 *  User defined Data Block configurations
 */
const E_EEPROM_PSC1_BLOCK_t E_EEPROM_PSC1_block_Config[] =
{
    /* Block 1 Configuration */
    {1U, E_EEPROM_BLOCK1_SIZE}
};

/*
*  EMULATED_EEPROM handle structure definition
*/
E_EEPROM_PSC1_t E_EEPROM_PSC1_handle =
{
 .block_config_ptr        = (E_EEPROM_PSC1_BLOCK_t *)(void*)E_EEPROM_PSC1_block_Config, /* Pointer to user block configurations */
 .data_ptr                = &E_EEPROM_PSC1_config,                                      /* Pointer to the state variable data structure */
#ifdef E_EEPROM_PSC1_CRC_SW_ENABLED
 .crc_handle_ptr          = NULL,
#endif
 .state                   = E_EEPROM_PSC1_STATUS_UNINITIALIZED,  /* Current state of EEPROM */
 .block_count             = E_EEPROM_BLOCK_NUM,                  /* Number of EEPROM blocks to be configured */
 .erase_all_auto_recovery = 0U,                                  /* Erase Complete emulation area and recover to default state disabled */
 .data_block_crc          = 0U,                                  /* Data block CRC disabled */
 .garbage_collection      = 1U                                   /* Garbage collection enabled */
};

/*****************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ****************************************************************************/
static void prep_write_buffer(uint8_t *rbuf, uint8_t *wbuf, uint32_t len);
static void E_EEPROM_test(uint8_t blockNum, uint32_t block_size);

/*******************************************************************************
* Function Name: prep_write_buffer
********************************************************************************
* Summary:
* This function increments each element of read buffer and stores the new value
* in write buffer. It is done to update the contents in the flash.
*
* Parameters:
* rbuf - read buffer
* wbuf - write buffer
* len - lenght of buffers
*
* Return:
* void
*
*******************************************************************************/
static void prep_write_buffer(uint8_t *rbuf, uint8_t *wbuf, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
    {
        wbuf[i] = rbuf[i] + 1;
    }
}

/*******************************************************************************
* Function Name: E_EEPROM_test
********************************************************************************
* Summary:
* This is the example function to test e_eeprom in PSOC™ Control C1 MCU. It performs below 
* steps.
* 1 - Read data from an eeprom block
* 2 - increment the read data by 1 and program it back to eeprom
* 3 - Read and print new data from eeprom
*
* Parameters:
* blockNum - EEPROM Block number to test
* block_size - Size of the EEPROM block
*
* Return:
* E_EEPROM_PSC1_STATUS_t
*
*******************************************************************************/
static void E_EEPROM_test(uint8_t blockNum, uint32_t block_size)
{
    uint8_t data_wbuffer[block_size];
    uint8_t data_rbuffer[block_size];

    /* Write data if EEPROM is inconsistent */
    if(E_EEPROM_inconsistent != 1U)
    {
        print_buffer_text((uint8_t *)E_EEPROM_INCONSISTENT_BLOCK_STRING, sizeof(E_EEPROM_INCONSISTENT_BLOCK_STRING));
        memset(data_wbuffer, 0x00, block_size);
        if(E_EEPROM_PSC1_Write(blockNum, data_wbuffer) != E_EEPROM_PSC1_OPERATION_STATUS_SUCCESS)
        {
            /* Failed to write data to emulated EEPROM */
            print_buffer_text((uint8_t *)E_EEPROM_WRITE_ERROR_STRING, sizeof(E_EEPROM_WRITE_ERROR_STRING));
            return;
        }
    }

    /* Read and print existing data from emulated EEPROM */
    if(E_EEPROM_PSC1_Read(blockNum, 0U, data_rbuffer, block_size) != E_EEPROM_PSC1_OPERATION_STATUS_SUCCESS)
    {
        /* Failed to read data from emulated EEPORM */
        print_buffer_text((uint8_t *)E_EEPROM_READ_ERROR_STRING, sizeof(E_EEPROM_READ_ERROR_STRING));
        return;
    }
    print_buffer_text((uint8_t *)E_EEPROM_EXISTING_DATA_STRING, sizeof(E_EEPROM_EXISTING_DATA_STRING));
    print_buffer_hex(data_rbuffer, block_size);

    /* Write new data to emulated EEPROM */
    prep_write_buffer(data_rbuffer, data_wbuffer, block_size);
    if(E_EEPROM_PSC1_Write(blockNum, data_wbuffer) != E_EEPROM_PSC1_OPERATION_STATUS_SUCCESS)
    {
        /* Failed to write data to emulated EEPROM */
        print_buffer_text((uint8_t *)E_EEPROM_WRITE_ERROR_STRING, sizeof(E_EEPROM_WRITE_ERROR_STRING));
        return;
    }

    /* Read and print new data from emulated EEPROM */
    if(E_EEPROM_PSC1_Read(blockNum, 0U, data_rbuffer, block_size) != E_EEPROM_PSC1_OPERATION_STATUS_SUCCESS)
    {
        /* Failed to read data from emulated EEPORM */
        print_buffer_text((uint8_t *)E_EEPROM_READ_ERROR_STRING, sizeof(E_EEPROM_READ_ERROR_STRING));
        return;
    }
    print_buffer_text((uint8_t *)E_EEPROM_NEW_DATA_STRING, sizeof(E_EEPROM_NEW_DATA_STRING));
    print_buffer_hex(data_rbuffer, block_size);

}

/*******************************************************************************
* Function Name: E_EEPROM_erase
********************************************************************************
* Summary:
* Function to erase the EEPROM area.
*
* Parameters:
* void
*
* Return:
* void
*
*******************************************************************************/
void E_EEPROM_erase()
{
    Cy_FLASH_ErasePages( (uint32_t*)E_EEPROM_PSC1_FLASH_BANK0_BASE, (E_EEPROM_PSC1_BANK_PAGES*1));
    Cy_FLASH_ErasePages( (uint32_t*)E_EEPROM_PSC1_FLASH_BANK1_BASE, (E_EEPROM_PSC1_BANK_PAGES*1));
}

/*******************************************************************************
* Function Name: E_EEPROM_example
********************************************************************************
* Summary:
* Function to initialize eeprom and execute the tests.
*
* Parameters:
* void
*
* Return:
* void
*
*******************************************************************************/
void E_EEPROM_example()
{
    uint8_t i;

    /* Before starting, make sure that the flash shall not contain any data in the EEPROM area. Else erase the EEPROM
     * area using Cy_FLASH_ErasePages() API shown below. Enable the below commented code and build.
     * Once the example runs successfully, this line can be removed from future builds to ensure data retention. */
//    Cy_FLASH_ErasePages( (uint32_t*)E_EEPROM_PSC1_FLASH_BANK0_BASE, (E_EEPROM_PSC1_BANK_PAGES*1));
//    Cy_FLASH_ErasePages( (uint32_t*)E_EEPROM_PSC1_FLASH_BANK1_BASE, (E_EEPROM_PSC1_BANK_PAGES*1));

    /* Initialize emulated EEPROM */
    if(E_EEPROM_PSC1_Init(&E_EEPROM_PSC1_handle) != E_EEPROM_PSC1_STATUS_SUCCESS)
    {
        /* Emulated EEPROM initialization failed */
        print_buffer_text((uint8_t *)E_EEPROM_INIT_ERROR_STRING, sizeof(E_EEPROM_INIT_ERROR_STRING));
        return;
    }

    for(i = 1; i <= E_EEPROM_PSC1_handle.block_count; i++)
    {
        /* Update if the block is consistent or not */
        E_EEPROM_inconsistent = E_EEPROM_PSC1_handle.data_ptr->block_info[i-1].status.consistent;
        E_EEPROM_test(i, E_EEPROM_PSC1_handle.block_config_ptr[i-1].size);
    }
}

