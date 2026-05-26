/*********************************************************************
 * @file        FMK_SRL.h
 * @brief       UART/USART Module.
 * @details     This module allows user to Transmit and Receive messages
 *              using UART/USART protocol.\n
 *              Configuration Public File.\n
 *              
 *
 * @author      mba
 * @date        21/12/2024
 * @version     1.0
 */
  
  
#ifndef FMKSRL_CONFIGPUBLIC_H_INCLUDED
#define FMKSRL_CONFIGPUBLIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "./FMK_CFG/FMKCFG_ConfigFiles/FMKCPU_ConfigPublic.h"    
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    #define FMKSRL_TIMEOUT_RECEPTION ((t_uint32)60)

    ///@brief enable/disable debug/ enable it dev, and disable it for prod
    #define FMKSRL_DEBUG_UART_ENABLE            (M_TRUE)
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    //-----------------------------ENUM TYPES-----------------------------//
    
    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief Enum for Serial Line Available on CPU.
    */
    typedef enum
    {
        FMKSRL_SERIAL_LINE_1 = 0,                  /**< Reference to Hardware Instance USART1 */
        FMKSRL_SERIAL_LINE_2,                        /**< Reference to Hardware Instance USART2 */
        FMKSRL_SERIAL_LINE_3,                        /**< Reference to Hardware Instance USART3 */
        FMKSRL_SERIAL_LINE_4,                        /**< Reference to Hardware Instance UART4 */
        FMKSRL_SERIAL_LINE_5,                        /**< Reference to Hardware Instance UART5 */
    
        FMKSRL_SERIAL_LINE_NB,
    } t_eFMKSRL_SerialLine;

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------TYPEDEF TYPES---------------------------//
    
    //-----------------------------STRUCT TYPES---------------------------//
    /**
     * @brief Hardware Protocol Manage by the module
     */
    typedef enum __t_eFMKSRL_HwProtocolType
    {
        FMKSRL_HW_PROTOCOL_UART = 0x00,     /**< Reference to hardware protocol UART */
        FMKSRL_HW_PROTOCOL_USART,           /**< Reference to hardware protocol USART */
        FMKSRL_HW_PROTOCOL_NB,              /**< Number of hardware protocol UART */
    } t_eFMKSRL_HwProtocolType;
    
    /**
     * @brief Transmit/ Receive Actions Mode
     */
    typedef enum __t_eFMKSRL_LineRunMode
    {
        FMKSRL_LINE_RUNMODE_POLL = 0x00,    /**< Transmit Receive Actions are done in Polling Mode 
                                            /!\/!\ On Embeded System -> not recommended */
        FMKSRL_LINE_RUNMODE_IT,             /**< Transmit Receive Actions are done in Interrupt Mode 
                                            /!\/!\ On Embeded System -> Better than polling but use CPU charged */
        FMKSRL_LINE_RUNMODE_DMA,            /**< Transmit Receive Actions are done in DMA Mode 
                                            /!\/!\ On Embeded System -> Highly recommended */
        FMKSRL_LINE_RUNMODE_NB
    } t_eFMKSRL_LineRunMode;

    /**
     * @brief Enumeration of UART hardware flow control options.
     *
     * Defines the hardware flow control modes for UART peripherals.
     */
    typedef enum __t_eFMKSRL_UartHwFlowCtrl
    {
        FMKSRL_UART_HW_FLOW_CTRL_NONE = 0X00,   /**< No hardware flow control. */
        FMKSRL_UART_HW_FLOW_CTRL_RTS,           /**< RTS flow control enabled. */
        FMKSRL_UART_HW_FLOW_CTRL_CTS,           /**< CTS flow control enabled. */
        FMKSRL_UART_HW_FLOW_CTRL_RTS_CTS,       /**< Both RTS and CTS flow control enabled. */

        FMKSRL_UART_HW_FLOW_CTRL_NB             /**< Total number of hardware flow control options. */
    } t_eFMKSRL_UartHwFlowCtrl;

    /**
     * @brief Enumeration of UART/USART line word lengths.
     *
     * Defines the number of data bits in a transmission frame.
     */
    typedef enum __t_eFMKSRL_LineWordLenght
    {
        FMKSRL_LINE_WORDLEN_9BITS = 0x00,       /**< Word length of 9 bits. */
        FMKSRL_LINE_WORDLEN_8BITS,              /**< Word length of 8 bits. */
        FMKSRL_LINE_WORDLEN_7BITS,              /**< Word length of 7 bits. */
        FMKSRL_LINE_WORDLEN_NB                  /**< Total number of word length options. */
    } t_eFMKSRL_LineWordLenght;

    /**
     * @brief UART advanced protocol configuration placeholder.
     */
    typedef struct __t_sFMKSRL_UartAdvProtCfg
    {
        t_uint8 None1_u8;
        t_uint8 None2_u8;
    } t_sFMKSRL_UartAdvProtCfg;

    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
   
    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // FMKSRL_CONFIGPUBLIC_H_INCLUDED           
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
