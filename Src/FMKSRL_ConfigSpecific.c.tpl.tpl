/*********************************************************************
 * @file        FMKCPU_ConfigSpecific.c
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */






// ********************************************************************
// *                      Includes
// ********************************************************************
#include "./FMKSRL_ConfigSpecific.h"
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
//                      Local functions - Prototypes
//********************************************************************************

//********************************************************************************
//                      Public functions - Prototypes
//********************************************************************************
/*********************************
 * FMKSRL_HAL_UART_Receive
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Receive(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32) {
    return HAL_UART_Receive((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16, f_timeOut_u32);
}

/*********************************
 * FMKSRL_HAL_UART_Receive_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Receive_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_UART_Receive_IT((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_UART_Receive_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Receive_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_UART_Receive_DMA((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_USART_Receive
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Receive(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32) {
    return HAL_USART_Receive((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16, f_timeOut_u32);
}

/*********************************
 * FMKSRL_HAL_USART_Receive_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Receive_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_USART_Receive_IT((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_USART_Receive_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Receive_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_USART_Receive_DMA((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}
/*********************************
 * FMKSRL_HAL_UART_Transmit
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32) {
    return HAL_UART_Transmit((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16, f_timeOut_u32);
}

/*********************************
 * FMKSRL_HAL_UART_Transmit_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_UART_Transmit_IT((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_UART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Transmit_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_UART_Transmit_DMA((UART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16, uint32_t f_timeOut_u32) {
    return HAL_USART_Transmit((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16, f_timeOut_u32);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit_IT(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_USART_Transmit_IT((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Transmit_DMA(void *bspHandle_pv, uint8_t *f_data_pu8, uint16_t f_dataSize_u16) {
    return HAL_USART_Transmit_DMA((USART_HandleTypeDef *)bspHandle_pv, f_data_pu8, f_dataSize_u16);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Abort(void *bspHandle_pv) {
    return HAL_UART_Abort((UART_HandleTypeDef *)bspHandle_pv);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_Abort_IT(void *bspHandle_pv) {
    return HAL_UART_Abort((UART_HandleTypeDef *)bspHandle_pv);
}
/*********************************
 * FMKSRL_HAL_USART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Abort(void *bspHandle_pv) {
    return HAL_USART_Abort((USART_HandleTypeDef *)bspHandle_pv);
}

/*********************************
 * FMKSRL_HAL_USART_Transmit_DMA
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_USART_Abort_IT(void *bspHandle_pv) {
    return HAL_USART_Abort_IT((USART_HandleTypeDef *)bspHandle_pv);
}

/*********************************
 * FMKSRL_HAL_UART_AbortReceive
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_AbortReceive(void *bspHandle_pv) {
    return HAL_UART_AbortReceive((UART_HandleTypeDef *)bspHandle_pv);
}

/*********************************
 * FMKSRL_HAL_UART_AbortTransmit
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_AbortTransmit(void *bspHandle_pv) {
    return HAL_UART_AbortTransmit((UART_HandleTypeDef *)bspHandle_pv);
}
/*********************************
 * FMKSRL_HAL_UART_AbortReceive_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_AbortReceive_IT(void *bspHandle_pv) {
    return HAL_UART_AbortReceive_IT((UART_HandleTypeDef *)bspHandle_pv);
}
/*********************************
 * FMKSRL_HAL_UART_AbortTransmit_IT
 *********************************/
HAL_StatusTypeDef FMKSRL_HAL_UART_AbortTransmit_IT(void *bspHandle_pv) {
    return HAL_UART_AbortTransmit_IT((UART_HandleTypeDef *)bspHandle_pv);
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
           
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
