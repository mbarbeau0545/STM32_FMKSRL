/**
 * @file        FMKIO.c
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      mba
 * @date        jj/mm/yyyy
 * @version     1.0
 */

// ********************************************************************
// *                      Includes
// ********************************************************************
#include "./FMK_IO.h"
#include "FMK_CFG/FMKCFG_ConfigFiles/FMKIO_ConfigPrivate.h"
#include "APP_CTRL/APP_SYS/Src/APP_SYS.h"
#include "FMK_HAL/FMK_TIM/Src/FMK_TIM.h"
#include "FMK_HAL/FMK_CDA/Src/FMK_CDA.h"
#include "FMK_HAL/FMK_CPU/Src/FMK_CPU.h"
#include "Library/SafeMem/SafeMem.h"

// ********************************************************************
// *                      Defines
// ********************************************************************
#define FMKIO_RAMP_UNUSED ((t_uint8)0xFF)
#define FMKIO_PID_UNUSED  ((t_uint8)0xFF)
#define FMKIO_AF_UNUSED   ((t_uint8)0xFF) /**< Flag to say that the alternate function is not used in Signal Init */
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
/**< Structure common to all digital signal to repertory signal information */
typedef struct 
{
    t_bool IsSigConfigured_b;               /**< Flag which indicate wether or not the signal has been configured */

} t_sFMKIO_DigSigInfo;

/**< Structure common to all analog  signal to repertory signal information */
typedef struct 
{
    t_bool IsSigConfigured_b;                   /**< Flag which indicate wether or not the signal has been configured */
    t_cbFMKIO_SigErrorMngmt * sigError_cb;      /**< callback function if an error occured  */

} t_sFMKIO_AnaSigInfo;
/**< Structure common to all Pwm signal to repertory signal information */
typedef struct 
{
    t_bool IsSigConfigured_b;                   /**< Flag which indicate wether or not the signal has been configured */
    t_uint32 frequencyReq_u32;
    t_uint16 reqDutycycle_u16;
    t_uint8 rampId_u8;
    t_uint8 pidId_u8;
    t_eFMKIO_PwmCtrlType ctrlType_e;
    t_cbFMKIO_PulseEvent    * pulseEvnt_pcb;      /**< callback function when a pulse is finihed if pwm pulse is set  */
    t_cbFMKIO_SigErrorMngmt * sigError_cb;      /**< callback function if an error occured  */

} t_sFMKIO_PwmSigInfo;

/**< Structure common to frequency signal information */
typedef struct 
{
    t_bool IsSigConfigured_b;       /**< @ref  IsSigConfigured_b*/
    t_bool isSigCalib_b;            /**< Flag to know if the frequency of the Input Compare is well calibrate  */
    t_eFMKIO_FreqMeas meas_e;       /**< Frequency measuremnt mode (counter/ freq/Period) */
    t_uint32 value_u32;             /**< the difference between the last and current capture on channel */
    t_uint32 savedValue_32;         /**< last value saved of diffValue_u32 */
    t_uint32 TimFreqValue_u32;
    t_uint32 TimARRValue_u32;         /**<  */
    t_bool FlagValueWriting_b;      /**< Flag indicate that an interruption is in progress */
    t_cbFMKIO_SigErrorMngmt * sigError_cb;    /**< callback function if an error occured  */

} t_sFMKIO_InFreqSigInfo;

/**< Structure common to Event signal information */
typedef struct 
{
    t_bool IsSigConfigured_b;               /**< @ref  IsSigConfigured_b*/
    t_uint32 debcDelay_u32;                 /**< Debouncer delay to ignore fluctuation when a interruption happen */
    t_cbFMKIO_EventFunc * EvntFunc_cb;       /**< Store the callback function*/
    t_cbFMKIO_SigErrorMngmt * sigError_cb;    /**< callback function if an error occured  */
   
} t_sFMKIO_InEvntSigInfo;

/**< Structure common to Encoder Signal Information */
typedef struct 
{
    t_bool isConfigured_b; 
    t_eFMKIO_EcdrStartOpe EcdrOpe;          /**< Encoder Operation to perform */
    t_uint8 MPR_u8;                         /**< Multiple Encoder Tour Per Revolution */
    t_uint32 calibOffset_u32;               /**< calibration offset parameter */
    t_uint16 PPRValue_u16;                  /**< Pulse per Revolution perform by the encoder  */
    t_eFMKIO_EcdrDir direction_e;           /**< Encoder Direction */
    t_uint32 lastPos_u32;                   /**< the position of the previous cyclic */
    t_float32 SoPosition_f32;                /**< Software Position calculate with offset, delta etc */
    t_float32 SoSpeed_f32;                /**< Software speed of the motor */
} t_sFMKIO_InEcdrSigInfo;
// ********************************************************************
// *                      Prototypes
// ********************************************************************

// ********************************************************************
// *                      Variables
// ********************************************************************
//--------IO Managment--------//
t_sFMKIO_InFreqSigInfo      g_InFreqSigInfo_as[FMKIO_INPUT_SIGFREQ_NB];        /**< Signal information for input frequency */
t_sFMKIO_AnaSigInfo         g_InAnaSigInfo_as[FMKIO_INPUT_SIGANA_NB];          /**< Signal information for input Analog */
t_sFMKIO_DigSigInfo         g_InDigSigInfo_as[FMKIO_INPUT_SIGDIG_NB];          /**< Signal information for input Digital */
t_sFMKIO_InEvntSigInfo      g_InEvntSigInfo_as[FMKIO_INPUT_SIGEVNT_NB];        /**< Signal information for input Event */
t_sFMKIO_InEcdrSigInfo      g_InEcdrSigInfo_as[FMKIO_INPUT_ENCODER_NB];        /**< Signal information for input Encoder */
t_sFMKIO_PwmSigInfo         g_OutPwmSigInfo_as[FMKIO_OUTPUT_SIGPWM_NB];        /**< Signal information for output PWM */
t_sFMKIO_DigSigInfo         g_OutDigSigInfo_as[FMKIO_OUTPUT_SIGDIG_NB];        /**< Signal information for output Digital */

//--------Communication Managment--------//
t_bool g_IsIOComCanConfigured_ab[FMKIO_COM_SIGNAL_CAN_NB];
t_bool g_IsIOComSerialConConfigured_ab[FMKIO_COM_SIGNAL_SERIAL_NB];

//----- Safe mem block ----//
t_sSafeMem_BlockInfo g_sfmb_EcdrInfo_as[FMKIO_INPUT_ENCODER_NB];
t_uint32 g_InFreqLastCapture_ua32[FMKIO_INPUT_SIGFREQ_NB];                      /**< Storage for Last Capture value for freqency pins */


/**< Variable to store the state of GPIO Clock */
t_eFMKCPU_ClockPortOpe g_IsGpioClockEnable_ae[FMKIO_GPIO_PORT_NB];


/**< Module state */
static t_eCyclicModState g_FmkIO_ModState_e = STATE_CYCLIC_CFG;
/**< DeadTime for Evnt Managment  */
t_uint32 g_lastTick_ua32[FMKIO_INPUT_SIGEVNT_NB];

/**< State of the fast task */
static t_bool g_fastTaskPwmStatus_b = (t_bool)False;
static t_bool g_fastTaskEcdrStatus_b = (t_bool)False;
//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************
/**
 *
 *	@brief      Get the Bsp Pull Mode 
 *	@note       Using the enum f_pull_e and switch case, this function 
                return the right uint32 hal pull for a pin.\n
 *
 *
 *	@param[in]  f_pull_e       : the enum that says the pull mode, value from @ref t_eFMKIO_PullMode
 *	@param[in]  f_bspPull_pu32 : the Hal value correponding the enum
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 * @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
 *
 */
static t_eReturnCode s_FMKIO_Get_BspPullMode(t_eFMKIO_PullMode f_pull_e, t_uint32 *f_bspPull_pu32);
/**
 *
 *	@brief      Get the Bsp speed Mode 
 *	@note       Using the enum f_spd_e and switch case, this function 
                return the right uint32 hal speed for a pin.\n
 *
 *
 *	@param[in]  f_spd_e       : the enum that says the speed mode, value from @ref t_eFMKIO_SpdMode
 *	@param[in]  f_bspSpd_pu32 : the Hal value correponding the enum
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 * @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
 *
 */
static t_eReturnCode s_FMKIO_Get_BspSpdMode(t_eFMKIO_SpdMode f_spd_e, t_uint32 *f_bspSpd_pu32);

/**
 *
 *	@brief      Get the Bsp trigger Mode for signal event
 *	@note       Using the enum f_trigger_e and switch case, this function 
                return the right uint32 hal trigger for a pin.\n
 *
 *
 *	@param[in]  f_trigger_e           : the enum that says the trigger mode, value from @ref t_eFMKIO_SigTrigCptr
 *	@param[in]  f_bspTrigger_pu32     : the Hal value correponding the enum
 *
 * @retval RC_OK                             @ref RC_OK
 *
 */
static t_eReturnCode s_FMKIO_Get_BspTriggerMode(t_eFMKIO_SigTrigCptr f_trigger_e, t_uint32 *f_bspTrigger_pu32);
/**
 *
 *	@brief      Get the Time Operation base on IO Encoder Ope.
 *
 *
 *	@param[in]  f_trigger_e           : the enum that says the trigger mode, value from @ref t_eFMKIO_SigTrigCptr
 *	@param[in]  f_bspTrigger_pu32     : the Hal value correponding the enum
 *
 * @retval RC_OK                             @ref RC_OK
 *
 */
static t_eReturnCode s_FMKIO_Get_EcdrTimerMode(t_eFMKIO_EcdrStartOpe f_StartOpeMode_e, t_eFMKTIM_EcdrOpe * f_EdrTimerOpe_pe);
/**
 *
 *	@brief      This function set the Init of a pin on a GPIO
 *
 *	@param[in]  f_GpioPort_e     : GPIOPort enum from @ref t_eFMKIO_GpioPort
 *	@param[in]  f_pin_e          : pin enum from @ref t_eFMKIO_BspGpioPin
 *	@param[in]  f_mode_u32       : Pin Mode from HAL_GPIO.h
 *	@param[in]  f_pull_e         : the enum that says the pull mode, value from @ref t_eFMKIO_PullMode
 *	@param[in]  f_spd_e          : the enum that says the speed mode, value from @ref t_eFMKIO_SpdMode
 *	@param[in]  f_AltFunc_u8     : the alternate function, FMKIO_AF_UNUSED if not used
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_Set_BspSigCfg(t_eFMKIO_GpioPort f_gpioPort_e,
                                            t_eFMKIO_BspGpioPin f_pin_e,
                                            t_uint32 f_mode_u32,
                                            t_eFMKIO_PullMode f_pull_e,
                                            t_eFMKIO_SpdMode f_spd_e,
                                            t_uint8 f_AltFunc_u8);
/**
 *
 *	@brief      FMKTIM callback function to determine signal frequency value
 *
 *	@param[in]  f_InterruptType_e        : Interrupt Type from  @ref t_eFMKTIM_InterruptLineType
 *	@param[in]  f_InterruptLine_u8       : Interrupt Line
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_MngSigFrequency(t_eFMKTIM_InterruptLineType f_InterruptType_e, t_uint8 f_InterruptLine_u8);
/**
 *
 *	@brief      Function to centralized GPIO interruption management
 *  @note       Called user function and reset flag 
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_MngSigPwmWaveForm( t_eFMKIO_OutPwmSig f_signal_e,
                                                t_uint16 * f_dutyCycle_pu16,
                                                t_uint32 * f_frequency_pu32,
                                                t_uint32 * f_pulses_pu32);
/**
 *
 *	@brief      FMKTIM callback function to determine signal frequency value
 *
 *	@param[in]  f_InterruptType_e        : Interrupt Type from  @ref t_eFMKTIM_InterruptLineType
 *	@param[in]  f_InterruptLine_u8       : Interrupt Line
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static void s_FMKIO_basicAdvTimerCallback(t_eFMKTIM_InterruptLineType f_InterruptType_e, t_uint8 f_InterruptLine_u8);
static void s_FMKIO_HighResTimerCallback(t_eFMKHRT_HighResLine f_HrLine_e, t_eFMKHRT_HrLineEvntCb f_Evnt_e);
/**
 *
 *	@brief      Function to set HAL_RCC clock state : Enable/Disable
 *
 *	@param[in]  f_GpioPort_e     : GPIOPort enum, value from @ref t_eFMKIO_GpioPort
 *	@param[in]  f_ope_e          : Enable or Diable RCC clock, value from @ref t_eFMKTIM_ClockPortOpe
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_Set_GpioClkState(t_eFMKIO_GpioPort f_gpioPort_e, t_eFMKCPU_ClockPortOpe f_ope_e);;

/**
 *
 *	@brief      Function to perform diag on siganl used 
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_Operational(void);
/**
 *
 *	@brief      Function to perform diag on siganl used 
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_SigFreqCalibOpe(void);
/**
 *
 *	@brief      Function to perform diag on siganl used 
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_PreOperational(void);
/**
 *
 *	@brief      Perform diagnostic cyclic on signal configured.\n 
 *
 * @retval RC_OK                             @ref RC_OK
 * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *
 */
static t_eReturnCode s_FMKIO_PerformDiagnostic(void);
/**
 *
 *	@brief      Fast task to perform fast actions
 */
static void s_FMKIO_FastTask(void);
/**
 *
 *	@brief      Fast task to perform Ramp/Pid COntrol on PWM
 */
static t_eReturnCode s_FMKIO_FastTask_PwmMngmt(void);
/**
 *
 *	@brief      Fast task to perform Encoder Computations
 */
static t_eReturnCode s_FMKIO_FastTask_EcdrMngmt(void);
//****************************************************************************
//                      Public functions - Implementation
//********************************************************************************
/*********************************
 * FMKTIM_Init
 *********************************/
t_eReturnCode FMKIO_Init(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLI_u8;
    t_uint8 idxBspPin_u8;
    GPIO_TypeDef * bspGpio_ps;

    
    //--------- Gpio clock port status ---------//
    for(LLI_u8 = (t_uint8)0; LLI_u8 < FMKIO_GPIO_PORT_NB ; LLI_u8++)
    {
        g_IsGpioClockEnable_ae[LLI_u8] = FMKCPU_CLOCKPORT_OPE_DISABLE;

        // set all pin to 0 to avoid random output voltage ----//
        for(idxBspPin_u8 = (t_uint8)0 ; idxBspPin_u8 < FMKIO_GPIO_PIN_NB ; idxBspPin_u8++)
        {
            Ret_e = FMKIO_Get_BspGpioPort((t_eFMKIO_GpioPort)LLI_u8, &bspGpio_ps);

            if(Ret_e == RC_OK)
            {
                HAL_GPIO_WritePin(  bspGpio_ps,
                                    c_BspPinMapping_ua16[idxBspPin_u8],
                                    GPIO_PIN_RESET);
            }
        }
    }

    //---------Set Frequency Input Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_INPUT_SIGFREQ_NB ; LLI_u8++)
    {
        g_InFreqSigInfo_as[LLI_u8].meas_e = FMKIO_FREQ_MEAS_NB;
        g_InFreqSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
        g_InFreqSigInfo_as[LLI_u8].isSigCalib_b        = False;
        g_InFreqSigInfo_as[LLI_u8].FlagValueWriting_b  = False;
        g_InFreqSigInfo_as[LLI_u8].value_u32           = (t_uint32)0;
        g_InFreqSigInfo_as[LLI_u8].savedValue_32      = (t_uint32)0;
        g_InFreqSigInfo_as[LLI_u8].sigError_cb = (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION;

        g_InFreqLastCapture_ua32[LLI_u8] = (t_uint32)0;
    }

    //---------Set Analogic Input Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_INPUT_SIGANA_NB ; LLI_u8++)
    {
        g_InAnaSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
        g_InAnaSigInfo_as[LLI_u8].sigError_cb = (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION;
    }

    //---------Set Digital Input Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_INPUT_SIGDIG_NB ; LLI_u8++)
    {
        g_InDigSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
    }

    //---------Set Event Input Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_INPUT_SIGEVNT_NB ; LLI_u8++)
    {
        g_InEvntSigInfo_as[LLI_u8].EvntFunc_cb = (t_cbFMKIO_EventFunc *)NULL_FUNCTION;
        g_InEvntSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
        g_InEvntSigInfo_as[LLI_u8].debcDelay_u32 = (t_uint32)0;
        g_InEvntSigInfo_as[LLI_u8].sigError_cb = (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION;

        g_lastTick_ua32[LLI_u8] = (t_uint32)0;
    }

    //---------Set Pwm Output Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_OUTPUT_SIGPWM_NB ; LLI_u8++)
    {
        g_OutPwmSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
        g_OutPwmSigInfo_as[LLI_u8].frequencyReq_u32   = (t_uint32)0;
        g_OutPwmSigInfo_as[LLI_u8].reqDutycycle_u16   = (t_uint16)0;
        g_OutPwmSigInfo_as[LLI_u8].rampId_u8 = (t_uint8)FMKIO_RAMP_UNUSED;
        g_OutPwmSigInfo_as[LLI_u8].pidId_u8 = (t_uint8)FMKIO_PID_UNUSED;
        g_OutPwmSigInfo_as[LLI_u8].sigError_cb = (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION;
        g_OutPwmSigInfo_as[LLI_u8].pulseEvnt_pcb = (t_cbFMKIO_PulseEvent *)NULL_FUNCTION;
    }

    //---------Set Digital Output Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < (t_uint8)FMKIO_OUTPUT_SIGDIG_NB ; LLI_u8++)
    {
        g_OutDigSigInfo_as[LLI_u8].IsSigConfigured_b   = False;
    }

    //---------Set Signal Com CAN Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < FMKIO_COM_SIGNAL_CAN_NB ; LLI_u8++)
    {
        g_IsIOComCanConfigured_ab[LLI_u8] = (t_bool)False;
    }

    //---------Set Signal Com CAN Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < FMKIO_COM_SIGNAL_SERIAL_NB ; LLI_u8++)
    {
        g_IsIOComSerialConConfigured_ab[LLI_u8] = (t_bool)False;
    }
    //---------Set Encoder Signal Default Value---------//
    for(LLI_u8 = (t_uint8)0 ; (LLI_u8 < FMKIO_INPUT_ENCODER_NB) && (Ret_e == RC_OK) ; LLI_u8++)
    {
        g_InEcdrSigInfo_as[LLI_u8].EcdrOpe = FMKIO_ENCODER_START_POS;
        g_InEcdrSigInfo_as[LLI_u8].lastPos_u32 = (t_uint32)0;
        g_InEcdrSigInfo_as[LLI_u8].PPRValue_u16 = (t_uint16)0;
        g_InEcdrSigInfo_as[LLI_u8].direction_e = FMKIO_ENCODER_DIR_FORWARD;
        g_InEcdrSigInfo_as[LLI_u8].isConfigured_b = (t_bool)False;

        Ret_e = SMB_SecureBlockInit(&g_sfmb_EcdrInfo_as[LLI_u8],
                                    &g_InEcdrSigInfo_as[LLI_u8],
                                    sizeof(t_sFMKIO_InEcdrSigInfo),
                                    5);
    }

    //---- Add Fast Task to APPSYS ----//
    Ret_e = APPSYS_AddFastTask(APPSYS_MODULE_FMK_IO, s_FMKIO_FastTask);

    return Ret_e;
}
/*********************************
 * FMKIO_Cyclic
 *********************************/
t_eReturnCode FMKIO_Cyclic(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch (g_FmkIO_ModState_e)
    {
        case STATE_CYCLIC_CFG:
        {
            g_FmkIO_ModState_e = STATE_CYCLIC_WAITING;
            Ret_e = RC_OK;
            break;
        }
        case STATE_CYCLIC_WAITING:
        {
            // nothing to do, just wait all module are Ope
            Ret_e = RC_OK;
            break;
        }
        case STATE_CYCLIC_PREOPE:
        {
            Ret_e = s_FMKIO_PreOperational();
            if(Ret_e == RC_OK)
            {
                g_FmkIO_ModState_e = STATE_CYCLIC_OPE;
            }
            // nothing to do, just wait all module are Ope
            break;
        }
        case STATE_CYCLIC_OPE:
        {
            Ret_e = s_FMKIO_Operational();
            if(Ret_e < RC_OK)
            {
                g_FmkIO_ModState_e = STATE_CYCLIC_ERROR;
            }
            break;
        }
        case STATE_CYCLIC_ERROR:
        {
            Ret_e = RC_OK;
            break;
        }
        case STATE_CYCLIC_BUSY:
        default:
            Ret_e = RC_OK;
            break;
    }

    return Ret_e;
}

/*********************************
 * FMKIO_GetState
 *********************************/
t_eReturnCode FMKIO_GetState(t_eCyclicModState *f_State_pe)
{
    t_eReturnCode Ret_e = RC_OK;
    
    if(f_State_pe == (t_eCyclicModState *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        *f_State_pe = g_FmkIO_ModState_e;
    }

    return Ret_e;
}

/*********************************
 * FMKIO_SetState
 *********************************/
t_eReturnCode FMKIO_SetState(t_eCyclicModState f_State_e)
{
    g_FmkIO_ModState_e = f_State_e;
    return RC_OK;
}
/*********************************
 * s_FMKIO_Get_BspPullMode
 *********************************/
t_eReturnCode FMKIO_Set_InDigSigCfg(t_eFMKIO_InDigSig f_signal_e, t_eFMKIO_PullMode f_pull_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_GpioPort gpioPort_e;

    if ((f_signal_e >= FMKIO_INPUT_SIGDIG_NB) 
    ||  (f_pull_e >= FMKIO_PULL_MODE_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InDigSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        gpioPort_e = c_InDigSigBspMap_as[f_signal_e].HwGpio_e;
        Ret_e = s_FMKIO_Set_BspSigCfg(gpioPort_e,
                                      c_InDigSigBspMap_as[f_signal_e].HwPin_e,
                                      (t_uint32)FMKIO_BSP_MODE_INPUT,
                                      f_pull_e,
                                      FMKIO_SPD_MODE_LOW, // irrevelent for a input sig dig
                                      FMKIO_AF_UNUSED);
        if (Ret_e == RC_OK)
        {
            g_InDigSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_InAnaSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_InAnaSigCfg(t_eFMKIO_InAnaSig f_signal_e, 
                                    t_eFMKIO_PullMode f_pull_e,
                                    t_cbFMKIO_SigErrorMngmt *f_sigErr_cb)
{
    t_eReturnCode Ret_e = RC_OK;

    if ((f_signal_e >= FMKIO_INPUT_SIGANA_NB) 
    ||  (f_pull_e >= FMKIO_PULL_MODE_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InAnaSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = s_FMKIO_Set_BspSigCfg(c_InAnaSigBspMap_as[f_signal_e].BasicCfg_s.HwGpio_e,
                                      c_InAnaSigBspMap_as[f_signal_e].BasicCfg_s.HwPin_e,
                                      (t_uint32)FMKIO_BSP_MODE_ANALOG,
                                      f_pull_e,
                                      FMKIO_SPD_MODE_LOW, // irrevelent for a input sig dig
                                      FMKIO_AF_UNUSED);
        if (Ret_e == RC_OK)
        { // configure the adc
            Ret_e = FMKCDA_Set_AdcChannelCfg(c_InAnaSigBspMap_as[f_signal_e].adc_e,
                                             c_InAnaSigBspMap_as[f_signal_e].adcChannel_e,
                                             FMKCDA_ADC_CFG_SCAN_DMA);

            if (Ret_e == RC_OK)
            { // update info
                g_InAnaSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
                g_InAnaSigInfo_as[f_signal_e].sigError_cb = f_sigErr_cb;
            }
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_InFreqSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_InFreqSigCfg(   t_eFMKIO_InFreqSig f_signal_e, 
                                        t_eFMKIO_SigTrigCptr f_trigger_e,
                                        t_eFMKIO_FreqMeas f_freqMeas_e,
                                        t_cbFMKIO_SigErrorMngmt *f_sigErr_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_ChnlMeasTrigger cpu_trigger_e = FMKTIM_LINE_MEAS_NB;
    t_eFMKTIM_InterruptLineIO ITLineIO_e;

    if (f_signal_e >= FMKIO_INPUT_SIGFREQ_NB 
    || f_trigger_e >= FMKIO_STC_NB 
    || f_freqMeas_e >= FMKIO_FREQ_MEAS_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InFreqSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = s_FMKIO_Set_BspSigCfg(c_InFreqSigBspMap_as[f_signal_e].BasicCfg_s.HwGpio_e,
                                      c_InFreqSigBspMap_as[f_signal_e].BasicCfg_s.HwPin_e,
                                      (t_uint32)GPIO_MODE_AF_PP,
                                      FMKIO_PULL_MODE_UP,
                                      FMKIO_SPD_MODE_HIGH, 
                                      c_InFreqSigBspMap_as[f_signal_e].BspAlternateFunc_u8);
        if (Ret_e == RC_OK)
        {
            switch (f_trigger_e)
            {
                case FMKIO_STC_RISING_EDGE:
                    cpu_trigger_e = FMKTIM_LINE_MEAS_RISING_EDGE;
                    break;
                case FMKIO_STC_FALLING_EDGE:
                    cpu_trigger_e = FMKTIM_LINE_MEAS_FALLING_EDGE;
                    break;
                case FMKIO_STC_BOTH_EDGE:
                    cpu_trigger_e = FMKTIM_LINE_MEAS_BOTH_EDGE;
                    break;
                case FMKIO_STC_NB:
                default:
                    Ret_e = RC_ERROR_WRONG_CONFIG;
                    break;
            }
        }
        if(Ret_e == RC_OK)
        {
            ITLineIO_e = (t_eFMKTIM_InterruptLineIO)c_InFreqSigBspMap_as[f_signal_e].ITLine_u8;
            // get the FMKTIM channel meas corresponding to f_trigger_e
            Ret_e = FMKTIM_Set_ICLineCfg(ITLineIO_e,
                                        cpu_trigger_e,
                                        (t_cbFMKTIM_InterruptLine *)s_FMKIO_MngSigFrequency);
        }
        if (Ret_e == RC_OK)
        { // update info
            g_InFreqSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
            g_InFreqSigInfo_as[f_signal_e].meas_e = f_freqMeas_e;
            g_InFreqSigInfo_as[f_signal_e].sigError_cb = f_sigErr_cb;
        }
        
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_InEncoderSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_InEncoderSigCfg(t_eFMKIO_InEcdrSignals f_InEncdr_e,
                                        t_sFMKIO_SigEcdrCfg f_encdrCfg_s,
                                        t_eFMKIO_EcdrStartOpe f_startOpe)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_GpioPort gpioPort_e;
    t_eFMKIO_BspGpioPin gpioPin_e;
    t_uint8 bspAF_u8;

    if(f_InEncdr_e >= FMKIO_INPUT_ENCODER_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InEcdrSigInfo_as[f_InEncdr_e].isConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if(Ret_e == RC_OK)
    {
        //----- Call FMKTIM Timer Encoder Configuration -----//
        Ret_e = FMKTIM_Set_EcdrLineCfg( c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].ITLine_e,
                                        f_encdrCfg_s.hwCfg_s,
                                        (t_uint32)(FMKIO_ECDR_ARR_VALUE));

        if(Ret_e == RC_OK)
        {
            //---- reach Information & Initialize Pin -----//
            gpioPort_e = c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].SigDir_s.HwGpio_e;
            gpioPin_e = c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].SigDir_s.HwPin_e;
            bspAF_u8 = c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].BspAlternateFunc_u8;

            Ret_e = s_FMKIO_Set_BspSigCfg(  gpioPort_e,
                                            gpioPin_e,
                                            GPIO_MODE_AF_PP,
                                            f_encdrCfg_s.pullMode_e,
                                            f_encdrCfg_s.speedMode_e,
                                            bspAF_u8);
            if(Ret_e == RC_OK)
            {
                gpioPort_e = c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].SigPos_s.HwGpio_e;
                gpioPin_e = c_FmkIo_InEcdrSigBspCfg_as[f_InEncdr_e].SigPos_s.HwPin_e;

                Ret_e = s_FMKIO_Set_BspSigCfg(  gpioPort_e,
                                                gpioPin_e,
                                                GPIO_MODE_AF_PP,
                                                f_encdrCfg_s.pullMode_e,
                                                f_encdrCfg_s.speedMode_e,
                                                bspAF_u8);
            }
        }
        if(Ret_e == RC_OK)
        {
            //--- we point directly to information 'cause interruption didn't start yet -----//
            g_InEcdrSigInfo_as[f_InEncdr_e].isConfigured_b = (t_bool)True;
            g_InEcdrSigInfo_as[f_InEncdr_e].PPRValue_u16 = (t_uint16)f_encdrCfg_s.PulsePerRev_u16;
            g_InEcdrSigInfo_as[f_InEncdr_e].MPR_u8 = (t_uint32)f_encdrCfg_s.MultipleTourPerRev_u8;
            g_InEcdrSigInfo_as[f_InEncdr_e].EcdrOpe = f_startOpe;
        }     
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Set_InEcdrCalibOffset
 *********************************/
t_eReturnCode FMKIO_Set_InEcdrCalibOffset(t_eFMKIO_InEcdrSignals f_InEncdr_e, t_uint32 f_calibValue_u32)
{
    t_eReturnCode Ret_e;
    t_sFMKIO_InEcdrSigInfo ecdrSigCfg_s;

    if(f_InEncdr_e >= FMKIO_INPUT_ENCODER_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    else 
    {
        Ret_e = SMB_Read(&g_sfmb_EcdrInfo_as[f_InEncdr_e], &ecdrSigCfg_s);

        if(Ret_e == RC_OK)
        {
            ecdrSigCfg_s.calibOffset_u32 = f_calibValue_u32;

            Ret_e = SMB_Write(&g_sfmb_EcdrInfo_as[f_InEncdr_e], &ecdrSigCfg_s);
        }
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Set_InEcdrCalibOffset
 *********************************/
t_eReturnCode FMKIO_Get_InEcdrSpeed(t_eFMKIO_InEcdrSignals f_InEncdr_e, 
                                    t_eFMKIO_EcdrValFormat f_speedFormat_e,
                                    t_float32 * f_ecdrSpeed_pf32)
{
    t_eReturnCode Ret_e;
    t_sFMKIO_InEcdrSigInfo ecdrSigCfg_s;

    if(f_InEncdr_e >= FMKIO_INPUT_ENCODER_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    else if(f_speedFormat_e >= FMKIO_ECDR_VAL_FORMAT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint32)f_speedFormat_e);
    }
    else 
    {
        Ret_e = SMB_Read(&g_sfmb_EcdrInfo_as[f_InEncdr_e], &ecdrSigCfg_s);

        if(Ret_e == RC_OK)
        {
            switch(f_speedFormat_e)
            {
            case FMKIO_ECDR_VAL_FORMAT_MRADIAN:
                *f_ecdrSpeed_pf32 = ecdrSigCfg_s.SoSpeed_f32;
            break;
            case FMKIO_ECDR_VAL_FORMAT_MDEGREE:
                *f_ecdrSpeed_pf32 = (t_float32)(ecdrSigCfg_s.SoSpeed_f32 * 180.0f / M_PI);
            break; 
            case FMKIO_ECDR_VAL_FORMAT_NB:
            default:
                Ret_e = RC_ERROR_WRONG_CONFIG;
                ASSERT((t_uint32)Ret_e);
            }
        }
    }

    return Ret_e;
}


/*********************************
 * FMKIO_Set_InEvntSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_InEvntSigCfg(t_eFMKIO_InEvntSig f_signal_e, 
                                          t_eFMKIO_PullMode f_pull_e,
                                          t_eFMKIO_SigTrigCptr f_trigger_e,
                                          t_uint32 f_debouncerDelay_u32,
                                          t_cbFMKIO_EventFunc * f_Evnt_cb,
                                          t_cbFMKIO_SigErrorMngmt *f_sigErr_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_GpioPort gpioPort_e;
    t_uint32 bspTrigger_u32;

    if ((f_signal_e >= FMKIO_INPUT_SIGEVNT_NB)
    ||  (f_pull_e >= FMKIO_PULL_MODE_NB) 
    ||  (f_trigger_e >= FMKIO_STC_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(f_Evnt_cb == (t_cbFMKIO_EventFunc *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InEvntSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        gpioPort_e = c_InEvntSigBspMap_as[f_signal_e].BasicCfg_s.HwGpio_e;
        Ret_e = s_FMKIO_Get_BspTriggerMode(f_trigger_e, &bspTrigger_u32);
        if (Ret_e == RC_OK)
        {
            Ret_e = s_FMKIO_Set_BspSigCfg(gpioPort_e,
                                          c_InEvntSigBspMap_as[f_signal_e].BasicCfg_s.HwPin_e,
                                          (t_uint32)bspTrigger_u32,
                                          FMKIO_PULL_MODE_DISABLE,
                                          FMKIO_SPD_MODE_LOW, // irrevelent for a input sig dig
                                          FMKIO_AF_UNUSED);
            if (Ret_e == RC_OK)
            {

                Ret_e = FMKCPU_Set_NVICState(c_InEvntSigBspMap_as[f_signal_e].ExtiIRQN_e,
                                             FMKCPU_NVIC_OPE_ENABLE);
                if (Ret_e == RC_OK)
                { // update info
                    g_InEvntSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
                    g_InEvntSigInfo_as[f_signal_e].debcDelay_u32 = f_debouncerDelay_u32;
                    g_InEvntSigInfo_as[f_signal_e].EvntFunc_cb = f_Evnt_cb;
                    g_InEvntSigInfo_as[f_signal_e].sigError_cb = f_sigErr_cb;
                }
            }
        }
    }
    return Ret_e;
}
/*********************************
 * FMKIO_Set_OutPwmSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_OutPwmSigCfg(   t_eFMKIO_OutPwmSig       f_signal_e, 
                                        t_sFMKIO_PwmWaveformCfg  f_sigPwmCfg_s,
                                        t_sFMKIO_PwmControlPrm   f_sigCtrlPrm_s,
                                        t_cbFMKIO_PulseEvent    * f_pulseEvnt_pcb,
                                        t_cbFMKIO_SigErrorMngmt * f_sigErr_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_GpioPort gpioPort_e = FMKIO_GPIO_PORT_NB;
    t_uint8 ITLineIO_u8;
    t_eFMKIO_OutTimerCfg timerOrigin_e;
    t_eFMKTIM_LinePolarity bscadvPolarity_e;
    t_sFMKHRT_PwmCfg pwmCfg_s;

    if (f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB 
    || f_sigPwmCfg_s.pullMode_e >= FMKIO_PULL_MODE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        ITLineIO_u8 = c_OutPwmSigBspMap_as[f_signal_e].ITLine_u8;    
        timerOrigin_e = c_OutPwmSigBspMap_as[f_signal_e].TimOrigin_e;    

        //---- Timer Configuration ----//
        if((timerOrigin_e == FMKIO_ITLINE_TYPE_BSCTIM)
        || (timerOrigin_e == FMKIO_ITLINE_TYPE_ADVTIM))
        {
            if(f_sigPwmCfg_s.polarity_e == FMKIO_SIGPWM_POLARITY_HIGH)
            {
                bscadvPolarity_e = FMKTIM_LINE_POLARITY_HIGH;
            }
            else 
            {
                bscadvPolarity_e = FMKTIM_LINE_POLARITY_LOW;
            }

            Ret_e = FMKTIM_Set_PWMLineCfg(  (t_eFMKTIM_InterruptLineIO)ITLineIO_u8, 
                                            f_sigPwmCfg_s.frequency_u32,
                                            bscadvPolarity_e,
                                            s_FMKIO_basicAdvTimerCallback);
        }
        else if(timerOrigin_e == FMKIO_ITLINE_TYPE_HRTIM)
        {
            
            pwmCfg_s.deadTime_u32 = f_sigPwmCfg_s.deadTime_u32;
            pwmCfg_s.frequency_u32 = f_sigPwmCfg_s.frequency_u32;

            if(f_sigPwmCfg_s.polarity_e == FMKIO_SIGPWM_POLARITY_HIGH)
            {
                pwmCfg_s.polarity_e = FMKHRT_LINE_POLARITY_HIGH;
            }
            else 
            {
                pwmCfg_s.polarity_e = FMKHRT_LINE_POLARITY_LOW;
            }

            Ret_e = FMKHRT_ConfigurePwmLine((t_eFMKHRT_HighResLine)ITLineIO_u8,
                                            FMKIO_PRM_FREQ_RANGE,
                                            pwmCfg_s,
                                            s_FMKIO_HighResTimerCallback);
        }
        else 
        {
            Ret_e = RC_ERROR_WRONG_CONFIG;
            ASSERT((t_uint16)(timerOrigin_e));
        }
        
        //---- Output Configuration ----//
        if (Ret_e == RC_OK)
        {
            gpioPort_e = c_OutPwmSigBspMap_as[f_signal_e].BasicCfg_s.HwGpio_e;
            
            Ret_e = s_FMKIO_Set_BspSigCfg(gpioPort_e,
                                      c_OutPwmSigBspMap_as[f_signal_e].BasicCfg_s.HwPin_e,
                                      (t_uint32)GPIO_MODE_AF_PP,
                                      f_sigPwmCfg_s.pullMode_e,
                                      f_sigPwmCfg_s.spdMode_e, // irrevelent for a input sig dig
                                      c_OutPwmSigBspMap_as[f_signal_e].BspAlternateFunc_u8);

        }

        //---- Control Output Configuration ----//
        if((Ret_e == RC_OK)
        && (f_sigCtrlPrm_s.ctrlType_e != FMKIO_PWM_CTRL_TYPE_UNUSED))
        {
            //---- Check If user wantsto ramps signal ----//
            if(f_sigCtrlPrm_s.rampCfg_ps != (t_sLIBRamp_RampCfg *)NULL)
            {
                Ret_e = LIBRamp_Init(   (*f_sigCtrlPrm_s.rampCfg_ps),
                                        (&g_OutPwmSigInfo_as[f_signal_e].rampId_u8));
            }
        }
        if (Ret_e == RC_OK)
        { 
            // update info
            g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
            g_OutPwmSigInfo_as[f_signal_e].sigError_cb = f_sigErr_cb;
            g_OutPwmSigInfo_as[f_signal_e].pulseEvnt_pcb = f_pulseEvnt_pcb;
            g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32 = (t_uint32)f_sigPwmCfg_s.frequency_u32;
            g_OutPwmSigInfo_as[f_signal_e].ctrlType_e = f_sigCtrlPrm_s.ctrlType_e;

        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_OutDigSigCfg
 *********************************/
t_eReturnCode FMKIO_Set_OutDigSigCfg(t_eFMKIO_OutDigSig f_signal_e,
                                      t_eFMKIO_PullMode f_pull_e,
                                      t_eFMKIO_SpdMode f_spd_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_GpioPort gpioPort_e;

    if ((f_signal_e >= FMKIO_OUTPUT_SIGDIG_NB )
    ||  (f_pull_e >= FMKIO_PULL_MODE_NB) 
    ||  (f_spd_e >= FMKIO_SPD_MODE_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutDigSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    if (Ret_e == RC_OK)
    {
        gpioPort_e = c_OutDigSigBspMap_as[f_signal_e].HwGpio_e;

        Ret_e = s_FMKIO_Set_BspSigCfg(gpioPort_e,
                                      c_OutDigSigBspMap_as[f_signal_e].HwPin_e,
                                      (t_uint32)GPIO_MODE_OUTPUT_PP,
                                      f_pull_e,
                                      f_spd_e,
                                      FMKIO_AF_UNUSED);
        if (Ret_e == RC_OK)
        {
            g_OutDigSigInfo_as[f_signal_e].IsSigConfigured_b = (t_bool)True;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_ComCanCfg
 *********************************/
t_eReturnCode FMKIO_Set_ComCanCfg(t_eFMKIO_ComSigCan f_SigCan_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKIO_RxTxComCfg *bspSigCanCfg_ps;

    if(f_SigCan_e >= FMKIO_COM_SIGNAL_CAN_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }

    if(g_IsIOComCanConfigured_ab[f_SigCan_e] == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    
    if(Ret_e == RC_OK)
    {
        //---------------Set Tx Configuration---------------//
        bspSigCanCfg_ps = (t_sFMKIO_RxTxComCfg *)&c_FmkIo_CanSigCfg_as[f_SigCan_e];

        Ret_e = s_FMKIO_Set_BspSigCfg(bspSigCanCfg_ps->TxPin_s.HwGpio_e,
                                      bspSigCanCfg_ps->TxPin_s.HwPin_e,
                                      GPIO_MODE_AF_PP,
                                      FMKIO_PULL_MODE_DISABLE,
                                      FMKIO_SPD_MODE_HIGH,
                                      bspSigCanCfg_ps->BspAlternateFunc_u8);

        if(Ret_e == RC_OK)
        {
            //---------------Set Rx Configuration---------------//
            Ret_e = s_FMKIO_Set_BspSigCfg(bspSigCanCfg_ps->RxPin_s.HwGpio_e,
                                      bspSigCanCfg_ps->RxPin_s.HwPin_e,
                                      GPIO_MODE_AF_PP,
                                      FMKIO_PULL_MODE_DISABLE,
                                      FMKIO_SPD_MODE_HIGH,
                                      bspSigCanCfg_ps->BspAlternateFunc_u8);
        }
        if(Ret_e == RC_OK)
        {
            g_IsIOComCanConfigured_ab[f_SigCan_e] = True;
        }
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Set_ComCanCfg
 *********************************/
t_eReturnCode FMKIO_Set_ComSerialCfg(t_eFMKIO_ComSigSerial f_SigSerial_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKIO_RxTxComCfg *bspSigSerialCfg_ps;

    if(f_SigSerial_e >= FMKIO_COM_SIGNAL_SERIAL_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }

    if(g_IsIOComSerialConConfigured_ab[f_SigSerial_e] == (t_bool)True)
    {
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    
    if(Ret_e == RC_OK)
    {
        //---------------Set Tx Configuration---------------//
        bspSigSerialCfg_ps = (t_sFMKIO_RxTxComCfg *)&c_FmkIo_SerialSigCfg_as[f_SigSerial_e];

        Ret_e = s_FMKIO_Set_BspSigCfg(bspSigSerialCfg_ps->TxPin_s.HwGpio_e,
                                      bspSigSerialCfg_ps->TxPin_s.HwPin_e,
                                      GPIO_MODE_AF_PP,
                                      FMKIO_PULL_MODE_DISABLE,
                                      FMKIO_SPD_MODE_LOW,
                                      bspSigSerialCfg_ps->BspAlternateFunc_u8);

        if(Ret_e == RC_OK)
        {
            //---------------Set Rx Configuration---------------//
            Ret_e = s_FMKIO_Set_BspSigCfg(bspSigSerialCfg_ps->RxPin_s.HwGpio_e,
                                      bspSigSerialCfg_ps->RxPin_s.HwPin_e,
                                      GPIO_MODE_AF_PP,
                                      FMKIO_PULL_MODE_DISABLE,
                                      FMKIO_SPD_MODE_LOW,
                                      bspSigSerialCfg_ps->BspAlternateFunc_u8);
        }
        if(Ret_e == RC_OK)
        {
            g_IsIOComSerialConConfigured_ab[f_SigSerial_e] = True;
        }
    }

    return Ret_e;
}
/*********************************
 * FMKIO_Set_OutDigSigValue
 *********************************/
t_eReturnCode FMKIO_Set_OutDigSigValue(t_eFMKIO_OutDigSig f_signal_e, t_eFMKIO_DigValue f_value_e)
{
    t_eReturnCode Ret_e = RC_OK;
    GPIO_TypeDef *bspGpio_ps = (GPIO_TypeDef *)NULL;
    GPIO_PinState bspSigValue_e;

    if (f_signal_e >= FMKIO_OUTPUT_SIGDIG_NB || f_value_e >= FMKIO_DIG_VALUE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutDigSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = FMKIO_Get_BspGpioPort(c_OutDigSigBspMap_as[f_signal_e].HwGpio_e, &bspGpio_ps);
        if (Ret_e == RC_OK)
        {
            switch (f_value_e)
            {
            case FMKIO_DIG_VALUE_LOW:
                bspSigValue_e = GPIO_PIN_RESET;
                break;
            case FMKIO_DIG_VALUE_HIGH:
                bspSigValue_e = GPIO_PIN_SET;
                break;
            case FMKIO_DIG_VALUE_NB:
            default:
                Ret_e = RC_WARNING_WRONG_RESULT;
                break;
            }
            if (Ret_e == RC_OK)
            {
                HAL_GPIO_WritePin(bspGpio_ps,
                                  c_BspPinMapping_ua16[c_OutDigSigBspMap_as[f_signal_e].HwPin_e],
                                  bspSigValue_e);
            }
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Set_OutPwmSigDutyCycle
 *********************************/
t_eReturnCode FMKIO_Set_OutPwmSigDutyCycle(t_eFMKIO_OutPwmSig f_signal_e, t_uint16 f_dutyCycle_u16)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16 == f_dutyCycle_u16)
    {
        Ret_e = RC_WARNING_NO_OPERATION;
    }
    if(Ret_e == RC_OK)
    {
        //---- if Pwm Ctrl Active let Fast Task deal with it ----//
        if(g_OutPwmSigInfo_as[f_signal_e].ctrlType_e == FMKIO_PWM_CTRL_TYPE_DC)
        {
            //---- check if the fast task is on ----//
            //---- if the dutycycle doesn't change from before enable fast task nothing to do ----//
            if(g_fastTaskPwmStatus_b == (t_bool)False)
            {
                g_fastTaskPwmStatus_b = True;
                Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_FMK_IO,
                                                APPSYS_FAST_TASK_ENABLE);
                if(Ret_e == RC_OK)
                {   
                    g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16 = f_dutyCycle_u16;
                }
            }
        }
        else 
        {
            //---- Make the output change now ----//
            Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                (&f_dutyCycle_u16),
                                                (t_uint32 *)NULL,
                                                (t_uint32 *)NULL);

            if(Ret_e == RC_OK)
            {
                g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16 = f_dutyCycle_u16;
            }
        }
    }
    if(Ret_e == RC_WARNING_NO_OPERATION)
    {
        Ret_e = RC_OK;
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Set_OutPwmSigFrequency
 *********************************/
t_eReturnCode FMKIO_Set_OutPwmSigFrequency(t_eFMKIO_OutPwmSig f_signal_e, t_uint32 f_frequency_u32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32 == f_frequency_u32)
    {
        Ret_e = RC_WARNING_NO_OPERATION;
    }
    if(Ret_e == RC_OK)
    {
        //---- if Pwm Ctrl Active let Fast Task deal with it ----//
        if(g_OutPwmSigInfo_as[f_signal_e].ctrlType_e == FMKIO_PWM_CTRL_TYPE_FREQ)
        {
            //---- check if the fast task is on ----//
            //---- if the dutycycle doesn't change from before enable fast task nothing to do ----//
            if(g_fastTaskPwmStatus_b == (t_bool)False)
            {
                g_fastTaskPwmStatus_b = True;
                Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_FMK_IO,
                                                APPSYS_FAST_TASK_ENABLE);
                if(Ret_e == RC_OK)
                {   
                    g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32 = f_frequency_u32;
                }
            }
        }
        else 
        {
            //---- Make the output change now ----//
            Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                (t_uint16 *)(&g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16),
                                                (t_uint32 *)(&f_frequency_u32),
                                                (t_uint32 *)NULL);

            if(Ret_e == RC_OK)
            {
                g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32 = f_frequency_u32;
            }
        }
    }
    
    if(Ret_e == RC_WARNING_NO_OPERATION)
    {
        Ret_e = RC_OK;
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Set_OutPwmSigPulses
 *********************************/
t_eReturnCode FMKIO_Set_OutPwmSigPulses(t_eFMKIO_OutPwmSig f_signal_e, 
                                        t_uint32 f_frequency_f32,
                                        t_uint16 f_dutyCycle_u16,
                                        t_uint16 f_pulses_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKIO_OutTimerCfg timOrgn_e;
    t_bool reqFastTaskON_b = False;

    if ((f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB)
    ||  (f_dutyCycle_u16 > FMKTIM_PWM_MAX_DUTY_CYLCE)
    ||  (f_pulses_u16 > (t_uint16)CST_MAX_UINT_16BIT))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(Ret_e == RC_OK)
    {
        timOrgn_e = c_OutPwmSigBspMap_as[f_signal_e].TimOrigin_e;
        if(timOrgn_e == FMKIO_ITLINE_TYPE_HRTIM)
        {
            //---- if Pwm Ctrl Active let Fast Task deal with it ----//
            if(g_OutPwmSigInfo_as[f_signal_e].ctrlType_e == FMKIO_PWM_CTRL_TYPE_DC)
            {
                //---- pulses are send now with previous dutycycle----//
                reqFastTaskON_b = True;
                Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                    (&g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16),
                                                    (t_uint32* )(&f_frequency_f32),
                                                    (t_uint32 *)(&f_pulses_u16));
            }
            else if(g_OutPwmSigInfo_as[f_signal_e].ctrlType_e == FMKIO_PWM_CTRL_TYPE_FREQ)
            {
                //---- pulses are send now with previous frequency and current dc----//
                reqFastTaskON_b = True;
                Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                    (&f_dutyCycle_u16),
                                                    (&g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32),
                                                    (t_uint32 *)(&f_pulses_u16));
            }
            else 
            {
                //---- pulses are send now with previous frequency and current dc----//
                Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                    (&f_dutyCycle_u16),
                                                    (&f_frequency_f32),
                                                    (t_uint32 *)(&f_pulses_u16));
            }
            //---- copy info ----//
            if(Ret_e == RC_OK)
            {
                g_OutPwmSigInfo_as[f_signal_e].frequencyReq_u32 = (t_uint32)f_frequency_f32;
                g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16 = (t_uint16)f_dutyCycle_u16;
            }
            //---- check if the fast task is on ----//
            if((Ret_e == RC_OK)
            && (reqFastTaskON_b == (t_bool)True)
            && (g_fastTaskPwmStatus_b == (t_bool)False))
            {
                g_fastTaskPwmStatus_b = True;
                Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_FMK_IO,
                                                APPSYS_FAST_TASK_ENABLE);
            }
        }
        else if(timOrgn_e == FMKIO_ITLINE_TYPE_ADVTIM)
        {
            //---- Make the output change now ----//
            Ret_e = s_FMKIO_MngSigPwmWaveForm(  f_signal_e,
                                                (t_uint16 *)(&f_dutyCycle_u16),
                                                (t_uint32 *)(&f_frequency_f32),
                                                (t_uint32 *)(&f_pulses_u16));

            if(Ret_e == RC_OK)
            {
                g_OutPwmSigInfo_as[f_signal_e].reqDutycycle_u16 = f_dutyCycle_u16;
            }
        }
        else if (timOrgn_e == FMKIO_ITLINE_TYPE_BSCTIM)
        {
            Ret_e =  RC_ERROR_WRONG_CONFIG;
            ASSERT((t_uint16)Ret_e);
        }
        else
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
            ASSERT((t_uint16)timOrgn_e);
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Get_OutPwmSigFrequency
 *********************************/
t_eReturnCode FMKIO_Get_OutPwmSigFrequency(t_eFMKIO_OutPwmSig f_signal_e, t_uint32 * f_frequency_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_PwmValue pwmTimOpe_s;
    t_sFMKHRT_PwmOpeVal pwmHrTimOpe_s;
    t_eFMKTIM_InterruptLineIO ITLineIO_u8;
    t_uint8 maskUpdate_u8 = (t_uint8)0; 
    t_eFMKIO_OutTimerCfg timOrgn_e;

    if ((f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB)
    ||  (f_frequency_pu32 == (t_uint32 *)NULL))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(Ret_e == RC_OK)
    {
        ITLineIO_u8 = c_OutPwmSigBspMap_as[f_signal_e].ITLine_u8;
        timOrgn_e = c_OutPwmSigBspMap_as[f_signal_e].TimOrigin_e;

        if((timOrgn_e == FMKIO_ITLINE_TYPE_BSCTIM)
        || (timOrgn_e == FMKIO_ITLINE_TYPE_ADVTIM))
        {

            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_PWM_FREQUENCY);

            Ret_e = FMKTIM_Get_PwmLineValue(ITLineIO_u8,
                                            (&pwmTimOpe_s),
                                            maskUpdate_u8);
            
            if(Ret_e == RC_OK)
            {
                *f_frequency_pu32 = pwmTimOpe_s.frequency_u32;
            }
        }
        else if(timOrgn_e == FMKIO_ITLINE_TYPE_HRTIM)
        {
            SETBIT_8B(maskUpdate_u8, FMKHRT_BIT_PWM_FREQUENCY);

            Ret_e = FMKHRT_GetPwmLineWaveform(  (t_eFMKHRT_HighResLine)ITLineIO_u8,
                                                (&pwmHrTimOpe_s),
                                                maskUpdate_u8);
            if(Ret_e == RC_OK)
            {
                *f_frequency_pu32 = pwmHrTimOpe_s.frequency_u32;
            }
        }
        else 
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
            ASSERT((t_uint16)Ret_e);
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Get_OutPwmSigDutyCycle
 *********************************/
t_eReturnCode FMKIO_Get_OutPwmSigDutyCycle(t_eFMKIO_OutPwmSig f_signal_e, t_uint16 * f_dutyCycle_pu16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_PwmValue pwmTimOpe_s;
    t_sFMKHRT_PwmOpeVal pwmHrTimOpe_s;
    t_eFMKTIM_InterruptLineIO ITLineIO_u8;
    t_uint8 maskUpdate_u8 = (t_uint8)0; 
    t_eFMKIO_OutTimerCfg timOrgn_e;

    if ((f_signal_e >= FMKIO_OUTPUT_SIGPWM_NB)
    ||  (f_dutyCycle_pu16 == (t_uint16 *)NULL))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutPwmSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(Ret_e == RC_OK)
    {
        ITLineIO_u8 = c_OutPwmSigBspMap_as[f_signal_e].ITLine_u8;
        timOrgn_e = c_OutPwmSigBspMap_as[f_signal_e].TimOrigin_e;

        if((timOrgn_e == FMKIO_ITLINE_TYPE_BSCTIM)
        || (timOrgn_e == FMKIO_ITLINE_TYPE_ADVTIM))
        {

            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_PWM_DUTYCYCLE);

            Ret_e = FMKTIM_Get_PwmLineValue(ITLineIO_u8,
                                        (&pwmTimOpe_s),
                                        maskUpdate_u8);
            
            if(Ret_e == RC_OK)
            {
                *f_dutyCycle_pu16 = pwmTimOpe_s.dutyCycle_u16;
            }
        }
        else if(timOrgn_e == FMKIO_ITLINE_TYPE_HRTIM)
        {
            SETBIT_8B(maskUpdate_u8, FMKHRT_BIT_PWM_DUTYCYCLE);

            Ret_e = FMKHRT_GetPwmLineWaveform(  (t_eFMKHRT_HighResLine)ITLineIO_u8,
                                                (&pwmHrTimOpe_s),
                                                maskUpdate_u8);
            if(Ret_e == RC_OK)
            {
                *f_dutyCycle_pu16 = pwmHrTimOpe_s.dutyCycle_u16;
            }
        }
        else 
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
            ASSERT((t_uint16)Ret_e);
        }
    }
    return Ret_e;
}


/*********************************
 * FMKIO_Get_InDigSigValue
 *********************************/
t_eReturnCode FMKIO_Get_InEcdrPositionValue(t_eFMKIO_InEcdrSignals f_signal_e, 
                                            t_eFMKIO_EcdrValFormat f_format_e,
                                            t_float32 *f_value_pf32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKIO_InEcdrSigInfo ecdrSigInfo_s;

    if((f_signal_e >= FMKIO_INPUT_ENCODER_NB)
    || (f_format_e >= FMKIO_ECDR_VAL_FORMAT_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(f_value_pf32 == (t_float32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    else 
    {
        Ret_e = SMB_Read(   &g_sfmb_EcdrInfo_as[f_signal_e],
                            &ecdrSigInfo_s);
        if(Ret_e == RC_OK)
        {
            if(ecdrSigInfo_s.EcdrOpe == FMKIO_ENCODER_START_DIR)
            {
                Ret_e = RC_ERROR_NOT_ALLOWED;
                ASSERT((t_uint16)Ret_e);
            }
            if(ecdrSigInfo_s.isConfigured_b == (t_bool)False)
            {
                Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
                ASSERT((t_uint16)Ret_e);
            }
            if(Ret_e == RC_OK)
            {
                switch(f_format_e)
                {
                case FMKIO_ECDR_VAL_FORMAT_MRADIAN:
                    //---- already in milliradian ----//
                    *f_value_pf32 = ecdrSigInfo_s.SoPosition_f32;
                break;
                case FMKIO_ECDR_VAL_FORMAT_MDEGREE:
                    *f_value_pf32 = (t_float32)(ecdrSigInfo_s.SoPosition_f32 * (t_float32)180 / M_PI);
                break;
                case FMKIO_ECDR_VAL_FORMAT_NB:
                default:
                    Ret_e = RC_ERROR_WRONG_CONFIG;
                    ASSERT((t_uint32)0);
                break;
                }
            }
        }
    }

    return Ret_e;
}

/*********************************
 * FMKIO_Get_InDigSigValue
 *********************************/
t_eReturnCode FMKIO_Get_InEcdrDirectionValue(t_eFMKIO_InEcdrSignals f_signal_e, t_eFMKIO_EcdrDir *f_Dirvalue_pe)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKIO_InEcdrSigInfo ecdrSigInfo_s;

    if(f_signal_e >= FMKIO_INPUT_ENCODER_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(f_Dirvalue_pe == (t_eFMKIO_EcdrDir *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    else 
    {
        Ret_e = SMB_Read(   &g_sfmb_EcdrInfo_as[f_signal_e],
                            &ecdrSigInfo_s);
        if(Ret_e == RC_OK)
        {
            if(ecdrSigInfo_s.EcdrOpe == FMKIO_ENCODER_START_POS)
            {
                Ret_e = RC_ERROR_NOT_ALLOWED;
                ASSERT((t_uint16)Ret_e);
            }
            if(ecdrSigInfo_s.isConfigured_b == (t_bool)False)
            {
                Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
                ASSERT((t_uint16)Ret_e);
            }
            if(Ret_e == RC_OK)
            {
                *f_Dirvalue_pe = ecdrSigInfo_s.direction_e;
            }
        }
    }

    return Ret_e;
}
/*********************************
 * FMKIO_Get_InDigSigValue
 *********************************/
t_eReturnCode FMKIO_Get_InDigSigValue(t_eFMKIO_InDigSig f_signal_e, t_eFMKIO_DigValue *f_value_pe)
{
    t_eReturnCode Ret_e = RC_OK;
    GPIO_TypeDef *bspGpio_ps = (GPIO_TypeDef *)NULL;
    GPIO_PinState bspSigValue_e;

    if (f_signal_e >= FMKIO_INPUT_SIGDIG_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_value_pe == (t_eFMKIO_DigValue *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InDigSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = FMKIO_Get_BspGpioPort(c_InDigSigBspMap_as[f_signal_e].HwGpio_e, &bspGpio_ps);
        if (Ret_e == RC_OK)
        {
            bspSigValue_e = HAL_GPIO_ReadPin(bspGpio_ps, c_BspPinMapping_ua16[c_InDigSigBspMap_as[f_signal_e].HwPin_e]);
             switch (bspSigValue_e)
            {
            case GPIO_PIN_RESET:
                *f_value_pe = FMKIO_DIG_VALUE_LOW;
                break;
            case GPIO_PIN_SET:
                *f_value_pe = FMKIO_DIG_VALUE_HIGH;
                break;
            default:
                *f_value_pe = FMKIO_DIG_VALUE_LOW;
                Ret_e = RC_WARNING_WRONG_RESULT;
                break;
            }
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKIO_Get_BspPullMode
 *********************************/
t_eReturnCode FMKIO_Get_InAnaSigValue(t_eFMKIO_InAnaSig f_signal_e, t_uint16 *f_value_pu16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint16 anaValue_16 = 0;

    if (f_signal_e >= FMKIO_INPUT_SIGANA_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_value_pu16 == (t_uint16 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InAnaSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = FMKCDA_Get_AnaChannelMeasure(c_InAnaSigBspMap_as[f_signal_e].adc_e,
                                             c_InAnaSigBspMap_as[f_signal_e].adcChannel_e,
                                             &anaValue_16);
        if(Ret_e == RC_OK)
        {
            *f_value_pu16 = (t_uint16)anaValue_16;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Get_InFreqSigValue
 *********************************/
t_eReturnCode FMKIO_Get_InFreqSigValue(t_eFMKIO_InFreqSig f_signal_e, t_uint32 *f_value_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_float32 value_f32 = 0;
    t_uint32 arrTim_u32 = 0;

    if (f_signal_e >= FMKIO_INPUT_SIGFREQ_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_InFreqSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_value_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(Ret_e == RC_OK)
    {
        if((g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
        || g_InFreqSigInfo_as[f_signal_e].isSigCalib_b == (t_bool)False)
        {
            Ret_e = RC_WARNING_BUSY;
        }
    }
    if (Ret_e == RC_OK)
    {
        // if flag writing value is False, make calcul on value, else make calcul on saved value
        if( g_InFreqSigInfo_as[f_signal_e].FlagValueWriting_b == (t_bool)False)
        {
            value_f32 = (t_float32)g_InFreqSigInfo_as[f_signal_e].value_u32;
        }
        else
        {
            value_f32 = (t_float32)g_InFreqSigInfo_as[f_signal_e].savedValue_32;
            Ret_e = RC_WARNING_BUSY;
        }

        arrTim_u32 = g_InFreqSigInfo_as[f_signal_e].TimARRValue_u32;

        switch(g_InFreqSigInfo_as[f_signal_e].meas_e)
        {
            case FMKIO_FREQ_MEAS_COUNT:
                *f_value_pu32 = (t_uint32)value_f32;
                g_InFreqSigInfo_as[f_signal_e].value_u32 = (t_uint32)0;
                g_InFreqSigInfo_as[f_signal_e].savedValue_32 = (t_uint32)0;
                break;

            case FMKIO_FREQ_MEAS_PERIOD:
                if(arrTim_u32 > (t_uint32)0)
                {
                    *f_value_pu32 = (t_uint32)((t_float32)((t_float32)2 * value_f32) / (t_float32)arrTim_u32);
                }
                else 
                {
                    *f_value_pu32 = 0;
                    Ret_e = RC_WARNING_NOT_ALLOWED;
                }
                break;

            case FMKIO_FREQ_MEAS_FREQ:
                if(value_f32 > (t_float32)0.0f)
                {
                    *f_value_pu32 = (t_uint32)((t_float32)arrTim_u32 / (t_float32)((t_float32)2 * value_f32));
                }
                else    
                {
                    *f_value_pu32 = 0;
                    Ret_e = RC_WARNING_NOT_ALLOWED;
                }
                
                break;
            case FMKIO_FREQ_MEAS_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_Get_OutDigSigValue
 *********************************/
t_eReturnCode FMKIO_Get_OutDigSigValue(t_eFMKIO_OutDigSig f_signal_e, t_eFMKIO_DigValue *f_value_pe)
{
    t_eReturnCode Ret_e = RC_OK;
    GPIO_TypeDef *bspGpio_ps = (GPIO_TypeDef *)NULL;
    GPIO_PinState bspSigValue_e;

    if (f_signal_e >= FMKIO_OUTPUT_SIGDIG_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_value_pe == (t_eFMKIO_DigValue *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_OutDigSigInfo_as[f_signal_e].IsSigConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }
    if(g_FmkIO_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = FMKIO_Get_BspGpioPort(c_OutDigSigBspMap_as[f_signal_e].HwGpio_e, &bspGpio_ps);
        if (Ret_e == RC_OK)
        {
            bspSigValue_e = HAL_GPIO_ReadPin(bspGpio_ps, c_BspPinMapping_ua16[c_OutDigSigBspMap_as[f_signal_e].HwPin_e]);
            switch (bspSigValue_e)
            {
            case GPIO_PIN_RESET:
                *f_value_pe = FMKIO_DIG_VALUE_LOW;
                break;
            case GPIO_PIN_SET:
                *f_value_pe = FMKIO_DIG_VALUE_HIGH;
                break;
            default:
                *f_value_pe = FMKIO_DIG_VALUE_LOW;
                Ret_e = RC_WARNING_WRONG_RESULT;
                break;
            }
        }
    }
    return Ret_e;
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
/*********************************
 * s_FMKIO_PreOperational
 *********************************/
static t_eReturnCode s_FMKIO_PreOperational(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxSigFreq_u8 = 0;
    t_uint8 idxEcdr_u8 = 0;
    t_eFMKTIM_InterruptLineIO IOLine_e;
    t_eFMKTIM_EcdrOpe EncoderTimerOpe_e;
    t_sFMKTIM_ICOpe ICOpe_s;
    t_sFMKTIM_ICValue ICValue_s;
    t_uint8 maskUpdate_u8 = (t_uint8)0;

    //----- Start Frequency Measurement -----//
    for(idxSigFreq_u8 = (t_uint8)0 ;
        (idxSigFreq_u8 < FMKIO_INPUT_SIGFREQ_NB) 
    &&  (Ret_e == RC_OK); 
        idxSigFreq_u8++)
    {
        if(g_InFreqSigInfo_as[idxSigFreq_u8].IsSigConfigured_b == (t_bool)True)
        {
            ICOpe_s.IcState_e = FMKTIM_IC_STATE_ENABLE;
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_IC_STATE);
            
            IOLine_e = (t_eFMKTIM_InterruptLineIO)c_InFreqSigBspMap_as[idxSigFreq_u8].ITLine_u8;
            Ret_e = FMKTIM_Set_ICLineValue(     IOLine_e,
                                                ICOpe_s,
                                                maskUpdate_u8);

            //---- get ARR &Frequency value to calculate the input frequency -----//
            maskUpdate_u8 = (t_uint8)0;
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_IC_ARR_REGISTER);
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_IC_FREQUENCY);

            Ret_e = FMKTIM_Get_ICLineValue(     IOLine_e,
                                                &ICValue_s,
                                                maskUpdate_u8);

            if(Ret_e == RC_OK)
            {
                g_InFreqSigInfo_as[idxSigFreq_u8].TimARRValue_u32 = ICValue_s.ARR_Register_u32;
                g_InFreqSigInfo_as[idxSigFreq_u8].TimFreqValue_u32 = ICValue_s.frequency_u32;
            }
        }
    }

    //----- Start Encoder Measurement -----//
    for(idxEcdr_u8 = (t_uint8)0 ;
        (idxEcdr_u8 < FMKIO_INPUT_ENCODER_NB)
    &&  (Ret_e == RC_OK); 
        idxEcdr_u8++)
    {
        if(g_InEcdrSigInfo_as[idxEcdr_u8].isConfigured_b == (t_bool)True)
        {
        
            Ret_e = s_FMKIO_Get_EcdrTimerMode(g_InEcdrSigInfo_as[idxEcdr_u8].EcdrOpe,
                                            &EncoderTimerOpe_e);
            if(Ret_e == RC_OK)
            {
                IOLine_e = c_FmkIo_InEcdrSigBspCfg_as[idxEcdr_u8].ITLine_e;
                Ret_e = FMKTIM_Set_EcdrLineState(  IOLine_e,
                                                    EncoderTimerOpe_e);
            }

            //---- Activate Fast task for Encoder Managment ----//
            Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_FMK_IO, APPSYS_FAST_TASK_ENABLE);

            if(Ret_e == RC_OK)
            {
                g_fastTaskEcdrStatus_b = (t_bool)True;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKIO_Operational
 *********************************/
static t_eReturnCode s_FMKIO_Operational(void)
{
    t_eReturnCode Ret_e = RC_OK;
    static t_uint32 s_SavedTime_u32 = 0;
    t_uint32 currentTime_u32 = 0;

    FMKCPU_GetTick(&currentTime_u32);
    if((currentTime_u32 - s_SavedTime_u32) > (t_uint32)FMKIO_TIME_BTWN_DIAG_MS)
    {//perform diag on timer / chnl used
        s_SavedTime_u32 = currentTime_u32;
        Ret_e = s_FMKIO_PerformDiagnostic();
    }

    Ret_e = s_FMKIO_SigFreqCalibOpe();
    
    return Ret_e;
}

/*********************************
 * s_FMKIO_PerformDiagnostic
 *********************************/
static t_eReturnCode s_FMKIO_PerformDiagnostic(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLI_u8;
    t_uint16 cpuChnlStatus_u16;
    t_uint16 adcChnlStatus_u16;
    t_uint8 ITLineVal_u8;
    t_eFMKTIM_InterruptLineType ITLineType_e;

    //------perform diag for PWM signal configuration------//
    for(LLI_u8 = (t_uint8)0 ; (LLI_u8 < FMKIO_OUTPUT_SIGPWM_NB) ; LLI_u8++)
    {
        if(g_OutPwmSigInfo_as[LLI_u8].IsSigConfigured_b == (t_bool)True)
        {

            //------update Information------//
            ITLineVal_u8 = (t_uint8)c_OutPwmSigBspMap_as[LLI_u8].ITLine_u8;
            ITLineType_e = FMKTIM_INTERRUPT_LINE_TYPE_IO;
            //------Get Error Status------//
            Ret_e = FMKTIM_Get_LineErrorStatus(ITLineType_e, 
                                                  ITLineVal_u8,
                                                  &cpuChnlStatus_u16);
            if((Ret_e == RC_OK)
            && (GETBIT(cpuChnlStatus_u16, FMKTIM_ERRSTATE_OK) != BIT_IS_SET_16B)
            && (g_OutPwmSigInfo_as[LLI_u8].sigError_cb != (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION))
            {
                g_OutPwmSigInfo_as[LLI_u8].sigError_cb( FMKIO_SIGTYPE_OUTPUT_PWM, 
                                                        LLI_u8,
                                                        cpuChnlStatus_u16, 
                                                        0);
            }
        }
    }
    // perform diag for Freqency signal configuration 
    for(LLI_u8 = (t_uint8)0 ; (LLI_u8 < FMKIO_INPUT_SIGFREQ_NB) ; LLI_u8++)
    {
        if(g_InFreqSigInfo_as[LLI_u8].IsSigConfigured_b == (t_bool)True)
        {
            //------update Information------//
            ITLineVal_u8 = (t_uint8)c_InFreqSigBspMap_as[LLI_u8].ITLine_u8;
            ITLineType_e = FMKTIM_INTERRUPT_LINE_TYPE_IO;
            //------Get Error Status------//
            Ret_e = FMKTIM_Get_LineErrorStatus(  ITLineType_e, 
                                                    ITLineVal_u8,
                                                    &cpuChnlStatus_u16);

            if((Ret_e == RC_OK)
            && (GETBIT(cpuChnlStatus_u16, FMKTIM_ERRSTATE_OK) !=  BIT_IS_SET_16B)
            && (g_InFreqSigInfo_as[LLI_u8].sigError_cb != (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION))
            {
                g_InFreqSigInfo_as[LLI_u8].sigError_cb( FMKIO_SIGTYPE_INPUT_FREQ,
                                                        LLI_u8,
                                                        cpuChnlStatus_u16,
                                                        0);
            }
        }
    } 
    //------perform diag for Analog signal configuration ------//
    for(LLI_u8 = (t_uint8)0 ; (LLI_u8 < FMKIO_INPUT_SIGANA_NB) ; LLI_u8++)
    {
        if(g_InAnaSigInfo_as[LLI_u8].IsSigConfigured_b == (t_bool)True)
        {
            Ret_e = FMKCDA_Get_AdcError(    c_InAnaSigBspMap_as[LLI_u8].adc_e,
                                            &adcChnlStatus_u16);

            if((Ret_e == RC_OK)
            && (GETBIT(adcChnlStatus_u16, FMKCDA_ERRSTATE_OK) == BIT_IS_RESET_16B)
            && (g_InAnaSigInfo_as[LLI_u8].sigError_cb != (t_cbFMKIO_SigErrorMngmt *)NULL_FUNCTION))
            {
                g_InAnaSigInfo_as[LLI_u8].sigError_cb(  FMKIO_SIGTYPE_INPUT_ANA,
                                                        LLI_u8,
                                                        adcChnlStatus_u16,
                                                        0);
            }
        }
    }
    
    return Ret_e;
}

/*********************************
 * s_FMKIO_SigFreqCalibOpe
 *********************************/
static t_eReturnCode s_FMKIO_SigFreqCalibOpe(void)
{
    static t_bool s_performFreqLogic_b = True;
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxFreq_u8;
    t_uint32 multiplierFreq_u32;
    t_eFMKTIM_InterruptLineIO freqLine_e;
    t_sFMKTIM_ICOpe ICValue_s;
    t_uint8 maskOpe_u8 = (t_uint8)0;

    if(s_performFreqLogic_b == (t_bool)True)
    {
        s_performFreqLogic_b = (t_bool)False;
        for(idxFreq_u8 = (t_uint8)0 ; idxFreq_u8 < FMKIO_INPUT_SIGFREQ_NB ; idxFreq_u8 ++)
        {
            if(g_InFreqSigInfo_as[idxFreq_u8].IsSigConfigured_b == (t_bool)True)
            {
                s_performFreqLogic_b = (t_bool)True;
                //---- check frequency Input Compare validity -----//
                switch (g_InFreqSigInfo_as[idxFreq_u8].meas_e)
                {
                    case FMKIO_FREQ_MEAS_FREQ:
                    case FMKIO_FREQ_MEAS_PERIOD:
                    {
                        if((g_InFreqSigInfo_as[idxFreq_u8].value_u32 < (t_uint32)FMKIO_FREQ_MIN_CC_VALUE)
                        && (g_InFreqSigInfo_as[idxFreq_u8].value_u32 > (t_uint32)0))
                        {
                            multiplierFreq_u32 = (t_uint32)((t_float32)(FMKIO_FREQ_MIN_CC_VALUE / 
                                                                (t_float32)g_InFreqSigInfo_as[idxFreq_u8].value_u32));

                            if(multiplierFreq_u32 < (t_uint32)2)
                            {
                                multiplierFreq_u32 = (t_uint32)2;
                            }
                            //----- Set Information for frequency -----//
                            freqLine_e = (t_eFMKTIM_InterruptLineIO)c_InFreqSigBspMap_as[idxFreq_u8].ITLine_u8;
                            SETBIT_8B(maskOpe_u8, FMKTIM_BIT_IC_FREQUENCY);

                            ICValue_s.frequency_u32 = (t_uint32)(g_InFreqSigInfo_as[idxFreq_u8].TimFreqValue_u32 *
                                                                    multiplierFreq_u32);
                            //---- Set new frequency ----//
                            Ret_e = FMKTIM_Set_ICLineValue(     freqLine_e,
                                                                ICValue_s,
                                                                maskOpe_u8);
                            if(Ret_e == RC_OK)
                            {
                                g_InFreqSigInfo_as[idxFreq_u8].TimFreqValue_u32 = ICValue_s.frequency_u32;
                            }
                        }
                        else 
                        {
                            if(g_InFreqSigInfo_as[idxFreq_u8].isSigCalib_b == (t_bool)False)
                            {
                                g_InFreqSigInfo_as[idxFreq_u8].isSigCalib_b = (t_bool)True;
                            }
                            Ret_e = RC_OK;  
                        }
                    }
                    case FMKIO_FREQ_MEAS_COUNT:
                    {
                        if(g_InFreqSigInfo_as[idxFreq_u8].isSigCalib_b == (t_bool)False)
                        {
                            g_InFreqSigInfo_as[idxFreq_u8].isSigCalib_b = (t_bool)True;
                        }
                        Ret_e = RC_OK;
                        break;
                    }
                    case FMKIO_FREQ_MEAS_NB:
                    default:
                    {
                        Ret_e = RC_WARNING_NO_OPERATION;
                    }
                }
            }
        }
    }

    return Ret_e;
}
/*********************************
 * s_FMKIO_Get_EcdrTimerMode
 *********************************/
static t_eReturnCode s_FMKIO_Get_EcdrTimerMode(t_eFMKIO_EcdrStartOpe f_StartOpeMode_e, t_eFMKTIM_EcdrOpe * f_EdrTimerOpe_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_StartOpeMode_e >= FMKIO_ENCODER_START_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_EdrTimerOpe_pe == (t_eFMKTIM_EcdrOpe *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_StartOpeMode_e)
        {
            case FMKIO_ENCODER_START_POS:
                *f_EdrTimerOpe_pe = FMKTIM_ECDR_OPE_START_TI1;
                break;
            case FMKIO_ENCODER_START_DIR:
                *f_EdrTimerOpe_pe = FMKTIM_ECDR_OPE_START_TI2;
                break;
            case FMKIO_ENCODER_START_BOTH:
                *f_EdrTimerOpe_pe = FMKTIM_ECDR_OPE_START_BOTH;
                break;
            case FMKIO_ENCODER_START_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }   

    return Ret_e;
}

/*********************************
 * s_FMKIO_FastTask
 *********************************/
static void s_FMKIO_FastTask(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_bool stopFastTask_b = (t_bool)True;
    
    if(g_fastTaskPwmStatus_b == (t_bool)True)
    {
        Ret_e = s_FMKIO_FastTask_PwmMngmt();
        stopFastTask_b = (t_bool)False;
    }
    if(g_fastTaskEcdrStatus_b == (t_bool)True)
    {
        Ret_e = s_FMKIO_FastTask_EcdrMngmt();
        stopFastTask_b = (t_bool)False;
    }
    if(Ret_e != RC_OK)
    {
        ASSERT((t_uint16)Ret_e);
    }
    if(stopFastTask_b == (t_bool)True)
    {
        Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_FMK_IO, APPSYS_FAST_TASK_DISABLE);
    }

    return;
}

/*********************************
 * s_FMKIO_FastTask_PwmMngmt
 *********************************/
static t_eReturnCode s_FMKIO_FastTask_PwmMngmt(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_float32 computeRampVal_f32;
    t_uint32 computeFreq_u32;
    t_uint16 computeDc_u16;
    t_uint8 idxSigPwm_u8;
    t_sFMKIO_PwmSigInfo * pwmSigInfo_ps;
    t_uint32 mskPwmActivity_u32 = (t_uint32)0;

    for(idxSigPwm_u8 = (t_uint8)0 ; idxSigPwm_u8 < FMKIO_OUTPUT_SIGPWM_NB ; idxSigPwm_u8++)
    {
        computeRampVal_f32 = (t_float32)0.0f;
        pwmSigInfo_ps = (t_sFMKIO_PwmSigInfo *)(&g_OutPwmSigInfo_as[idxSigPwm_u8]);

        if(pwmSigInfo_ps->ctrlType_e == FMKIO_PWM_CTRL_TYPE_FREQ)
        {
            Ret_e = LIBRamp_Compute(pwmSigInfo_ps->rampId_u8,
                                    (t_float32)pwmSigInfo_ps->frequencyReq_u32,
                                    (t_float32 *)(&computeRampVal_f32));
            if(Ret_e == RC_OK)
            {
                //---- Make the output change now ----//
                computeFreq_u32 = (t_uint32)(computeRampVal_f32);
                Ret_e = s_FMKIO_MngSigPwmWaveForm(  (t_eFMKIO_OutPwmSig)idxSigPwm_u8,
                                                    (t_uint16 *)(&pwmSigInfo_ps->reqDutycycle_u16),
                                                    (t_uint32 *)(&computeFreq_u32),
                                                    (t_uint32 *)(NULL));
            }
            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint16)(Ret_e));
            }
            if(pwmSigInfo_ps->frequencyReq_u32 != (t_uint32)computeRampVal_f32)
            {
                SETBIT_8B(mskPwmActivity_u32, idxSigPwm_u8);
            }
        }
        else if(pwmSigInfo_ps->ctrlType_e == FMKIO_PWM_CTRL_TYPE_DC)
        {
            Ret_e = LIBRamp_Compute(pwmSigInfo_ps->rampId_u8,
                                    (t_float32)pwmSigInfo_ps->reqDutycycle_u16,
                                    (t_float32 *)(&computeRampVal_f32));
            if(Ret_e == RC_OK)
            {
                //---- Make the output change now ----//
                computeDc_u16 = (t_uint16)computeRampVal_f32;
                Ret_e = s_FMKIO_MngSigPwmWaveForm(  (t_eFMKIO_OutPwmSig)idxSigPwm_u8,
                                                    (t_uint16 *)(&computeDc_u16),
                                                    (t_uint32 *)(NULL),
                                                    (t_uint32 *)(NULL));
            }
            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint16)(Ret_e));
            }
            if(pwmSigInfo_ps->reqDutycycle_u16 != (t_uint16)computeRampVal_f32)
            {
                SETBIT_8B(mskPwmActivity_u32, idxSigPwm_u8);
            }
        }
        else 
        {
            ASSERT((t_uint32)pwmSigInfo_ps->ctrlType_e);
        }
    }
    if(mskPwmActivity_u32 == (t_uint32)0)
    {
        g_fastTaskPwmStatus_b = (t_bool)False;
    }

    return Ret_e;
}

/*********************************
 * s_FMKIO_FastTask_PwmMngmt
 *********************************/
static t_eReturnCode s_FMKIO_FastTask_EcdrMngmt(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxEcdr_u8;
    t_sFMKIO_InEcdrSigInfo ecdrSigCfg_s;
    t_uint32 hwDirection_u32;
    t_sint32 halfRange_s32;
    t_uint32 currPosition_u32;
    t_sint32 Sodelta_s32;

    for(idxEcdr_u8 = (t_uint8)0 ; 
    (idxEcdr_u8 < FMKIO_INPUT_ENCODER_NB)
    && (Ret_e == RC_OK) ; 
    idxEcdr_u8++)
    {
        Ret_e = SMB_Read(&g_sfmb_EcdrInfo_as[idxEcdr_u8], &ecdrSigCfg_s);
        
        if((Ret_e == RC_OK)
        && (ecdrSigCfg_s.isConfigured_b == (t_bool)True))
        {
            hwDirection_u32 = (t_uint32)0;
            currPosition_u32 = (t_uint32)0;
            Sodelta_s32 = (t_sint32)0;
            //---- Perform Logic ----//
            Ret_e = FMKTIM_Get_EcdrLineValue(c_FmkIo_InEcdrSigBspCfg_as[idxEcdr_u8].ITLine_e,
                                            &currPosition_u32,
                                            &hwDirection_u32);

            if(hwDirection_u32 == (t_uint32)0x01)
            {
                ecdrSigCfg_s.direction_e = FMKIO_ENCODER_DIR_FORWARD;
            }
            else 
            {
                ecdrSigCfg_s.direction_e = FMKIO_ENCODER_DIR_BACKWARD;
            }
            
            Sodelta_s32 = (t_sint32)(currPosition_u32 - ecdrSigCfg_s.lastPos_u32);
            halfRange_s32 = ((FMKIO_ECDR_ARR_VALUE + 1) / 2);

            if(Sodelta_s32 > halfRange_s32)
            {
                Sodelta_s32 -= (FMKIO_ECDR_ARR_VALUE + 1);
            }
            else if (Sodelta_s32 < -halfRange_s32)
            {
                Sodelta_s32 += (FMKIO_ECDR_ARR_VALUE + 1);
            }           
             
            //---- update sofware variable ----//
            // in 
            ecdrSigCfg_s.SoPosition_f32 += 
                ((t_float32)((t_float32)Sodelta_s32) 
                * CST_2PI_MRAD) 
                / ((t_float32)ecdrSigCfg_s.PPRValue_u16 * (t_float32)ecdrSigCfg_s.MPR_u8);
              
            if(ecdrSigCfg_s.SoPosition_f32 < 0.0f)
            {
                ecdrSigCfg_s.SoPosition_f32 = 0.0f;
            }
            else if(ecdrSigCfg_s.SoPosition_f32 > CST_2PI_MRAD)
            {
                ecdrSigCfg_s.SoPosition_f32 = CST_2PI_MRAD;
            }
            //---- speed in mmrad / sec ----//
            //---- calculate a positive speed ----//
            if(Sodelta_s32 < (t_sint32)0)
            {
                Sodelta_s32 *= (t_sint32)(-1);
            }

            ecdrSigCfg_s.SoSpeed_f32 = ((t_float32)Sodelta_s32 * (1000.0f / ((t_float32)APPSYS_ELASPED_TIME_FASTTASK))) / 
                                        ((t_float32)ecdrSigCfg_s.PPRValue_u16 * (t_float32)ecdrSigCfg_s.MPR_u8);
            ecdrSigCfg_s.SoSpeed_f32 *= (t_float32)1000.0f;

            //---- save current encoder position ----//
            ecdrSigCfg_s.lastPos_u32 = currPosition_u32;

            Ret_e = SMB_Write(&g_sfmb_EcdrInfo_as[idxEcdr_u8], &ecdrSigCfg_s);

            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint32)Ret_e);
            }
        }
    }


    return Ret_e;
}
/*********************************
 * s_FMKIO_MngSigFrequency
 *********************************/
static t_eReturnCode s_FMKIO_MngSigFrequency(t_eFMKTIM_InterruptLineType f_InterruptType_e, t_uint8 f_InterruptLine_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_ICValue IcLineValues_s;
    t_uint32 currentCapture_u32 = 0;
    t_uint32 lastCapture_u32 = 0;
    t_uint32 captureDifference_u32 = 0;
    t_uint8 maskEvnt_u8 = 0;
    t_uint8 LLI_u8 = 0;
    t_eFMKIO_InFreqSig freqSig_e = FMKIO_INPUT_SIGFREQ_NB;
    t_uint8 ItLine_u8;

    if (f_InterruptType_e != FMKTIM_INTERRUPT_LINE_TYPE_IO)
    { 
        Ret_e = RC_ERROR_PARAM_INVALID;
    }

    if (Ret_e == RC_OK)
    {
        //-------------Know which signal made the interruption -------------//
        for (LLI_u8 = (t_uint8)0; LLI_u8 < FMKIO_INPUT_SIGFREQ_NB; LLI_u8++)
        {
            if(f_InterruptLine_u8 == (t_uint8)c_InFreqSigBspMap_as[LLI_u8].ITLine_u8)
            {
                freqSig_e = (t_eFMKIO_InFreqSig)LLI_u8;
                break;
            }
        }

        if (freqSig_e != FMKIO_INPUT_SIGFREQ_NB)
        {
            g_InFreqSigInfo_as[freqSig_e].savedValue_32 = g_InFreqSigInfo_as[freqSig_e].value_u32;

            switch (g_InFreqSigInfo_as[freqSig_e].meas_e)
            {
                case FMKIO_FREQ_MEAS_COUNT:
                    g_InFreqSigInfo_as[freqSig_e].value_u32 += (t_uint32)1;
                    break;

                case FMKIO_FREQ_MEAS_PERIOD:
                case FMKIO_FREQ_MEAS_FREQ:
                    //-------------Read value for channel linked to the signal-------------//
                    ItLine_u8 = (t_uint8)f_InterruptLine_u8;
                    SETBIT_8B(maskEvnt_u8, FMKTIM_BIT_IC_CCRX_REGISTER);
                    Ret_e = FMKTIM_Get_ICLineValue( ItLine_u8, 
                                                    &IcLineValues_s,
                                                    maskEvnt_u8);

                    if (Ret_e == RC_OK)
                    {
                        currentCapture_u32 = IcLineValues_s.CCRxRegister_u16;
                        lastCapture_u32 = g_InFreqLastCapture_ua32[freqSig_e];

                        // Check for overflow and calculate capture difference
                        if (IcLineValues_s.CCRxRegister_u16 < lastCapture_u32)
                        {
                            // Timer has overflowed, calculate the correct difference
                            //overflowCount_u32 = 1; // Increment overflow count (1 overflow)
                            captureDifference_u32 = ((g_InFreqSigInfo_as[freqSig_e].TimARRValue_u32 
                                                        - lastCapture_u32) + currentCapture_u32 + 1);
                        }
                        else
                        {
                            //overflowCount_u32 = 0; // No overflow
                            captureDifference_u32 = currentCapture_u32 - lastCapture_u32;
                        }

                        //-------------set flag to writing equals true-------------//
                        g_InFreqSigInfo_as[freqSig_e].FlagValueWriting_b = (t_bool)True;

                        //-------------saved difference in freq sig structure, calculation will not be made in interruption-------------//
                        g_InFreqSigInfo_as[freqSig_e].value_u32 = captureDifference_u32;

                        //-------------reset flag-------------//
                        g_InFreqSigInfo_as[freqSig_e].FlagValueWriting_b = (t_bool)False;

                        //-------------saved last capture value-------------//
                        g_InFreqLastCapture_ua32[freqSig_e] = currentCapture_u32;

                        // Optionally: Account for overflow in frequency calculation
                        //g_InFreqSigInfo_as[freqSig_e].overflowCount_u32 = overflowCount_u32; // Store overflow count if needed
                    }
                    break;

                case FMKIO_FREQ_MEAS_NB:
                default:
                    Ret_e = RC_WARNING_NO_OPERATION;
                    break;
            }
        }
        else
        {
            Ret_e = RC_ERROR_LIMIT_REACHED;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKIO_MngSigPwmWaveForm
 *********************************/
static t_eReturnCode s_FMKIO_MngSigPwmWaveForm( t_eFMKIO_OutPwmSig f_signal_e,
                                                t_uint16 * f_dutyCycle_pu16,
                                                t_uint32 * f_frequency_pu32,
                                                t_uint32 * f_pulses_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_PwmOpe    pwmTimOpe_s;
    t_sFMKHRT_PwmOpeVal pwmHrTImOpe_s;
    t_uint8 ITLineIO_u8;
    t_eFMKIO_OutTimerCfg timOrgn_e;
    t_uint8 maskUpdate_u8 = (t_uint8)0;

    ITLineIO_u8 = c_OutPwmSigBspMap_as[f_signal_e].ITLine_u8;
    timOrgn_e = c_OutPwmSigBspMap_as[f_signal_e].TimOrigin_e;

    if((timOrgn_e == FMKIO_ITLINE_TYPE_BSCTIM)
    || (timOrgn_e == FMKIO_ITLINE_TYPE_ADVTIM))
    {
        //---- set to default info ----//
        pwmTimOpe_s.dutyCycle_u16 = (t_uint16)0;
        pwmTimOpe_s.frequency_u32 = (t_uint32)0;
        pwmTimOpe_s.nbPulses_u16 = (t_uint16)0;

        if(f_dutyCycle_pu16 != (t_uint16 *)NULL)
        {
            pwmTimOpe_s.dutyCycle_u16 = (t_uint16)(*f_dutyCycle_pu16);
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_PWM_DUTYCYCLE);
        }
        if(f_frequency_pu32 != (t_uint32 *)NULL)
        {
            pwmTimOpe_s.frequency_u32 = (t_uint32)(*f_frequency_pu32);
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_PWM_FREQUENCY);
        }
        if(f_pulses_pu32 != (t_uint32 *)NULL)
        {
            pwmTimOpe_s.nbPulses_u16 = (t_uint16)(*f_pulses_pu32);
            SETBIT_8B(maskUpdate_u8, FMKTIM_BIT_PWM_NB_PULSES);
        }

        Ret_e = FMKTIM_Set_PwmLineValue(ITLineIO_u8,
                                        pwmTimOpe_s,
                                        maskUpdate_u8);
    }
    else if(timOrgn_e == FMKIO_ITLINE_TYPE_HRTIM)
    {
        //---- set to default info ----//
        pwmHrTImOpe_s.dutyCycle_u16 = (t_uint16)0;
        pwmHrTImOpe_s.frequency_u32 = (t_uint32)0;
        pwmHrTImOpe_s.nbPulses_u16 = (t_uint16)0;

        if(f_dutyCycle_pu16 != (t_uint16 *)NULL)
        {
            pwmHrTImOpe_s.dutyCycle_u16 = (t_uint16)(*f_dutyCycle_pu16);
            SETBIT_8B(maskUpdate_u8, FMKHRT_BIT_PWM_DUTYCYCLE);
        }
        if(f_frequency_pu32 != (t_uint32 *)NULL)
        {
            pwmHrTImOpe_s.frequency_u32 = (t_uint32)(*f_frequency_pu32);
            SETBIT_8B(maskUpdate_u8, FMKHRT_BIT_PWM_FREQUENCY);
        }
        if(f_pulses_pu32 != (t_uint32 *)NULL)
        {
            pwmHrTImOpe_s.nbPulses_u16 = (t_uint16)(*f_pulses_pu32);
            SETBIT_8B(maskUpdate_u8, FMKHRT_BIT_PWM_NB_PULSES);
        }

        Ret_e = FMKHRT_SetPwmLineWaveform(  (t_eFMKHRT_HighResLine)ITLineIO_u8,
                                            pwmHrTImOpe_s,
                                            maskUpdate_u8);
    }
    else 
    {
        Ret_e = RC_ERROR_MISSING_CONFIG;
        ASSERT((t_uint16)Ret_e);
    }

    return Ret_e;
}
/*********************************
 * s_FMKIO_basicAdvTimerCallback
 *********************************/
static void s_FMKIO_basicAdvTimerCallback(t_eFMKTIM_InterruptLineType f_InterruptType_e, t_uint8 f_InterruptLine_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLI_u8= 0;
    t_eFMKIO_OutPwmSig PwmSig_e = FMKIO_OUTPUT_SIGPWM_NB;

    if (f_InterruptType_e != FMKTIM_INTERRUPT_LINE_TYPE_IO)
    { 
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        //-------------Know wich signal made the interruption -------------//
        for(LLI_u8 = (t_uint8)0 ; LLI_u8 < FMKIO_OUTPUT_SIGPWM_NB ; LLI_u8++)
        {
            if(f_InterruptLine_u8 == (t_uint8)c_OutPwmSigBspMap_as[LLI_u8].ITLine_u8)
            {
                if((c_OutPwmSigBspMap_as[LLI_u8].TimOrigin_e == FMKIO_ITLINE_TYPE_ADVTIM)
                || (c_OutPwmSigBspMap_as[LLI_u8].TimOrigin_e == FMKIO_ITLINE_TYPE_BSCTIM))
                {
                    PwmSig_e = (t_eFMKIO_OutPwmSig)LLI_u8;
                    break;
                }
            }
        }
        if(PwmSig_e != FMKIO_OUTPUT_SIGPWM_NB)
        {
            if(g_OutPwmSigInfo_as[PwmSig_e].pulseEvnt_pcb != NULL_FUNCTION)
            {
                g_OutPwmSigInfo_as[PwmSig_e].pulseEvnt_pcb(PwmSig_e);
            }
        }
    }

    return;
}
/*********************************
 * s_FMKIO_HighResTimerCallback
 *********************************/
static void s_FMKIO_HighResTimerCallback(t_eFMKHRT_HighResLine f_HrLine_e, t_eFMKHRT_HrLineEvntCb f_Evnt_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 LLI_u8= 0;  
    t_eFMKIO_OutPwmSig PwmSig_e = FMKIO_OUTPUT_SIGPWM_NB;

    if((f_HrLine_e >= FMKHRT_HR_LINE_NB)
    || (f_Evnt_e != FMKHRT_HR_LINE_EVNT_CB_PULSE_FINISH))
    { 
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        //-------------Know wich signal made the interruption -------------//
        for(LLI_u8 = (t_uint8)0 ; LLI_u8 < FMKIO_OUTPUT_SIGPWM_NB ; LLI_u8++)
        {
            if((t_uint8)f_HrLine_e == (t_uint8)c_OutPwmSigBspMap_as[LLI_u8].ITLine_u8)
            {
                if(c_OutPwmSigBspMap_as[LLI_u8].TimOrigin_e == FMKIO_ITLINE_TYPE_HRTIM)
                {
                    PwmSig_e = (t_eFMKIO_OutPwmSig)LLI_u8;
                    break;
                }
            }
        }
        if(PwmSig_e != FMKIO_OUTPUT_SIGPWM_NB)
        {
            if(g_OutPwmSigInfo_as[PwmSig_e].pulseEvnt_pcb != NULL_FUNCTION)
            {
                g_OutPwmSigInfo_as[PwmSig_e].pulseEvnt_pcb(PwmSig_e);
            }
        }
    }

    return;
}
/*********************************
 * s_FMKIO_Get_BspPullMode
 *********************************/
static t_eReturnCode s_FMKIO_Get_BspPullMode(t_eFMKIO_PullMode f_pull_e, t_uint32 *f_bspPull_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_pull_e >= FMKIO_PULL_MODE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspPull_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_pull_e)
        {
        case FMKIO_PULL_MODE_DISABLE:
            *f_bspPull_pu32 = GPIO_NOPULL;
            break;
        case FMKIO_PULL_MODE_DOWN:
            *f_bspPull_pu32 = GPIO_PULLDOWN;
            break;
        case FMKIO_PULL_MODE_UP:
            *f_bspPull_pu32 = GPIO_PULLUP;
            break;
        case FMKIO_PULL_MODE_NB:
        default:
            Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
            break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKIO_Get_BspPullMode
 *********************************/
static t_eReturnCode s_FMKIO_Get_BspSpdMode(t_eFMKIO_SpdMode f_spd_e, t_uint32 *f_bspSpd_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_spd_e >= FMKIO_SPD_MODE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspSpd_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_spd_e)
        {
        case FMKIO_SPD_MODE_LOW:
            *f_bspSpd_pu32 = GPIO_SPEED_FREQ_LOW;
            break;
        case FMKIO_SPD_MODE_MEDIUM:
            *f_bspSpd_pu32 = GPIO_SPEED_FREQ_MEDIUM;
            break;
        case FMKIO_SPD_MODE_HIGH:
            *f_bspSpd_pu32 = GPIO_SPEED_FREQ_HIGH;
            break;
        case FMKIO_SPD_MODE_NB:
        default:
            Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
            break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKIO_Get_BspTriggerMode
 *********************************/
static t_eReturnCode s_FMKIO_Get_BspTriggerMode(t_eFMKIO_SigTrigCptr f_trigger_e, t_uint32 *f_bspTrigger_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_trigger_e >= FMKIO_STC_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspTrigger_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_trigger_e)
        {
        case FMKIO_STC_RISING_EDGE:
            *f_bspTrigger_pu32 = GPIO_MODE_IT_RISING;
            break;
        case FMKIO_STC_FALLING_EDGE:
            *f_bspTrigger_pu32 = GPIO_MODE_IT_FALLING;
            break;
        case FMKIO_STC_BOTH_EDGE:
            *f_bspTrigger_pu32 = GPIO_MODE_IT_RISING_FALLING;
            break;
        case FMKIO_STC_NB:
        default:
            Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
            break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKIO_Set_BspSigCfg
 *********************************/
static t_eReturnCode s_FMKIO_Set_BspSigCfg(t_eFMKIO_GpioPort f_gpioPort_e,
                                            t_eFMKIO_BspGpioPin f_pin_e,
                                            t_uint32 f_mode_u32,
                                            t_eFMKIO_PullMode f_pull_e,
                                            t_eFMKIO_SpdMode f_spd_e,
                                            t_uint8 f_AltFunc_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    GPIO_TypeDef *bspGpio_ps = (GPIO_TypeDef *)NULL;
    GPIO_InitTypeDef bspInit_s;
    t_uint32 bspPull_32 = 0;
    t_uint32 bspSpd_u32 = 0;
 
    if ((f_gpioPort_e >= FMKIO_GPIO_PORT_NB)
    ||  (f_pin_e >= FMKIO_GPIO_PIN_NB)
    ||  (f_pull_e >= FMKIO_PULL_MODE_NB)
    ||  (f_spd_e >= FMKIO_SPD_MODE_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        Ret_e = FMKIO_Get_BspGpioPort(f_gpioPort_e, &bspGpio_ps);
        if (Ret_e == RC_OK)
        {
            Ret_e = s_FMKIO_Get_BspPullMode(f_pull_e, &bspPull_32);
        }
        if (Ret_e == RC_OK)
        {
            Ret_e = s_FMKIO_Get_BspSpdMode(f_spd_e, &bspSpd_u32);
        }
        if (g_IsGpioClockEnable_ae[f_gpioPort_e] == FMKCPU_CLOCKPORT_OPE_DISABLE)
        {
            Ret_e = s_FMKIO_Set_GpioClkState(f_gpioPort_e, FMKCPU_CLOCKPORT_OPE_ENABLE);
        }
        if (Ret_e == RC_OK)
        {
            bspInit_s.Mode = (t_uint32)f_mode_u32;
            bspInit_s.Pin = (t_uint32)c_BspPinMapping_ua16[f_pin_e];
            bspInit_s.Pull = (t_uint32)bspPull_32;
            bspInit_s.Speed = (t_uint32)bspSpd_u32;
            if (f_AltFunc_u8 != (t_uint8)FMKIO_AF_UNUSED)
            {
                bspInit_s.Alternate = (t_uint32)f_AltFunc_u8;
            }
            HAL_GPIO_Init(bspGpio_ps, &bspInit_s);
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKIO_Set_GpioClkState
 *********************************/
static t_eReturnCode s_FMKIO_Set_GpioClkState(t_eFMKIO_GpioPort f_gpioPort_e, t_eFMKCPU_ClockPortOpe f_ope_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKCPU_ClockPort gpioClkPort_e;

    if (f_gpioPort_e >= FMKIO_GPIO_PORT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        //----- Get Rcc Clock For the GPIO Port -----//
        Ret_e = FMKIO_GetGpioRccClock(f_gpioPort_e, &gpioClkPort_e);

        if (Ret_e == RC_OK)
        {
            //----- Enable/Disable HArdware Clock -----//
            Ret_e = FMKCPU_Set_HwClock(gpioClkPort_e, f_ope_e);
        }
        if (Ret_e == RC_OK)
        {
            //----- Update Information -----//
            g_IsGpioClockEnable_ae[f_gpioPort_e] = f_ope_e;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_BspRqst_InterruptMngmt
 *********************************/
void FMKIO_BspRqst_InterruptMngmt(void)
{
    t_uint8 LLI_u8;
    t_eFMKIO_BspGpioPin pin_e;
    t_uint16 bspPin_u16;
    t_uint32 currentTick_u32 = 0;


    for(LLI_u8 = (t_uint8)0 ; LLI_u8 < FMKIO_INPUT_SIGEVNT_NB ; LLI_u8++)
    {
        pin_e = c_InEvntSigBspMap_as[LLI_u8].BasicCfg_s.HwPin_e;

        if(g_InEvntSigInfo_as[LLI_u8].IsSigConfigured_b == (t_bool)True)
        {
            bspPin_u16 = c_BspPinMapping_ua16[pin_e];

            if(__HAL_GPIO_EXTI_GET_IT(bspPin_u16) != 0x00u)
            { 
                //call user function if debouncer counter is passed
                FMKCPU_GetTick(&currentTick_u32);
                if((currentTick_u32 - g_lastTick_ua32[LLI_u8]) > g_InEvntSigInfo_as[LLI_u8].debcDelay_u32)
                {
                    g_lastTick_ua32[LLI_u8] = currentTick_u32;
                    __HAL_GPIO_EXTI_CLEAR_IT(bspPin_u16);
                    g_InEvntSigInfo_as[LLI_u8].EvntFunc_cb();
                }
            }
        }
    }

    return;
}
//********************************************************************************
//                      HAL_Callback Implementation
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
 *	@params[in]
 *
 *
 *
 */
