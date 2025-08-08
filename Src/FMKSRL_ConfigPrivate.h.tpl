/*********************************************************************
 * @file        FMK_SRL.h
 * @brief       UART/USART Module.
 * @details     This module allows user to Transmit and Receive messages
 *              using UART/USART protocol.\n
 *              Configuration Private File.\n
 *              
 *
 * @author      mba
 * @date        21/12/2024
 * @version     1.0
 */
  
#ifndef FMKSRL_CONFIGPRIVATE_H_INCLUDED
#define FMKSRL_CONFIGPRIVATE_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "./FMKSRL_ConfigPublic.h"
    #include "FMK_CFG/FMKCFG_ConfigSpecific/FMKSRL_ConfigSpecific.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    #define FMKSRL_TIMEOUT_POLLING ((t_uint32)0x0000FFFF)

    #define FMKSRL_MAX_BYTES_TO_SEND ((t_uint16)512)

    #define FMKSRL_MAX_ERR_CNT       ((t_uint8)10)

    /// buffer logger sier
    #define FMKSRL_UART_BUFFER_SIZE     ((t_uint16)256)
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    //-----------------------------ENUM TYPES-----------------------------//
    /**
     * @brief Bsp Transmission Operation
     */
    typedef enum __t_eFMKSRL_BspTransmitOpe
    {
        FMKSRL_BSP_TX_OPE_TRANSMIT = 0x00,      /**< Bsp Transmission Operation */
        FMKSRL_BSP_TX_OPE_TRANSMIT_RECEIVE,     /**< Bsp Transmission/Reception Operation */

        FMKSRL_BSP_TX_OPE_NB                    /**< Bsp Transmission Operation Number */
    } t_eFMKSRL_BspTransmitOpe;

    /**
     * @brief Bsp Reception Operation
     */
    typedef enum __t_eFMKSRL_BspReceiveOpe
    {
        FMKSRL_BSP_RX_OPE_RECEIVE = 0x00,       /**< Bsp Reception Size Operation */
        FMKSRL_BSP_RX_OPE_RECEIVE_IDLE,         /**< Bsp Reception Idle Operation */
#ifdef FMKCPU_STM32_ECU_FAMILY_G
        FMKSRL_BSP_RX_OPE_RECEIVE_TIMEOUT,      /**< Bsp Reception Timeout Operation */
#endif
        FMKSRL_BSP_RX_OPE_NB                    /**< Bsp Reception Operation Number */
    } t_eFMKSRL_BspReceiveOpe;

    /**
     * @brief Bsp Abort Operation
     */
    typedef enum __t_eFMKSRL_BspAbortOpe
    {
        FMKSRL_OPE_ABORT_RECEPTION = 0x00,       /**< Bsp Abort Operation : Reception */
        FMKSRL_OPE_ABORT_TRANSMISSION,           /**< Bsp Abort Operation : Transmssion */
        FMKSRL_OPE_ABORT_BOTH,                   /**< Bsp Abort Operation : Reception/Transmssion */

        FMKSRL_OPE_ABORT_NB                     /**< Bsp Abort Operation Number */
    } t_eFMKSRL_BspAbortOpe;
    /* CAUTION : Automatic generated code section for Enum: Start */

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------TYPEDEF TYPES---------------------------//
    /**
     * @brief Serial Line Configuration.
     */
    typedef struct 
    {
        const t_eFMKCPU_ClockPort           c_clockPort_e;
        const t_eFMKCPU_IRQNType            c_IRQNType_e;
        const t_eFMKSRL_HwProtocolType      c_HwType_e;
        const t_eFMKCPU_DmaRqst             c_DmaRqstRx;
        const t_eFMKCPU_DmaRqst             c_DmaRqstTx;
        t_uint8 * Rx_StartAddressBuffer_pu8;
        t_uint16 Rx_bufferSize_u16;
        t_uint8 * Tx_StartAddressBuffer_pu8;
        t_uint16 Tx_bufferSize_u16;
    } t_sFMKSRL_SerialCfg;
    /**
    *
    *	@brief      HAL UART.USART function to Receive/ TRansmit in Polling Mode
    *               is used in blocking mode 
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *	@param[out]     f_data_pu8          : the pointer to the data
    *	@param[out]     f_dataSize_u16      : the data size
    *	@param[out]     f_timeOut_u32       : tiume out in polling mode
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_BspRxTx_Poll)(   void * bspHandle_pv, 
                                                    t_uint8 *f_data_pu8, 
                                                    t_uint16 f_dataSize_u16,
                                                    t_uint32 f_timeOut_u32);

    /**
    *
    *	@brief      HAL UART.USART function to Receive/ TRansmit in Polling Mode
    *               is used in blocking mode 
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *	@param[out]     f_data_pu8          : the pointer to the data
    *	@param[out]     f_dataSize_u16      : the data size
    *	@param[out]     f_timeOut_u32       : tiume out in polling mode
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */

    /**
    *
    *	@brief      HAL UART.USART function to Receive/ TRansmit in Interrupt Mode
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *	@param[out]     f_data_pu8          : the pointer to the data
    *	@param[out]     f_dataSize_u16      : the data size
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_BspRxTx_IT)(   void * bspHandle_pv, 
                                                    t_uint8 *f_data_pu8, 
                                                    t_uint16 f_dataSize_u16);

    /**
    *
    *	@brief      HAL UART.USART function to Receive/ TRansmit in Dma Mode 
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *	@param[out]     f_data_pu8          : the pointer to the data
    *	@param[out]     f_dataSize_u16      : the data size
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_BspRxTx_DMA)(   void * bspHandle_pv, 
                                                    t_uint8 *f_data_pu8, 
                                                    t_uint16 f_dataSize_u16);

    /**
    *
    *	@brief      HAL UART.USART function Abort Polling Mode
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_BspAbort)(void * bspHandle_pv);
    /**
    *
    *	@brief      HAL UART.USART function Abort Interrupt Mode
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_BspAbort_IT)(void * bspHandle_pv);

    /**
    *
    *	@brief      HAL UART.USART function Abort Interrupt Mode
    *
    *	@param[in]      bspHandle_pv        : the UART/USART Handle_TypeDef
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKSRL_GetError)(void * bspHandle_pv);
    //-----------------------------STRUCT TYPES---------------------------//
    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */


    /**
     * @brief Bsp transmit Receive Function use in UART/USART Protocol.
     */
    typedef struct __t_sFMKSRL_BspRxTxFunc
    {
        t_cbFMKSRL_BspRxTx_Poll *       bspRxTxPoll_pcb;
        t_cbFMKSRL_BspRxTx_IT   *       bspRxTxIT_pcb;
        t_cbFMKSRL_BspRxTx_DMA  *       bspRxTxDMA_pcb;
    } t_sFMKSRL_BspRxTxFunc;

    /**
     * @brief Bsp Abort Function use in UART/USART.
     */
    typedef struct __t_sFMKSRL_BspAbortFunc
    {
        t_cbFMKSRL_BspAbort    *       bspAbortPoll_pcb;
        t_cbFMKSRL_BspAbort_IT *       bspAbortIT_pcb;
        t_cbFMKSRL_BspAbort_IT *       bspAbortDMA_pcb;
    } t_sFMKSRL_BspAbortFunc;
    
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    t_char g_UartBufferLog_uac[FMKSRL_UART_BUFFER_SIZE];
    /* CAUTION : Automatic generated code section for Variable: Start */
    //--------- Tx, Rx Buffer for Serial Line 1 ---------//
    t_uint8 g_SrlLine_1_RxBuffer_ua8[256];
    t_uint8 g_SrlLine_1_TxBuffer_ua8[256];

    //--------- Tx, Rx Buffer for Serial Line 2 ---------//
    t_uint8 g_SrlLine_2_RxBuffer_ua8[256];
    t_uint8 g_SrlLine_2_TxBuffer_ua8[1024];

    //--------- Tx, Rx Buffer for Serial Line 3 ---------//
    t_uint8 g_SrlLine_3_RxBuffer_ua8[0];
    t_uint8 g_SrlLine_3_TxBuffer_ua8[0];

    //--------- Tx, Rx Buffer for Serial Line 4 ---------//
    t_uint8 g_SrlLine_4_RxBuffer_ua8[0];
    t_uint8 g_SrlLine_4_TxBuffer_ua8[0];

    //--------- Tx, Rx Buffer for Serial Line 5 ---------//
    t_uint8 g_SrlLine_5_RxBuffer_ua8[0];
    t_uint8 g_SrlLine_5_TxBuffer_ua8[0];

    /**< Store the Serial Configuration */
    const t_sFMKSRL_SerialCfg c_FmkSrl_SerialCfg_as[FMKSRL_SERIAL_LINE_NB] = {
        [FMKSRL_SERIAL_LINE_1] = {
            .c_clockPort_e = FMKCPU_RCC_CLK_USART1,
            .c_HwType_e    = FMKSRL_HW_PROTOCOL_USART,
            .c_IRQNType_e  = FMKCPU_NVIC_USART1_IRQN,
            .c_DmaRqstRx   = FMKCPU_DMA_RQSTYPE_USART1_RX,
            .c_DmaRqstTx   = FMKCPU_DMA_RQSTYPE_USART1_TX,
            .Rx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_1_RxBuffer_ua8[0]),
            .Rx_bufferSize_u16 = (t_uint16)256,
            .Tx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_1_TxBuffer_ua8[0]),
            .Tx_bufferSize_u16 = (t_uint16)256,
        },
        [FMKSRL_SERIAL_LINE_2] = {
            .c_clockPort_e = FMKCPU_RCC_CLK_USART2,
            .c_HwType_e    = FMKSRL_HW_PROTOCOL_USART,
            .c_IRQNType_e  = FMKCPU_NVIC_USART2_IRQN,
            .c_DmaRqstRx   = FMKCPU_DMA_RQSTYPE_USART2_RX,
            .c_DmaRqstTx   = FMKCPU_DMA_RQSTYPE_USART2_TX,
            .Rx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_2_RxBuffer_ua8[0]),
            .Rx_bufferSize_u16 = (t_uint16)256,
            .Tx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_2_TxBuffer_ua8[0]),
            .Tx_bufferSize_u16 = (t_uint16)1024,
        },
        [FMKSRL_SERIAL_LINE_3] = {
            .c_clockPort_e = FMKCPU_RCC_CLK_USART3,
            .c_HwType_e    = FMKSRL_HW_PROTOCOL_USART,
            .c_IRQNType_e  = FMKCPU_NVIC_USART3_IRQN,
            .c_DmaRqstRx   = (t_eFMKCPU_DmaRqst)0xFF,
            .c_DmaRqstTx   = (t_eFMKCPU_DmaRqst)0xFF,
            .Rx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_3_RxBuffer_ua8[0]),
            .Rx_bufferSize_u16 = (t_uint16)0,
            .Tx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_3_TxBuffer_ua8[0]),
            .Tx_bufferSize_u16 = (t_uint16)0,
        },
        [FMKSRL_SERIAL_LINE_4] = {
            .c_clockPort_e = FMKCPU_RCC_CLK_UART4,
            .c_HwType_e    = FMKSRL_HW_PROTOCOL_UART,
            .c_IRQNType_e  = FMKCPU_NVIC_UART4_IRQN,
            .c_DmaRqstRx   = (t_eFMKCPU_DmaRqst)0xFF,
            .c_DmaRqstTx   = (t_eFMKCPU_DmaRqst)0xFF,
            .Rx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_4_RxBuffer_ua8[0]),
            .Rx_bufferSize_u16 = (t_uint16)0,
            .Tx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_4_TxBuffer_ua8[0]),
            .Tx_bufferSize_u16 = (t_uint16)0,
        },
        [FMKSRL_SERIAL_LINE_5] = {
            .c_clockPort_e = FMKCPU_RCC_CLK_UART5,
            .c_HwType_e    = FMKSRL_HW_PROTOCOL_UART,
            .c_IRQNType_e  = FMKCPU_NVIC_UART5_IRQN,
            .c_DmaRqstRx   = (t_eFMKCPU_DmaRqst)0xFF,
            .c_DmaRqstTx   = (t_eFMKCPU_DmaRqst)0xFF,
            .Rx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_5_RxBuffer_ua8[0]),
            .Rx_bufferSize_u16 = (t_uint16)0,
            .Tx_StartAddressBuffer_pu8 = (t_uint8 *)(&g_SrlLine_5_TxBuffer_ua8[0]),
            .Tx_bufferSize_u16 = (t_uint16)0,
        },
    };
    /**
    * @brief Mapping between Serial Line And Bsp Handle Typedef
    */
    USART_TypeDef * c_FmkSrl_BspInitIstcMapp_pas[FMKSRL_SERIAL_LINE_NB] = {
        USART1,                         // Reference to Serial Line 1 
        USART2,                         // Reference to Serial Line 2 
        USART3,                         // Reference to Serial Line 3 
        UART4,                          // Reference to Serial Line 4 
        UART5,                          // Reference to Serial Line 5 
    };

    /* CAUTION : Automatic generated code section for Variable: End */

    /**
     * @brief Bsp UART/USART Receive Function
     */
    const t_sFMKSRL_BspRxTxFunc c_FmkSrl_RxBspFunc_apf[FMKSRL_HW_PROTOCOL_NB] = {
        {FMKSRL_HAL_UART_Receive,      FMKSRL_HAL_UART_Receive_IT ,        FMKSRL_HAL_UART_Receive_DMA},   // FMKSRL_HW_PROTOCOL_UART
        {FMKSRL_HAL_USART_Receive,     FMKSRL_HAL_USART_Receive_IT,        FMKSRL_HAL_USART_Receive_DMA},  // FMKSRL_HW_PROTOCOL_USART
        
    };

    /**
     * @brief Bsp UART/USART Transmit Function
     */
    const t_sFMKSRL_BspRxTxFunc c_FmkSrl_TxBspFunc_apf[FMKSRL_HW_PROTOCOL_NB] = {
        {FMKSRL_HAL_UART_Transmit,      FMKSRL_HAL_UART_Transmit_IT ,        FMKSRL_HAL_UART_Transmit_DMA},   // FMKSRL_HW_PROTOCOL_UART
        {FMKSRL_HAL_USART_Transmit,     FMKSRL_HAL_USART_Transmit_IT,        FMKSRL_HAL_USART_Transmit_DMA},  // FMKSRL_HW_PROTOCOL_USART
    }; 

    /**
     * @brief Bsp UART/USART Abort Function 
     */   
    const t_sFMKSRL_BspAbortFunc c_FmkSrl_AbortBspFunc_apf[FMKSRL_HW_PROTOCOL_NB] = {
        {FMKSRL_HAL_UART_Abort,        FMKSRL_HAL_UART_Abort_IT,      FMKSRL_HAL_UART_Abort_IT},         // FMKSRL_HW_PROTOCOL_UART    
        {FMKSRL_HAL_USART_Abort,       FMKSRL_HAL_USART_Abort_IT,     FMKSRL_HAL_USART_Abort_IT},        // FMKSRL_HW_PROTOCOL_USART
    };

    /**
     * @brief Bsp Abort Receive for UART
     */
    const t_sFMKSRL_BspAbortFunc c_FmkSrl_AbortRxBspFunc_apf[FMKSRL_HW_PROTOCOL_NB] = {
        {FMKSRL_HAL_UART_AbortReceive,              FMKSRL_HAL_UART_AbortReceive_IT,         FMKSRL_HAL_UART_AbortReceive_IT},   // FMKSRL_HW_PROTOCOL_UART    
        {NULL_FUNCTION,                             NULL_FUNCTION,                           NULL_FUNCTION},                     // FMKSRL_HW_PROTOCOL_USART
    };

    /**
     * @brief Bsp Abort Transmit for UART
     */
    const t_sFMKSRL_BspAbortFunc c_FmkSrl_AbortTxBspFunc_apf[FMKSRL_HW_PROTOCOL_NB] = {
        {FMKSRL_HAL_UART_AbortTransmit,             FMKSRL_HAL_UART_AbortTransmit_IT,         FMKSRL_HAL_UART_AbortTransmit_IT},   // FMKSRL_HW_PROTOCOL_UART    
        {NULL_FUNCTION,                             NULL_FUNCTION,                            NULL_FUNCTION},                      // FMKSRL_HW_PROTOCOL_USART
    };

    /***********************************************************
     * USART CALLBACK MANAGEMENT
     **********************************************************/
    /* CAUTION : Automatic generated code section for UART/USART IRQHandler: Start */
    void USART1_IRQHandler(void)
    {
        UART_HandleTypeDef * uartHandle_ps = NULL;
        USART_HandleTypeDef * usartHandle_ps = NULL;

        FMKSRL_PRIVATE_GetHandleTypeDef(FMKSRL_SERIAL_LINE_1, &uartHandle_ps, &usartHandle_ps);

        if(uartHandle_ps != (UART_HandleTypeDef *)NULL)
        {
            HAL_UART_IRQHandler((UART_HandleTypeDef *)(uartHandle_ps));
        }
        else if(usartHandle_ps != (USART_HandleTypeDef *)NULL)
        {
            HAL_USART_IRQHandler((USART_HandleTypeDef *)(usartHandle_ps));
        }
    }

    void USART2_IRQHandler(void)
    {
        UART_HandleTypeDef * uartHandle_ps = NULL;
        USART_HandleTypeDef * usartHandle_ps = NULL;

        FMKSRL_PRIVATE_GetHandleTypeDef(FMKSRL_SERIAL_LINE_2, &uartHandle_ps, &usartHandle_ps);

        if(uartHandle_ps != (UART_HandleTypeDef *)NULL)
        {
            HAL_UART_IRQHandler((UART_HandleTypeDef *)(uartHandle_ps));
        }
        else if(usartHandle_ps != (USART_HandleTypeDef *)NULL)
        {
            HAL_USART_IRQHandler((USART_HandleTypeDef *)(usartHandle_ps));
        }
    }

    void USART3_IRQHandler(void)
    {
        UART_HandleTypeDef * uartHandle_ps = NULL;
        USART_HandleTypeDef * usartHandle_ps = NULL;

        FMKSRL_PRIVATE_GetHandleTypeDef(FMKSRL_SERIAL_LINE_3, &uartHandle_ps, &usartHandle_ps);

        if(uartHandle_ps != (UART_HandleTypeDef *)NULL)
        {
            HAL_UART_IRQHandler((UART_HandleTypeDef *)(uartHandle_ps));
        }
        else if(usartHandle_ps != (USART_HandleTypeDef *)NULL)
        {
            HAL_USART_IRQHandler((USART_HandleTypeDef *)(usartHandle_ps));
        }
    }

    void UART4_IRQHandler(void)
    {
        UART_HandleTypeDef * uartHandle_ps = NULL;
        USART_HandleTypeDef * usartHandle_ps = NULL;

        FMKSRL_PRIVATE_GetHandleTypeDef(FMKSRL_SERIAL_LINE_4, &uartHandle_ps, &usartHandle_ps);

        if(uartHandle_ps != (UART_HandleTypeDef *)NULL)
        {
            HAL_UART_IRQHandler((UART_HandleTypeDef *)(uartHandle_ps));
        }
        else if(usartHandle_ps != (USART_HandleTypeDef *)NULL)
        {
            HAL_USART_IRQHandler((USART_HandleTypeDef *)(usartHandle_ps));
        }
    }

    void UART5_IRQHandler(void)
    {
        UART_HandleTypeDef * uartHandle_ps = NULL;
        USART_HandleTypeDef * usartHandle_ps = NULL;

        FMKSRL_PRIVATE_GetHandleTypeDef(FMKSRL_SERIAL_LINE_5, &uartHandle_ps, &usartHandle_ps);

        if(uartHandle_ps != (UART_HandleTypeDef *)NULL)
        {
            HAL_UART_IRQHandler((UART_HandleTypeDef *)(uartHandle_ps));
        }
        else if(usartHandle_ps != (USART_HandleTypeDef *)NULL)
        {
            HAL_USART_IRQHandler((USART_HandleTypeDef *)(usartHandle_ps));
        }
    }

    /* CAUTION : Automatic generated code section for UART/USART IRQHandler: End */
    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // FMKSRL_CONFIGPRIVATE_H_INCLUDED           
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
