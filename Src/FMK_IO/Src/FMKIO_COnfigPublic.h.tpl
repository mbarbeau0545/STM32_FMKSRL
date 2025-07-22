/*********************************************************************
 * @file        FMKIO_ConfigPublic.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKIO_ConfigPublic_H_INCLUDED
#define FMKIO_ConfigPublic_H_INCLUDED
    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "TypeCommon.h"
    #include "./FMKCDA_ConfigPublic.h"
    #include "./FMKCPU_ConfigPublic.h"
    #include "FMK_HAL/FMK_HRT/Src/FMK_HRT.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    #ifdef FMKCPU_STM32_ECU_FAMILY_G
        #define FMKIO_BSP_MODE_INPUT ((t_uint32)GPIO_MODE_INPUT)
        #define FMKIO_BSP_MODE_ANALOG ((t_uint32)GPIO_MODE_ANALOG)
    #elif FMKCPU_STM32_ECU_FAMILY_F
        #define FMKIO_BSP_MODE_INPUT ((t_uint32)MODE_INPUT)
        #define FMKIO_BSP_MODE_ANALOG ((t_uint32)MODE_ANALOG)
    #else
        #error('ecu family is currently not available')
    #endif

    #define FMKIO_ENCODER_BUFFER ((t_uint8)10)

    /**
     * @brief Parameter for Pwm Output Frequency Range, 
     *          refer to FMKHRT_ConfigurePwmLine for more details 
     */
    #define FMKIO_PRM_FREQ_RANGE FMKHRT_FREQRANGE_DIV_4
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    //-----------------------------ENUM TYPES-----------------------------//
    typedef enum
    {
        FMKIO_INPUT_ENCODER_1,
        FMKIO_INPUT_ENCODER_2,
        FMKIO_INPUT_ENCODER_NB,
    } t_eFMKIO_InEcdrSignals;

    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief List of gpio port available on this board.
    */
    typedef enum
    {
        FMKIO_GPIO_PORT_A = 0x0,                  /**< Reference to bsp gpio A register */
        FMKIO_GPIO_PORT_B,                        /**< Reference to bsp gpio B register */
        FMKIO_GPIO_PORT_C,                        /**< Reference to bsp gpio C register */
        FMKIO_GPIO_PORT_D,                        /**< Reference to bsp gpio D register */
        FMKIO_GPIO_PORT_E,                        /**< Reference to bsp gpio E register */
        FMKIO_GPIO_PORT_F,                        /**< Reference to bsp gpio F register */
        FMKIO_GPIO_PORT_G,                        /**< Reference to bsp gpio G register */
    
        FMKIO_GPIO_PORT_NB,
    } t_eFMKIO_GpioPort;

    /**
    * @brief List of Pin available for each GPIO on this board.
    */
    typedef enum
    {
        FMKIO_GPIO_PIN_0 = 0x0,                  /**< Reference to bsp gpio pin 0 */
        FMKIO_GPIO_PIN_1,                        /**< Reference to bsp gpio pin 1 */
        FMKIO_GPIO_PIN_2,                        /**< Reference to bsp gpio pin 2 */
        FMKIO_GPIO_PIN_3,                        /**< Reference to bsp gpio pin 3 */
        FMKIO_GPIO_PIN_4,                        /**< Reference to bsp gpio pin 4 */
        FMKIO_GPIO_PIN_5,                        /**< Reference to bsp gpio pin 5 */
        FMKIO_GPIO_PIN_6,                        /**< Reference to bsp gpio pin 6 */
        FMKIO_GPIO_PIN_7,                        /**< Reference to bsp gpio pin 7 */
        FMKIO_GPIO_PIN_8,                        /**< Reference to bsp gpio pin 8 */
        FMKIO_GPIO_PIN_9,                        /**< Reference to bsp gpio pin 9 */
        FMKIO_GPIO_PIN_10,                       /**< Reference to bsp gpio pin 10 */
        FMKIO_GPIO_PIN_11,                       /**< Reference to bsp gpio pin 11 */
        FMKIO_GPIO_PIN_12,                       /**< Reference to bsp gpio pin 12 */
        FMKIO_GPIO_PIN_13,                       /**< Reference to bsp gpio pin 13 */
        FMKIO_GPIO_PIN_14,                       /**< Reference to bsp gpio pin 14 */
        FMKIO_GPIO_PIN_15,                       /**< Reference to bsp gpio pin 15 */
    
        FMKIO_GPIO_PIN_NB,
    } t_eFMKIO_BspGpioPin;

    /**
    * @brief /List of input digital pin available on this board.
    */
    typedef enum
    {
        FMKIO_INPUT_SIGDIG_1 = 0x0,                  /**< Reference to PC10 */
        FMKIO_INPUT_SIGDIG_2,                        /**< Reference to PC11 */
        FMKIO_INPUT_SIGDIG_3,                        /**< Reference to PC13 */
        FMKIO_INPUT_SIGDIG_4,                        /**< Reference to PC14 */
        FMKIO_INPUT_SIGDIG_5,                        /**< Reference to PC15 */
        FMKIO_INPUT_SIGDIG_6,                        /**< Reference to PA0 */
        FMKIO_INPUT_SIGDIG_7,                        /**< Reference to PA7 */
        FMKIO_INPUT_SIGDIG_8,                        /**< Reference to PB10 */
        FMKIO_INPUT_SIGDIG_9,                        /**< Reference to PB13 */
    
        FMKIO_INPUT_SIGDIG_NB,
    } t_eFMKIO_InDigSig;

    /**
    * @brief List of input Analog pin available on this board.
    */
    typedef enum
    {
        FMKIO_INPUT_SIGANA_1 = 0x0,                  /**< Reference to PA1 */
        FMKIO_INPUT_SIGANA_2,                        /**< Reference to PA4 */
        FMKIO_INPUT_SIGANA_3,                        /**< Reference to PB1 */
        FMKIO_INPUT_SIGANA_4,                        /**< Reference to PB0 */
        FMKIO_INPUT_SIGANA_5,                        /**< Reference to PA5 */
        FMKIO_INPUT_SIGANA_6,                        /**< Reference to PA6 */
    
        FMKIO_INPUT_SIGANA_NB,
    } t_eFMKIO_InAnaSig;

    /**
    * @brief List of input frequency pin available on this board.
    */
    typedef enum
    {
        FMKIO_INPUT_SIGFREQ_1 = 0x0,                  /**< Reference to PB6, TIMER_4 CHANNEL_1 */
        FMKIO_INPUT_SIGFREQ_2,                        /**< Reference to PB7, TIMER_4 CHANNEL_2 */
    
        FMKIO_INPUT_SIGFREQ_NB,
    } t_eFMKIO_InFreqSig;

    /**
    * @brief List of input event pin available on this board.
    */
    typedef enum
    {
        FMKIO_INPUT_SIGEVNT_1 = 0x0,                  /**< Reference to PB3 */
        FMKIO_INPUT_SIGEVNT_2,                        /**< Reference to PB5 */
        FMKIO_INPUT_SIGEVNT_3,                        /**< Reference to PC4 */
    
        FMKIO_INPUT_SIGEVNT_NB,
    } t_eFMKIO_InEvntSig;

    /**
    * @brief List of output digital pin available on this board.
    */
    typedef enum
    {
        FMKIO_OUTPUT_SIGDIG_1 = 0x0,                  /**< Reference to PB11 */
        FMKIO_OUTPUT_SIGDIG_2,                        /**< Reference to PA11 */
        FMKIO_OUTPUT_SIGDIG_3,                        /**< Reference to PA12 */
        FMKIO_OUTPUT_SIGDIG_4,                        /**< Reference to PA9 */
        FMKIO_OUTPUT_SIGDIG_5,                        /**< Reference to PB8 */
        FMKIO_OUTPUT_SIGDIG_6,                        /**< Reference to PC3 */
        FMKIO_OUTPUT_SIGDIG_7,                        /**< Reference to PC9 */
        FMKIO_OUTPUT_SIGDIG_8,                        /**< Reference to PC5 */
        FMKIO_OUTPUT_SIGDIG_9,                        /**< Reference to PF0 */
        FMKIO_OUTPUT_SIGDIG_10,                       /**< Reference to PF1 */
    
        FMKIO_OUTPUT_SIGDIG_NB,
    } t_eFMKIO_OutDigSig;

    /**
    * @brief List of output PWM pin available on this board.
    */
    typedef enum
    {
        FMKIO_OUTPUT_SIGPWM_1 = 0x0,                  /**< Reference to PB4, TIMER_16 CHANNEL_1 ,Pwm with Adaptable Frequency and DutyCycle */
        FMKIO_OUTPUT_SIGPWM_2,                        /**< Reference to PB9, TIMER_17 CHANNEL_1 ,Pwm with Adaptable Frequency and DutyCycle */
        FMKIO_OUTPUT_SIGPWM_3,                        /**< Reference to PC12, TIMER_5 CHANNEL_2 ,Pwm with Adaptable Frequency and DutyCycle */
        FMKIO_OUTPUT_SIGPWM_4,                        /**< Reference to PB15, TIMER_15 CHANNEL_2 ,Pwm with Adaptable Frequency and DutyCycle */
        FMKIO_OUTPUT_SIGPWM_5,                        /**< Reference to PC7, TIMER_8 CHANNEL_2 ,Pwm with Adaptable Frequency, DutyCycle and Pulses, (WaveForm pulses cannot be change during generation) */
        FMKIO_OUTPUT_SIGPWM_6,                        /**< Reference to PB2, TIMER_20 CHANNEL_1 ,Pwm with Adaptable Frequency, DutyCycle and Pulses, (WaveForm pulses cannot be change during generation) */
        FMKIO_OUTPUT_SIGPWM_7,                        /**< Reference to PC2, TIMER_1 CHANNEL_3 ,Pwm with Adaptable Frequency, DutyCycle and Pulses, (WaveForm pulses cannot be change during generation) */
        FMKIO_OUTPUT_SIGPWM_8,                        /**< Reference to PA8, TIMER_A CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
        FMKIO_OUTPUT_SIGPWM_9,                        /**< Reference to PA10, TIMER_B CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
        FMKIO_OUTPUT_SIGPWM_10,                       /**< Reference to PB12, TIMER_C CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
        FMKIO_OUTPUT_SIGPWM_11,                       /**< Reference to PB14, TIMER_D CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
        FMKIO_OUTPUT_SIGPWM_12,                       /**< Reference to PC6, TIMER_F CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
        FMKIO_OUTPUT_SIGPWM_13,                       /**< Reference to PC8, TIMER_E CHANNEL_1 ,Pwm with Adaptable Frequency > 500 Hz, DutyCycle, Pulses and current Feedback, (Waveform Pulses can be change during generation) */
    
        FMKIO_OUTPUT_SIGPWM_NB,
    } t_eFMKIO_OutPwmSig;

    /**
    * @brief List of signals used for CAN communication.
    */
    typedef enum
    {
        FMKIO_COM_SIGNAL_CAN_1 = 0x0,                  /**< Rx -> PF11, Tx -> PF12, Reference to CAN 0 */
        FMKIO_COM_SIGNAL_CAN_2,                        /**< Rx -> PG14, Tx -> PG15, Reference to CAN 1 */
        FMKIO_COM_SIGNAL_CAN_3,                        /**< Rx -> PG13, Tx -> PG12, Reference to CAN 2 */
    
        FMKIO_COM_SIGNAL_CAN_NB,
    } t_eFMKIO_ComSigCan;

    /**
    * @brief List of signals used for Serial communication.
    */
    typedef enum
    {
        FMKIO_COM_SIGNAL_SERIAL_1 = 0x0,                  /**< Rx -> PG3, Tx -> PG4, Reference to Serial PIN_3 */
        FMKIO_COM_SIGNAL_SERIAL_2,                        /**< Rx -> PA3, Tx -> PA2, Reference to Serial PIN_3 */
        FMKIO_COM_SIGNAL_SERIAL_3,                        /**< Rx -> PG5, Tx -> PG6, Reference to Serial PIN_5 */
        FMKIO_COM_SIGNAL_SERIAL_4,                        /**< Rx -> PG7, Tx -> PG8, Reference to Serial PIN_7 */
        FMKIO_COM_SIGNAL_SERIAL_5,                        /**< Rx -> PG9, Tx -> PG10, Reference to Serial PIN_9 */
    
        FMKIO_COM_SIGNAL_SERIAL_NB,
    } t_eFMKIO_ComSigSerial;

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------TYPEDEF TYPES---------------------------//
    //-----------------------------STRUCT TYPES---------------------------//
    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */

    /**< Basic configuration needed in all Signal configuration*/



    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************


    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // FMKIO_ConfigPublic_H_INCLUDED           
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
