/*********************************************************************
 * @file        FMK_SRL.h
 * @brief       UART/USART Module.
 * @details     This module allows user to Transmit and Receive messages
 *              using UART/USART protocol.\n
 *              
 *
 * @author      mba
 * @date        21/12/2024
 * @version     1.0
 */
  
#ifndef FMKSERIAL_INCLUDED
#define FMKSERIAL_INCLUDED

    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "./FMK_CFG/FMKCFG_ConfigFiles/FMKSRL_ConfigPublic.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    
    // ********************************************************************
    // *                      Types
    // ********************************************************************
	/* CAUTION : Automatic generated code section for Enum: Start */

	/* CAUTION : Automatic generated code section for Enum: End */
	//-----------------------------ENUM TYPES-----------------------------//
    /**
     * @brief Receive Operation Available in Module
     */
    typedef enum __t_eFMKSRL_RxOpeMode
    {
        FMKSRL_OPE_RX_ONESHOT_SIZE = 0x00,      /**< Receive one msg, trigger for calling user is the size. */
        FMKSRL_OPE_RX_ONESHOT_IDLE,             /**< Receive one msg, trigger for calling user is quiet again after starting receive msg.
                                                        It appears that this mode increase CPU load by making a lot of interruption, be aware of that */

        FMKSRL_OPE_RX_CYCLIC_SIZE,              /**< Receive Cyclic Msg,  trigger for calling user is the size.
                                                    This mode is useful for Drivers (GPS, etc). */

        FMKSRL_OPE_RX_CYCLIC_IDLE,              /**< Receive Cyclic Msg, trigger for calling user is quiet again after starting receive msg.
                                                    This mode is useful for Drivers (GPS, etc). */

#ifdef FMKCPU_STM32_ECU_FAMILY_G
        FMKSRL_OPE_RX_ONESHOT_TIMEOUT,          /**< Receive one msg,  trigger for calling user when the line is
                                                        quiet for the amount of time indicate by user in millisecond.\n */
                                                        
        FMKSRL_OPE_RX_CYCLIC_TIMEOUT,           /**< Receive one msg,  trigger for calling user when the line is
                                                        quiet for the amount of time indicates by user in millisecond.\n 
                                                        This mode is useful for Drivers (GPS, etc).\n */
#endif

        FMKSRL_OPE_RX_NB,                       /**< Number of Rx Operation */
    } t_eFMKSRL_RxOpeMode;

    /**
     * @brief Transmit Operation Available in Module
     */
    typedef enum __t_eFMKSRL_TxOpeMode
    {
        FMKSRL_TX_ONESHOT = 0x00,               /**< Transmit one Message */
        FMKSRL_TX_RX_SIZE,                      /**< Transmit one Message and configure the Rx line to receive a 
                                                        message, user will be called when x bytes will be received.\n
                                                        This mode is useful for Drivers using AT CMD for instance */

        FMKSRL_TX_RX_IDLE,                      /**< Transmit one Message and configure the Rx line to receive a 
                                                        message, user will be called when the line will be quiet again after starting receive msg.
                                                        This mode is useful for Drivers using AT CMD for instance. */

#ifdef FMKCPU_STM32_ECU_FAMILY_G
        FMKSRL_TX_RX_TIMEOUT,                   /**< Transmit one Message and configure the Rx line to receive a 
                                                        message, user will be called when the line is quiet for x millisecond, x will be indeicate by user.
                                                        This mode is useful for Drivers using AT CMD for instance. */
#endif

        FMKSRL_TX_NB                            /**< Number of Transmit Operation  */
    } t_eFMKSRL_TxOpeMode;

    /**
     * @brief Mode Avaialable in this Module using UART Protocol
     */
/**
 * @brief Enumeration of UART types.
 *
 * Specifies the available operational modes for UART peripherals.
 */
typedef enum __t_eFMKSRL_UartType
{
    FMKSRL_UART_TYPECFG_UART = 0x00,        /**< Standard UART operation mode. */
    FMKSRL_UART_TYPECFG_HALF_DUPLEX,        /**< UART in half-duplex communication mode. */
    FMKSRL_UART_TYPECFG_LIN,                /**< UART in LIN protocol communication mode. */
    FMKSRL_UART_TYPECFG_MULTI_PROCESS,      /**< UART in multi-processor communication mode. */

    FMKSRL_UART_TYPECFG_NB                  /**< Total number of UART operation types. */
} t_eFMKSRL_UartType;

/**
 * @brief Enumeration of available USART modes.
 *
 * Specifies the operational modes for USART peripherals.
 */
typedef enum __t_eFMKSRL_UsartTypeCfg
{
    FMKSRL_USART_TYPECFG_USART = 0x00,      /**< Standard USART operation mode. */

    FMKSRL_USART_TYPECFG_NB                 /**< Total number of USART operation types. */
} t_eFMKSRL_UsartType;

/**
 * @brief Enumeration of USART clock phases.
 *
 * Determines which clock transition is used to sample data.
 */
typedef enum __t_eFMKSRL_UsartClockPhase
{
    FMKSRL_USART_PHASE_1EDGE = 0x00,        /**< Data sampled on the first clock transition. */
    FMKSRL_USART_PHASE_2EDGE,              /**< Data sampled on the second clock transition. */

    FMKSRL_USART_PHASE_NB                   /**< Total number of USART clock phase options. */
} t_eFMKSRL_UsartClockPhase;

/**
 * @brief Enumeration of USART last bit modes.
 *
 * Specifies whether the last data bit (10th or 9th) is output in synchronous mode.
 */
typedef enum __t_eFMKSRL_UsartLastBit
{
    FMKSRL_USART_LAST_BIT_ENABLE = 0x00,    /**< Last data bit is output. */
    FMKSRL_USART_LAST_BIT_DISABLE,         /**< Last data bit is not output. */

    FMKSRL_USART_LAST_BIT_NB                /**< Total number of USART last bit options. */
} t_eFMKSRL_UsartLastBit;

/**
 * @brief Enumeration of USART clock polarity.
 *
 * Specifies the idle state of the clock signal.
 */
typedef enum __t_eFMKSRL_UsartClkPolarity
{
    FMKSRL_USART_CLK_POLARITY_HIGH = 0x00,  /**< Clock is high when idle. */
    FMKSRL_USART_CLK_POLARITY_LOW,         /**< Clock is low when idle. */

    FMKSRL_USART_CLK_POLARITY_NB           /**< Total number of USART clock polarity options. */
} t_eFMKSRL_UsartClkPolarity;

/**
 * @brief Enumeration of LIN break lengths.
 *
 * Defines the length of the break field in LIN mode.
 */
typedef enum __t_eFMKSRL_LinBreakLenght
{
    FMKSRL_LIN_BREAK_LENGHT_10B = 0x00,     /**< Break field length of 10 bits. */
    FMKSRL_LIN_BREAK_LENGHT_11B,           /**< Break field length of 11 bits. */

    FMKSRL_LIN_BREAK_LENGHT_NB             /**< Total number of LIN break length options. */
} t_eFMKSRL_LinBreakLenght;

/**
 * @brief Enumeration of multi-process wake-up methods.
 *
 * Specifies how the multi-processor communication mode wakes up.
 */
typedef enum __t_eFMKSRL_MProcessWakeUpMeth
{
    FMKSRL_MPROCESS_WAKEUP_IDLE = 0x00,     /**< Wake up using idle line detection. */
    FMKSRL_MPROCESS_WAKEUP_ADDMARK,        /**< Wake up using address mark detection. */

    FMKSRL_MPROCESS_WAKEUP_NB              /**< Total number of wake-up methods. */
} t_eFMKSRL_MProcessWakeUpMeth;

#ifdef FMKCPU_STM32_ECU_FAMILY_G
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
#endif

/**
 * @brief Enumeration of UART/USART line modes.
 *
 * Specifies whether the line operates in receive, transmit, or both modes.
 */
typedef enum __t_eFMKSRL_LineMode
{
    FMKSRL_LINE_MODE_RX = 0x00,                 /**< Line in receive mode. */
    FMKSRL_LINE_MODE_TX,                        /**< Line in transmit mode. */
    FMKSRL_LINE_MODE_RX_TX,                     /**< Line in both receive and transmit modes. */

    FMKSRL_LINE_MODE_NB                         /**< Total number of line mode options. */
} t_eFMKSRL_LineMode;

/**
 * @brief Enumeration of UART/USART line parity modes.
 *
 * Specifies the parity configuration for the line.
 */
typedef enum __t_eFMKSRL_LineParity
{
    FMKSRL_LINE_PARITY_NONE = 0x00,         /**< No parity. */
    FMKSRL_LINE_PARITY_EVEN,                /**< Even parity. */
    FMKSRL_LINE_PARITY_ODD,                 /**< Odd parity. */

    FMKSRL_LINE_PARITY_NB                  /**< Total number of parity options. */
} t_eFMKSRL_LineParity;

/**
 * @brief Enumeration of UART/USART line word lengths.
 *
 * Defines the number of data bits in a transmission frame.
 */
typedef enum __t_eFMKSRL_LineWordLenght
{
    FMKSRL_LINE_WORDLEN_9BITS = 0x00,       /**< Word length of 9 bits. */
    FMKSRL_LINE_WORDLEN_8BITS,              /**< Word length of 8 bits. */
#ifdef FMKCPU_STM32_ECU_FAMILY_G
    FMKSRL_LINE_WORDLEN_7BITS,              /**< Word length of 7 bits. */
#endif
    FMKSRL_LINE_WORDLEN_NB                  /**< Total number of word length options. */
} t_eFMKSRL_LineWordLenght;

/**
 * @brief Enumeration of UART/USART line stop bits.
 *
 * Specifies the number of stop bits in a transmission frame.
 */
typedef enum __t_eFMKSRL_LineSoptbit
{
    FMKSRL_LINE_STOPBIT_0_5 = 0x00,          /**< Stop bit of 0.5 bit. */
    FMKSRL_LINE_STOPBIT_1,                   /**< Stop bit of 1 bit. */
    FMKSRL_LINE_STOPBIT_1_5,                 /**< Stop bit of 1.5 bits. */
    FMKSRL_LINE_STOPBIT_2,                   /**< Stop bit of 2 bits. */

    FMKSRL_LINE_STOPBIT_NB                   /**< Total number of stop bit options. */
} t_eFMKSRL_LineSoptbit;

/**
 * @brief Enumeration of UART/USART line baud rates.
 *
 * Defines the available baud rates for UART/USART communication.
 */
typedef enum __t_eFMKSRL_LineBaudrate
{
    FMKSRL_LINE_BAUDRATE_300 = 0x00,       /**< Baud rate of 300 bps. */
    FMKSRL_LINE_BAUDRATE_1200,             /**< Baud rate of 1200 bps. */
    FMKSRL_LINE_BAUDRATE_2400,             /**< Baud rate of 2400 bps. */
    FMKSRL_LINE_BAUDRATE_4800,             /**< Baud rate of 4800 bps. */
    FMKSRL_LINE_BAUDRATE_9600,             /**< Baud rate of 9600 bps. */
    FMKSRL_LINE_BAUDRATE_19200,            /**< Baud rate of 19200 bps. */
    FMKSRL_LINE_BAUDRATE_38400,            /**< Baud rate of 38400 bps. */
    FMKSRL_LINE_BAUDRATE_57600,            /**< Baud rate of 57600 bps. */
    FMKSRL_LINE_BAUDRATE_74880,            /**< Baud rate of 74880 bps. */
    FMKSRL_LINE_BAUDRATE_115200,           /**< Baud rate of 115200 bps. */
    FMKSRL_LINE_BAUDRATE_230400,           /**< Baud rate of 230400 bps. */
    FMKSRL_LINE_BAUDRATE_250000,           /**< Baud rate of 250000 bps. */

    FMKSRL_LINE_BAUDRATE_NB                /**< Total number of baud rate options. */
} t_eFMKSRL_LineBaudrate;

    /**
     * @brief Receive Callback Information for User
     */
    typedef enum 
    {
        FMKSRL_CB_INFO_RECEIVE_ENDING = 0x00,           /**< The message has been is complete in the callback*/
        FMKSRL_CB_INFO_RECEIVE_PENDING,             /**< The message has been cut and is not complete, user callback is about to
                                                            be called again to get the rest of the msg*/
        
        FMKSRL_CB_INFO_RECEIVE_OVERFLOW,            /**< The message is incomplete due to overflow buffer */
        FMKSRL_CB_INFO_RECEIVE_ERR,                 /**< The message is inncomplete or non-existent due to an unknown error */

    } t_eFMKSRL_RxCallbackInfo;
    
    /**
     * @brief Transmit Callback Information for User
     */
    typedef enum 
    {
        FMKSRL_CB_INFO_TRANSMIT_OK,                 /**< The message has been successfully transmit */
        FMKSRL_CB_INFO_TRANSMIT_OVERFLOW,           /**< The message hasn't be correctly send du to overflow error */
        FMKSRL_CB_INFO_TRANSMIT_ERR,                /**< The message hasn't be correctly send du to an unknown error */
    } t_eFMKSRL_TxCallbackInfo;

    /**
     * @brief Enumeration to represent the health status of a communication line.
     */
    typedef enum __t_eFMKSRL_LineHealth
    {
        FMKSRL_LINE_STATUS_OK = 0x00,       /**< UART/USART No Error detected */
        FMKSRL_LINE_STATUS_PE,              /**< UART/USART Line Parity Error */
        FMKSRL_LINE_STATUS_NE,              /**< UART/USART Noise Error */
        FMKSRL_LINE_STATUS_FE,              /**< UART/USART Frame Error */
        FMKSRL_LINE_STATUS_ORE,             /**< UART/USART Overrun Error */
        FMKSRL_LINE_STATUS_DMA,             /**< UART/USART DMA Transfer Error */
        FMKSRL_LINE_STATUS_RTO,             /**< UART/USART Receiver Timeout Error */
        FMKSRL_LINE_STATUS_UDR,            /**< USART SPI Slave Underrun Error */

        FMKSRL_LINE_STATUS_NB               /**< Number of Error Possible */
    } t_eFMKSRL_LineHealth;
	/* CAUTION : Automatic generated code section for Structure: Start */

	/* CAUTION : Automatic generated code section for Structure: End */
	//-----------------------------STRUCT TYPES---------------------------//
    /**
     * @brief UART Lin Configuration
     */
    typedef struct _t_sFMKSRL_LinCfg
    {
        t_eFMKSRL_LinBreakLenght BreakLen_e;        /**< Lin break Lenght */
    } t_sFMKSRL_LinCfg;

    /**
     * @brief UART Multi Process Configuration
     */
    typedef struct _t_sFMKSRL_MProcessCfg
    {
        t_eFMKSRL_MProcessWakeUpMeth WakeUpMethod_e;        /**< Multi Process Wake Up Method */
        t_uint8 IstcIdentifer_u8;                           /**< The identifier on the line for the CPU */
    } t_sFMKSRL_MProcessCfg;

    /**
     * @brief UART Mode Configuration
     */
    typedef union __t_uFMKSRL_UartTypeCfgSpec
    {
        t_sFMKSRL_LinCfg linCfg_s;                          /**< LIN configuration specific */
        t_sFMKSRL_MProcessCfg MProcessCfg_s;                /**< Multi Process Configuration */
    } t_uFMKSRL_UartTypeCfgSpec;

    typedef struct __t_sFMKSRL_UartAdvProtCfg
    {
        t_uint8 None1_u8;
        t_uint8 None2_u8;
    } t_sFMKSRL_UartAdvProtCfg;

    /**
     * @brief USART Configuration, that differ from UART
     */
    typedef struct __t_sFMKSRL_UsartCfgSpec
    {
        t_eFMKSRL_UsartType             Type_e;             /**< Usart Type of Protocol used */
        t_eFMKSRL_UsartClkPolarity      clkPolarity_e;      /**< Usart Clock Polarity */
        t_eFMKSRL_UsartClockPhase       clockPhase_e;       /**< Usart Clock Phase */
        t_eFMKSRL_UsartLastBit          lastBit_e;          /**< Usart Last Bit */
    } t_sFMKSRL_UsartCfgSpec;

    /**
     * @brief UART Configuration, that differ from USART
     */
    typedef struct __t_sFMKSRL_UartCfgSpec
    {
        t_eFMKSRL_UartType Type_e;                          /**< Uart Type of Protocol */
        t_uFMKSRL_UartTypeCfgSpec typeCfg_u;                /**< Uart Config Specific of the Protocol */
#ifdef FMKCPU_STM32_ECU_FAMILY_G
        t_sFMKSRL_UartAdvProtCfg    advProtCfg_s;           /**< Advance Configuration, not used at this point */
        t_eFMKSRL_UartHwFlowCtrl    hwFlowCtrl_e;           /**< Hardware Flow Control */
#endif
    } t_sFMKSRL_UartCfgSpec;

    /**
     * @brief USART/UART Configuration
     */
    typedef struct __t_sFMKSRL_HwProtocolCfg
    {
        t_eFMKSRL_LineBaudrate      Baudrate_e;         /**< Uart/Usart Line Baudrate */
        t_eFMKSRL_LineMode          Mode_e;             /**< Uart/Usart Rx/Tx Mode */
        t_eFMKSRL_LineParity        Parity_e;           /**< Uart/Usart Line Parity*/
        t_eFMKSRL_LineSoptbit       Stopbit_e;          /**< Uart/Usart Line Stop Bit */
        t_eFMKSRL_LineWordLenght    wordLenght_e;       /**< Uart/Usart Word Lenght */
    } t_sFMKSRL_HwProtocolCfg;


    
	/* CAUTION : Automatic generated code section : Start */

	/* CAUTION : Automatic generated code section : End */
	//-----------------------------TYPEDEF TYPES---------------------------//
    /**
    *
    *	@brief  Callback function to read a msg from UART/USART.\n
    *   @note   When the Serial line has started and user configure a receive mode,
    *           the module will call this function every time a message is received 
    *           and data match user configuration.\n
    *
    *	@param[in]  f_rxData_pu8          : the pointer to the data
    *	@param[in]  f_dataSize_u16        : the data size
    *	@param[in]  f_healthLine_e        : line status, value from @ref t_eFMKSRL_HealthLine
    *
    */
    typedef void (t_cbFMKSRL_RcvMsgEvent)(  t_uint8 * f_rxData_pu8, 
                                            t_uint16 f_dataSize_u16, 
                                            t_eFMKSRL_RxCallbackInfo f_InfoCb_e);

    /**
    *
    *	@brief  Callback function to know if a msg has been successfully transmit.\n
    *   @note   When the Serial line has started and user configure a receive mode,
    *           the module will call this function every time a message is received 
    *           and data match user configuration.\n
    *
    *	@param[in]  f_isMsgTransmit_b : Know id the message is transmited correctly.\n
    *	@param[in]  f_healthLine_e        : line status, value from @ref t_eFMKSRL_HealthLine
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    typedef void (t_cbFMKSRL_TransmitMsgEvent)(t_bool f_isMsgTransmit_b, t_eFMKSRL_TxCallbackInfo f_InfoCb_e);

    /**
     * @brief USART/UART Configuration Specific
     */
    typedef union __t_uSerialCfgSpec
    {
        t_sFMKSRL_UartCfgSpec       uartCfg_s;          /**< Serial Line Uart Configuration  */
        t_sFMKSRL_UsartCfgSpec      usartCfg_s;         /**< Serial Line Usart Configuration */
    } t_uFMKSRL_SerialCfgSpec;
    
    /**
     * @brief Driver Configuration
     */
    typedef struct __t_sFMKSRL_DrvSerialCfg
    {
        t_sFMKSRL_HwProtocolCfg     hwCfg_s;            /**< Hardware Protocol Configuration */
        t_uFMKSRL_SerialCfgSpec     CfgSpec_u;          /**< UART/USART Configuration */
        t_eFMKSRL_LineRunMode       runMode_e;          /**< Transmit/ Receive Run Mode */
        t_eFMKSRL_HwProtocolType    hwProtType_e;       /**< Uart/Usart Protocol used */
    } t_sFMKSRL_DrvSerialCfg;
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    


    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************
        /**
    *
    *	@brief      Perform all Init action for this module.\n
    *	@note       Set to default value all globals structure that
    *                store information for each signals.\n
    *
    */
    t_eReturnCode FMKSRL_Init(void);
    /**
    *
    *	@brief      Perform all Cyclic action for this module.\n
    *	@note       
    *
    */
    t_eReturnCode FMKSRL_Cyclic(void);
    /**
    *
    *	@brief Function to know the module state.\n 
    *
    *	@param[in]  f_State_pe : store the value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode FMKSRL_GetState(t_eCyclicModState *f_State_pe);
    /**
    *
    *	@brief Function to update the module state.\n
    *
    *	@param[in]  f_State_e : the new value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    */
    t_eReturnCode FMKSRL_SetState(t_eCyclicModState f_State_e);

    /**
    *
    *	@brief  Init an UART/ USART Driver for communication.\n 
    *   @note   This function allow user to configure a Serial Line.\n
    *           This function perform 
    *               - a verification of the configuration, for instance  : 
    *                      - UART Handle TypeDef can not perform a USART Protocol Init, reverse is true
    *               - Set the UART/USART Hardware Clock and set the RCC periphClock.\n
    *               - Set the NVIC state if the runMode is different from polling.\n
    *               - Set the Rx,Tx Line IO Configuration using FMKIO_Set_ComSerialCfg.\n
    *               - Set the bsp Serial Init UART/USART depending on hwProtType_e.\n
    *   @warning 
    *               If the parity is not None, be aware that the lenght of data bit (WordLen) 
    *               is impacted and reduce by 1, see the documentation on UART/USART protocol for more details 
    * 
    *   @example UART Basic Configuration 
    *               
    *            t_sFMKSRL_DrvSerialCfg SrlCfg_s;
    *            SrlCfg_s.runMode_e = FMKSRL_LINE_RUNMODE_IT;
    *            SrlCfg_s.hwProtType_e = FMKSRL_HW_PROTOCOL_UART;
    *
    *            SrlCfg_s.hwCfg_s.Baudrate_e = FMKSRL_LINE_BAUDRATE_9600,
    *            SrlCfg_s.hwCfg_s.Mode_e = FMKSRL_LINE_MODE_RX_TX;
    *            SrlCfg_s.hwCfg_s.Parity_e = FMKSRL_LINE_PARITY_NONE,
    *            SrlCfg_s.hwCfg_s.Stopbit_e = FMKSRL_LINE_STOPBIT_1,
    *            SrlCfg_s.hwCfg_s.wordLenght_e = FMKSRL_LINE_WORDLEN_8BITS,
    *
    *            SrlCfg_s.CfgSpec_u.uartCfg_s.hwFlowCtrl_e = FMKSRL_UART_HW_FLOW_CTRL_NONE;
    *            SrlCfg_s.CfgSpec_u.uartCfg_s.Type_e = FMKSRL_UART_TYPECFG_UART,
    * 
    *	@param[in]  f_SrlLine_e        : The Serial Line on Which the Configuration will be made, value from @ref t_eFMKSRL_SerialLine
    *	@param[in]  f_SerialCfg_s      : The Serial Configuration
    *	@param[in]  f_rcvMsgEvnt_pcb   : Pointer to function, to be called when msg is received. Essential
    *	@param[in]  f_rcvMsgEvnt_pcb   : Pointer to function, to be called when msg has been transmitted. not Essential
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode FMKSRL_InitDrv(   t_eFMKSRL_SerialLine f_SrlLine_e, 
                                    t_sFMKSRL_DrvSerialCfg f_SerialCfg_s,
                                    t_cbFMKSRL_RcvMsgEvent * f_rcvMsgEvnt_pcb,
                                    t_cbFMKSRL_TransmitMsgEvent * f_txMsgEvnt_pcb);

    /**
    *
    *	@brief  Transmit a Message threw Serial Line.
    *   @note   This function allow user to choose a Tx Mode from t_eFMKSRL_TxOpeMode
    *           and transmit a message using Tx Line of f_SrlLine_e.
    *           f_msgData_pu8 is the pointor for the beginning of the message with f_dataSize_u16 bytes
    *           To note that if the message is longer than the Tx Buffer Size in Excel Configuration 
    *           task is not accepted.
    *           If user want's to know whenever the message has been transmit, it has to set 
    *           to true f_EnableTxCb_b.
    *           When User Configure a Cylic Receive Operation with ConfigureReception before use TX_RX Operation, 
    *           the Receive Cylcic Operation will be momentarily disable. In consequence 
    *           be aware that the data on the Rx Line are lost once this function is called.
    *           The Cyclic Operation will be automatically enable once the Rx Message is received and user is called with callback        
    *
    *  @details If the user wants to reduce the complexity of the use of this function, to debug for instance.
    *           It can make a macro after make the InitDrv of the Serial Line as so :
    *           
    *           #define FMKSRL_DEBUGLOG(msg, size) FMKSRL_Transmit( FMKSRL_SERIAL_LINE_1, \
    *                                                               FMKSRL_TX_ONESHOT,    \
    *                                                               msg,                  \
    *                                                               size,                 \
    *                                                               False)
    *
    *	@param[in]  f_SrlLine_e        : The Serial Line on Which the Configuration will be made, value from @ref t_eFMKSRL_SerialLine
    *	@param[in]  f_OpeMode_e        : Transmit Ope Mode , value from @ref t_eFMKSRL_TxOpeMode
    *	@param[in]  f_msgData_pu8      : a pointor at the beginning of the message
    *	@param[in]  f_dataSize_u16     : Number a byte about to be transmist
    *	@param[in]  f_InfoMode_u16     : In FMKSRL_TX_RX_SIZE    -> f_InfoMode_u16 will be the size of data expected to be received.
    *                                    In FMKSRL_TX_RX_IDLE    -> f_InfoMode_u16 is not used.
    *                                    In FMKSRL_TX_RX_TIMEOUT -> f_InfoMode_u16 will be the amount of time after 
    *                                                                   we consider the line is quiet. then you'll be called 
    *	@param[in]  f_EnableTxCb_b     : If set to true, user will be called whenever the data is fully transmit
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode FMKSRL_Transmit(  t_eFMKSRL_SerialLine f_SrlLine_e, 
                                    t_eFMKSRL_TxOpeMode f_OpeMode_e,
                                    t_uint8 * f_msgData_pu8,
                                    t_uint16 f_dataSize_u16,
                                    t_uint16 f_InfoMode_u16,
                                    t_bool   f_EnableTxCb_b);

    /**
    *
    *	@brief      Configure a Reception Mode.
    *   @note       Configure the Rx line with f_OpeMode_e Mode.
    *               User will be called with data using the callback he gives in 
    *               InitDrv once the trigger appears.
    *               Be aware that in DMA mode, it is possible that the msg is cut between the end and 
    *               the beginning of the buffer. In this case user will be called once 
    *               with  f_InfoCb_e = MSG_CUT and a second time with f_InfoCb_e = 'MSG_END'
    *               
    *
    *	@param[in]  f_SrlLine_e        : The Serial Line on Which the Configuration will be made, value from @ref t_eFMKSRL_SerialLine
    *	@param[in]  f_OpeMode_e        : Receive Ope Mode , value from @ref t_eFMKSRL_TxOpeMode
    *	@param[in]  f_InfoMode_u16     : In FMKSRL_OPE_RX_ONESHOT/CYLIC_SIZE     -> f_InfoMode_u16 will be the size of data expected to be received.
    *                                    In FMKSRL_OPE_RX_ONESHOT/CYCLIC_IDLE    -> f_InfoMode_u16 is not used.
    *                                    In FMKSRL_OPE_RX_ONESHOT/CYCLIC_TIMEOUT -> f_InfoMode_u16 will be the amount of time after 
    *                                                                                   we consider the line is quiet. then you'll be called 
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode FMKSRL_ConfigureReception(  t_eFMKSRL_SerialLine f_SrlLine_e, 
                                              t_eFMKSRL_RxOpeMode f_OpeMode_e,
                                              t_uint16 f_InfoOpe_u16);

    /**
    *
    *	@brief      DO NOT USE, Function to get the Handle Type def for IRQN_Handler Function
    *
    *	@param[in]  f_timer_e                : enum value for the timer, value from @ref t_eFMKTIM_Timer
    *
    *
    */
    void * FMKSRL_PRIVATE_GetHandleTypeDef( t_eFMKSRL_SerialLine f_SrlLine_u8,
                                            UART_HandleTypeDef ** f_huartHandle_ps,
                                            USART_HandleTypeDef ** f_UsartHandle_ps);
#endif // FMKSERIAL_INCLUDED           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note
 *
 *
 *	@param[in] 
 *	@param[out]
 *	 
 *
 *
 */
