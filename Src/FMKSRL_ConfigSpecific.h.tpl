/*********************************************************************
 * @file        FMKCPU_ConfigSpecific.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKSRL_CONFIGSPECIFIC_H_INCLUDED
#define FMKSRL_CONFIGSPECIFIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKCPU_ConfigPublic.h"
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************

    // ********************************************************************
    // *                      Types
    // ********************************************************************
    //-----------------------------ENUM TYPES-----------------------------//
    /* CAUTION : Automatic generated code section for Enum: Start */

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------TYPEDEF TYPES---------------------------//
    //-----------------------------STRUCT TYPES---------------------------//
    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    


    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************
    HAL_StatusTypeDef FMKSRL_HAL_UART_Receive(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32);
    HAL_StatusTypeDef FMKSRL_HAL_UART_Receive_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_UART_Receive_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Receive(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Receive_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Receive_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);

    HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32);
    HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16);

    HAL_StatusTypeDef FMKSRL_HAL_UART_Abort(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_UART_Abort_IT(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Abort(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_USART_Abort_IT(void *bspHandle_pv);

    HAL_StatusTypeDef FMKSRL_HAL_UART_AbortReceive(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_UART_AbortTransmit(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_UART_AbortReceive_IT(void *bspHandle_pv);
    HAL_StatusTypeDef FMKSRL_HAL_UART_AbortTransmit_IT(void *bspHandle_pv);
#endif // FMKSRL_CONFIGSPECIFIC_H_INCLUDED           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@params[in] 
 *	@params[out]
 *	 
 *
 *
 */
