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
 * FMKSRL_IsRxTimeoutOpeSupported
 *********************************/
t_bool FMKSRL_IsRxTimeoutOpeSupported(void)
{
    t_bool isSupported_b = (t_bool)FALSE;

#ifdef FMKCPU_STM32_ECU_FAMILY_G4
    isSupported_b = (t_bool)TRUE;
#endif

    return isSupported_b;
}

/*********************************
 * FMKSRL_Set_UartSpecificInitCfg
 *********************************/
t_eReturnCode FMKSRL_Set_UartSpecificInitCfg(  UART_HandleTypeDef *f_bspHandle_ps,
                                                t_eFMKSRL_UartHwFlowCtrl f_hwFlowCtrl_e,
                                                t_sFMKSRL_UartAdvProtCfg *f_softAdvCfg_ps)
{
    t_eReturnCode Ret_e = RC_OK;

    if((f_bspHandle_ps == (UART_HandleTypeDef *)NULL)
    || (f_softAdvCfg_ps == (t_sFMKSRL_UartAdvProtCfg *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    else
    {
#ifdef FMKCPU_STM32_ECU_FAMILY_G4
        switch(f_hwFlowCtrl_e)
        {
            case FMKSRL_UART_HW_FLOW_CTRL_NONE:
                f_bspHandle_ps->Init.HwFlowCtl = UART_HWCONTROL_NONE;
                break;
            case FMKSRL_UART_HW_FLOW_CTRL_RTS:
                f_bspHandle_ps->Init.HwFlowCtl = UART_HWCONTROL_RTS;
                break;
            case FMKSRL_UART_HW_FLOW_CTRL_CTS:
                f_bspHandle_ps->Init.HwFlowCtl = UART_HWCONTROL_CTS;
                break;
            case FMKSRL_UART_HW_FLOW_CTRL_RTS_CTS:
                f_bspHandle_ps->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
                break;
            case FMKSRL_UART_HW_FLOW_CTRL_NB:
            default:
                Ret_e = RC_ERROR_PARAM_INVALID;
                break;
        }

        if(Ret_e == RC_OK)
        {
            (void)f_softAdvCfg_ps;
            f_bspHandle_ps->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        }
#else
        (void)f_hwFlowCtrl_e;
        (void)f_softAdvCfg_ps;
#endif
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_Set_UartSpecificPostInitCfg
 *********************************/
t_eReturnCode FMKSRL_Set_UartSpecificPostInitCfg(UART_HandleTypeDef *f_bspHandle_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    (void)bspRet_e;

    if(f_bspHandle_ps == (UART_HandleTypeDef *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    else
    {
#ifdef FMKCPU_STM32_ECU_FAMILY_G4
        bspRet_e = HAL_UARTEx_DisableFifoMode(f_bspHandle_ps);

        if(bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
#endif
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_Get_BspWordLength
 *********************************/
t_eReturnCode FMKSRL_Get_BspWordLength(    t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                            t_eFMKSRL_LineWordLenght f_lineWordLenght_e,
                                            t_uint32 *f_bspLineWordLenght_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if((f_HwProtUsed_e >= FMKSRL_HW_PROTOCOL_NB)
    || (f_lineWordLenght_e >= FMKSRL_LINE_WORDLEN_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspLineWordLenght_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch(f_lineWordLenght_e)
        {
            case FMKSRL_LINE_WORDLEN_9BITS:
                if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineWordLenght_pu32 = (t_uint32)USART_WORDLENGTH_9B;
                }
                else if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineWordLenght_pu32 = (t_uint32)UART_WORDLENGTH_9B;
                }
                else
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_WORDLEN_8BITS:
                if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineWordLenght_pu32 = (t_uint32)UART_WORDLENGTH_8B;
                }
                else if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineWordLenght_pu32 = (t_uint32)USART_WORDLENGTH_8B;
                }
                else
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_WORDLEN_7BITS:
                Ret_e = FMKSRL_Get_Bsp7BitsWordLength(f_HwProtUsed_e, f_bspLineWordLenght_pu32);
                break;

            case FMKSRL_LINE_WORDLEN_NB:
            default:
                Ret_e = RC_ERROR_PARAM_INVALID;
                break;
        }
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_Get_Bsp7BitsWordLength
 *********************************/
t_eReturnCode FMKSRL_Get_Bsp7BitsWordLength(   t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_uint32 *f_bspLineWordLenght_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_bspLineWordLenght_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    else
    {
#ifdef FMKCPU_STM32_ECU_FAMILY_G4
        if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
        {
            *f_bspLineWordLenght_pu32 = (t_uint32)UART_WORDLENGTH_7B;
        }
        else if(f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
        {
            *f_bspLineWordLenght_pu32 = (t_uint32)USART_WORDLENGTH_7B;
        }
        else
        {
            Ret_e = RC_ERROR_NOT_SUPPORTED;
        }
#else
        (void)f_HwProtUsed_e;
        Ret_e = RC_ERROR_NOT_SUPPORTED;
#endif
    }

    return Ret_e;
}

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
