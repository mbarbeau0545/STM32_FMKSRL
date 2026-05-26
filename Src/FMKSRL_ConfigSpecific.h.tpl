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
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKSRL_ConfigPublic.h"
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
#ifdef FMKCPU_STM32_ECU_FAMILY_G4
        ///@brief Debug serial line
        #define FMKSRL_DEBUG_SERIAL_LINE_IDX ((t_uint8)FMKSRL_SERIAL_LINE_2)
    #elif defined FMKCPU_STM32_ECU_FAMILY_H7
        ///@brief Debug serial line
        #define FMKSRL_DEBUG_SERIAL_LINE_IDX ((t_uint8)FMKSRL_SERIAL_LINE_3)
    #endif

    ///@brief Backward-compatible debug serial line mapping
    #define FMKSRL_DEBUG_SERIAL_LINE ((t_eFMKSRL_SerialLine)FMKSRL_DEBUG_SERIAL_LINE_IDX)
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
    t_bool FMKSRL_IsRxTimeoutOpeSupported(void);
    t_eReturnCode FMKSRL_Set_UartSpecificInitCfg(  UART_HandleTypeDef *f_bspHandle_ps,
                                                    t_eFMKSRL_UartHwFlowCtrl f_hwFlowCtrl_e,
                                                    t_sFMKSRL_UartAdvProtCfg *f_softAdvCfg_ps);
    t_eReturnCode FMKSRL_Set_UartSpecificPostInitCfg(UART_HandleTypeDef *f_bspHandle_ps);
    t_eReturnCode FMKSRL_Get_BspWordLength(    t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_eFMKSRL_LineWordLenght f_lineWordLenght_e,
                                                t_uint32 *f_bspLineWordLenght_pu32);
    t_eReturnCode FMKSRL_Get_Bsp7BitsWordLength(   t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                    t_uint32 *f_bspLineWordLenght_pu32);
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
