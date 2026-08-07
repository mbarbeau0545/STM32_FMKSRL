/*********************************************************************
 * @file        FMK_SRL.c
 * @brief       Template_BriefDescription.
 * @details     TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */






// ********************************************************************
// *                      Includes
// ********************************************************************
#include "./FMK_SRL.h"
#include "FMK_HAL/FMK_CPU/Src/FMK_CPU.h"
#include "FMK_HAL/FMK_IO/Src/FMK_IO.h"
#include "3_APP/APP_CTRL/APP_SYS/Src/APP_SYS.h"
#include "3_APP/APP_CTRL/APP_SDM/Src/APP_SDM.h"

#include "FMK_CFG/FMKCFG_ConfigFiles/FMKSRL_ConfigPrivate.h"
#include <stdarg.h>
#include <stdio.h>
#include "Library/SafeMem/SafeMem.h"
#include "Constant.h"
// ********************************************************************
// *                      Defines
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************
/**
 * @brief Union to define hardware serial handles for UART and USART.
 */
typedef union __t_uFMKSRL_HardwareSerial
{
    UART_HandleTypeDef  uartH_s;    /**< UART handle. */
    USART_HandleTypeDef usartH_s;   /**< USART handle. */
} t_uFMKSRL_HardwareHandle;


/**
 * @brief Enumeration to represent the status of a buffer.
 */
typedef enum __t_eFMKSRL_BufferStatus
{
    FMKSRL_BUFFSTATUS_READY = 0x0000U,            /**< Buffer is ready for use. */
    FMKSRL_BUFFSTATUS_BUSY = 0x0001U,             /**< Buffer is currently busy. */
    FMKSRL_BUFFSTATUS_MSG_CUT = 0x0002U,          /**< Message was cut in the buffer. */
    FMKSRL_BUFFSTATUS_MSG_PENDING = 0x0003U,      /**< Message is pending in the buffer. */
    FMKSRL_BUFFSTATUS_EMPTY = 0x0004U,            /**< Buffer is empty. */
    FMKSRL_BUFFSTATUS_OVERFLOW = 0x0005U,         /**< Buffer overflow occurred. */
    FMKSRL_BUFFSTATUS_ERROR = 0x0006U,            /**< Buffer encountered an error. */
} t_eFMKSRL_BufferStatus;


/**
 * @brief Enumeration to enable or disable timeout operations.
 */
typedef enum __t_eFMKSRL_TimeoutOpe
{
    FMKSRL_TIMEOUT_OPE_ACTIVATE = 0x00,     /**< Enable timeout operations. */
    FMKSRL_TIMEOUT_OPE_DISACTIVATE,         /**< Disable timeout operations. */

    FMKSRL_TIMEOUT_OPE_NB                   /**< Number of timeout operation types. */
} t_eFMKSRL_TimeoutOpe;

/**
 * @brief Enumeration for transmit callback events.
 */
typedef enum __t_eFMKSRL_BspCbTxEvnt
{
    FMKSRL_BSP_TX_CB_HALCPLT = 0x00,    /**< HAL transmit complete callback. */
    FMKSRL_BSP_TX_CB_CPLT,              /**< Transmit complete callback. */
    FMKSRL_BSP_TX_RX_CB_CPLT,           /**< Transmit and receive complete callback. */
    FMKSRL_BSP_TX_CB_NB                 /**< Number of transmit callback event types. */
} t_eFMKSRL_BspCbTxEvnt;

/**
 * @brief Enumeration for receive callback events.
 */
typedef enum __t_eFMKSRL_BspCbRxEvnt
{
    FMKSRL_BSP_RX_CB_HALCPLT = 0x00,    /**< HAL receive complete callback. */
    FMKSRL_BSP_RX_CB_CPLT,              /**< Receive complete callback. */
    FMKSRL_BSP_RX_CB_EVENT,             /**< General receive event callback. */

    FMKSRL_BSP_RX_CB_NB                 /**< Number of receive callback event types. */
} t_eFMKSRL_BspCbRxEvnt;

typedef enum 
{
    FMKSRL_BSP_ERR_CB_ABORT_ALL = 0x00,
    FMKSRL_BSP_ERR_CB_ABORT_TX,
    FMKSRL_BSP_ERR_CB_ABORT_RX,
    FMKSRL_BSP_ERR_CB_ERROR,

    FMKSRL_BSP_ERR_CB_NB,
} t_eFMKSRL_BspCbErrEvnt;
/* CAUTION : Automatic generated code section for Structure: Start */

/* CAUTION : Automatic generated code section for Structure: End */
//-----------------------------STRUCT TYPES---------------------------//

/**
 * @brief Buffer Information Structure
 */
typedef struct __t_sFMKSRL_BufferInfo
{
    t_uint8 * bufferAdd_pu8;        /**< Pointor to buffer address */
    t_uint16 buffferSize_u16;       /**< Buffer Size */
    t_uint16 bytesPending_u16;      /**< Bytes currently in the buffer*/
    t_uint16 bytesSending_u16;      /**< Bytes currently sending */
    t_uint16 writeIdx_u16;          /**< Buffer Write Index  */
    t_uint16 readIdx_u16;           /**< Buffer Read Index */
    t_uint16 status_u16;            /**< Buffer Status/Info */
} t_sFMKSRL_BufferInfo;

/**
 * @brief Transmit Line Information
 */
typedef struct __t_sFMKSRL_TxMngmt
{
    t_eFMKSRL_TxOpeMode                 OpeMode_e;          /** Transmit Ope Mode */
    t_eFMKSRL_BspTransmitOpe            bspTxOpe_e;         /**< Bsp transmit Ope Mode */
    t_sFMKSRL_BufferInfo                Buffer_s;           /**< Tx Buffer Information */
    t_bool                              RqstTxRxOpe_b;      /**< Flag to know if TxRX Ope is Requested */
    t_cbFMKSRL_TransmitMsgEvent       * TxUserCb_pcb;       /**< Tx User callback */
    t_bool                              NotifyUser_b;       /**< Wether or not we use Tx callback */
} t_sFMKSRL_TxMngmt;

/**
 * @brief Receive Line Information
 */
typedef struct __t_sFMKSRL_RxMngmt
{
    t_eFMKSRL_RxOpeMode         OpeMode_e;          /** Receive Ope Mode */
    t_eFMKSRL_BspReceiveOpe     bspRxOpe_e;         /**< Bsp Receive Ope Mode */
    t_sFMKSRL_BufferInfo        Buffer_s;           /**< Rx Buffer Information */
    t_cbFMKSRL_RcvMsgEvent    * RxUserCb_pcb;       /**< Rx User callback */
    t_bool                      RqstCyclic_b;       /**< Flag to know if a Cyclic Ope is request */
    t_uint16                    infoMode_u16;       /**< Store the TimeOut/Size in case of cyclic Operation */
} t_sFMKSRL_RxMngmt;

/**
 * @brief Serial Line Information
 */
typedef struct __t_sFMKSRL_SerialInfo
{
    t_uFMKSRL_HardwareHandle            bspHandle_u;            /**< UART/USART Handle of the Serial Line */
    t_eFMKCPU_ClockPort                 c_clockPort_e;          /**< Clock Port of the UART/USART */
    t_eFMKCPU_IRQNType                  c_IRQNType_e;           /**< IRQN Type for the UART/USART */
    t_eFMKSRL_HwProtocolType            c_HwType_e;             /**< know if the HandleTypeDef is an UART or USART */
    t_eFMKCPU_DmaRqst                   c_DmaRqstRx;            /**< DMA Rx Channel */
    t_eFMKCPU_DmaRqst                   c_DmaRqstTx;            /**< DMA Tx Channel */
    t_eFMKSRL_HwProtocolType            SoftType_e;             /**< Store the software protocol set by user, cause USART can actually use UART protocol */
    t_eFMKSRL_LineBaudrate              baudrate_e;             /**< Store the baudrate for timeout Operation */
    t_eFMKSRL_LineRunMode               runMode_e;              /**< Store the run mode to use Transmit/Receive Operation */
    t_sFMKSRL_TxMngmt                   TxInfo_s;               /**< Transmit Information */
    t_sFMKSRL_RxMngmt                   RxInfo_s;               /**< Receive Information */
    t_eFMKSRL_LineHealth                Health_e;               /**< Serial Line health Storage*/
    t_uint32                            ErrorCnt_u32;           /**< Number of Error Detected on Line */
    t_uint32                            lastErrorOcc_u32;       /**< Last time an error has been detected  */
    t_bool                              isLineConfigured_b;     /**< Flag to know if Serial is configured */
    t_bool                              flagErrDetected_b;        /**< Flag to know if an error has been report by hardware*/
} t_sFMKSRL_SerialInfo;
/* CAUTION : Automatic generated code section : Start */

/* CAUTION : Automatic generated code section : End */
//-----------------------------TYPEDEF TYPES---------------------------//

// ********************************************************************
// *                      Prototypes
// ********************************************************************

// ********************************************************************
// *                      Variables
// ********************************************************************
static t_eCyclicModState g_FmkSrl_ModState_e = STATE_CYCLIC_CFG;

static t_uint8 g_MProcessIdUsed[FMKSRL_SERIAL_LINE_NB];

static t_eFMKSRL_BspReceiveOpe g_SavedUserRxOpeMode_ae[FMKSRL_SERIAL_LINE_NB];

static t_sFMKSRL_SerialInfo g_SerialInfo_as[FMKSRL_SERIAL_LINE_NB];

/* CAUTION : Automatic generated code section for Variable: Start */
//--------- Tx, Rx Buffer for Serial Line 1 ---------//

/* CAUTION : Automatic generated code section for Variable: End */

//********************************************************************************
//                      Local functions - Prototypes
//****************************************************************************
static t_eReturnCode s_FMKSRL_Operational(void);
static t_eReturnCode s_FMKSRL_PerformDiagnostic(t_eFMKSRL_SerialLine f_srlLine_e);
static t_eReturnCode s_FMKSRL_BspRxOpeMngmt(t_eFMKSRL_BspReceiveOpe f_RxBspOpe,
                                            t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                            t_uint16 f_InfoMode_u16);
static t_eReturnCode s_FMKSRL_BspTxOpeMngmt(t_eFMKSRL_BspTransmitOpe f_TxBspOpe,
                                            t_sFMKSRL_SerialInfo *f_srlInfo_ps);
static t_eReturnCode s_FMKSRL_BspRxOpeTimeOutMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                                   t_eFMKSRL_TimeoutOpe f_Ope_e,
                                                   t_uint16 f_timeOutMs_u16);
static t_eReturnCode s_FMKSRL_BspRxOpeReceiveMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                                   t_uint16 f_rcvDataSize_u16);
static t_eReturnCode s_FMKSRL_BspRxOpeReceiveIdleMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps);
static t_eReturnCode s_FMKSRL_BspTxOpeTransmitMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps);
static t_eReturnCode s_FMKSRL_BspTxOpeTransmitReceiveMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps);
static t_eReturnCode s_FMKSRL_UpdateTxBufferInfo(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                                 t_uint16 *f_dataSizeAdmitted_pu16);
static t_eReturnCode s_FMKSRL_UpdateRxBufferInfo(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                                 t_uint16 f_rcvDataClaim_u16,
                                                 t_uint16 *f_WriteIdx_pu16,
                                                 t_uint16 *f_rcvDataSizeAccept_pu16);
static t_eReturnCode s_FMKSRL_AbortMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                         t_eFMKSRL_BspAbortOpe f_Ope_e);
static t_eReturnCode s_FMKSRL_CheckConfiguration(t_eFMKSRL_HwProtocolType f_hwCfg_e,
                                                 t_eFMKSRL_HwProtocolType f_softCfg_e);
static t_eReturnCode s_FMKSRL_SetBspSerialInit(t_eFMKSRL_SerialLine f_SrlLine_e,
                                               t_sFMKSRL_DrvSerialCfg *f_DrvSrlCfg_ps);
static t_eReturnCode s_FMKSRL_SetUartBspInit(t_eFMKSRL_SerialLine f_SrlLine_e,
                                             t_sFMKSRL_UartCfgSpec *f_UartCfg_ps,
                                             t_sFMKSRL_HwProtocolCfg *f_HwProtCfg_ps);
static t_eReturnCode s_FMKSRL_SetUsartBspInit(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                              t_sFMKSRL_UsartCfgSpec *f_UsartCfg_ps,
                                              t_sFMKSRL_HwProtocolCfg *f_HwProtCfg_ps);
static t_eReturnCode s_FMKSRL_CopyData(t_sFMKSRL_BufferInfo *f_RxTxBuffer_s,
                                       t_uint8 *f_data_pu8,
                                       t_uint16 f_dataSized_u16);
static void s_FMKSRL_BspRxEventCbMngmt(t_uFMKSRL_HardwareHandle *f_Handle_pu,
                                       t_eFMKSRL_BspCbRxEvnt f_Evnt_e,
                                       t_uint16 f_InfoCb_u16);
static void s_FMKSRL_BspTxEventCbMngmt(t_uFMKSRL_HardwareHandle *f_Handle_pu,
                                       t_eFMKSRL_BspCbTxEvnt f_Evnt_e);
static void s_FMKSRL_BspErrorEventCbMngmt(t_uFMKSRL_HardwareHandle *f_Handle_pu,
                                          t_eFMKSRL_BspCbErrEvnt f_Evnt_e);
static t_eReturnCode s_FMKSRL_CallUserMngmt(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                            t_uint16 f_InfoCb_u16);
static t_eReturnCode s_FMKSRL_TimeOutMngmt(t_eFMKSRL_TimeoutOpe f_Ope_e,
                                           t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                           t_uint16 f_timeOutMs_u16);
static t_eReturnCode s_FMKSRL_GetLineErrorFromBsp(t_sFMKSRL_SerialInfo *f_srlInfo_ps,
                                                  t_eFMKSRL_LineHealth *f_health_e);
static t_eReturnCode s_FMKSRL_GetBspLineBaudrate(t_eFMKSRL_LineBaudrate f_lineBaudrate_e,
                                                 t_uint32 *f_bspLineBaudrate_pu32);
static t_eReturnCode s_FMKSRL_GetBspLineStopbit(t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_eFMKSRL_LineSoptbit f_lineStopbit_e,
                                                t_uint32 *f_bspLineStopbit_pu32);
static t_eReturnCode s_FMKSRL_GetBspLineParity(t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                               t_eFMKSRL_LineParity f_lineParity_e,
                                               t_uint32 *f_bspLineParity_pu32);
static t_eReturnCode s_FMKSRL_GetBspLineMode(t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                             t_eFMKSRL_LineMode f_lineMode_e,
                                             t_uint32 *f_bspLineMode_pu32);
static t_eReturnCode s_FMKSRL_GetBspWordLenght(t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                               t_eFMKSRL_LineWordLenght f_lineWordLenght_e,
                                               t_uint32 *f_bspLineWordLenght_pu32);
static t_eReturnCode s_FMMKSRL_GetBspLinBreakLen(t_eFMKSRL_LinBreakLenght f_BreakLenght_e,
                                                 t_uint32 *f_bspBreakLenght_pu32);
static t_eReturnCode s_FMKSRL_GetBspMProcessWakeUpMethod(t_eFMKSRL_MProcessWakeUpMeth f_WakeUpMeth_e,
                                                         t_uint32 *f_bspWakeUpMeth_pu32);
static t_eReturnCode s_FMKSRL_GetUsartBspClkPolarity(t_eFMKSRL_UsartClkPolarity f_ClkPolarity_e,
                                                     t_uint32 *f_bspClkPolarity_pu32);
static t_eReturnCode s_FMKSRL_GetUsartBspLastbit(t_eFMKSRL_UsartLastBit f_LastBit_e,
                                                 t_uint32 *f_bspLastbit_pu32);
static t_eReturnCode s_FMKSRL_GetUsartBspClkPhase(t_eFMKSRL_UsartClockPhase f_ClkPhase_e,
                                                  t_uint32 *f_bspClkPhase_pu32);

//                      Public functions - Implementation
//********************************************************************************
/*********************************
 * FMKSRL_Init
 *********************************/
t_eReturnCode FMKSRL_Init(void)
{
    t_uint8 idxSrlLine_u8;
    t_sFMKSRL_SerialInfo * srlInfo_ps;

    for(idxSrlLine_u8 = (t_uint8)0; idxSrlLine_u8 < FMKSRL_SERIAL_LINE_NB ; idxSrlLine_u8++)
    {
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[idxSrlLine_u8]);
        //------ General Serial Information ------//
        srlInfo_ps->Health_e = FMKSRL_LINE_ERROR_OK;
        srlInfo_ps->ErrorCnt_u32 = (t_uint32)0;
        srlInfo_ps->lastErrorOcc_u32 = (t_uint32)0;
        srlInfo_ps->runMode_e = FMKSRL_LINE_RUNMODE_NB;
        srlInfo_ps->SoftType_e = g_SerialInfo_as[idxSrlLine_u8].c_HwType_e;
        srlInfo_ps->flagErrDetected_b = (t_bool)False;
        srlInfo_ps->c_clockPort_e = c_FmkSrl_SerialCfg_as[idxSrlLine_u8].c_clockPort_e;
        srlInfo_ps->c_IRQNType_e = c_FmkSrl_SerialCfg_as[idxSrlLine_u8].c_IRQNType_e;
        srlInfo_ps->c_DmaRqstRx = c_FmkSrl_SerialCfg_as[idxSrlLine_u8].c_DmaRqstRx;
        srlInfo_ps->c_DmaRqstTx = c_FmkSrl_SerialCfg_as[idxSrlLine_u8].c_DmaRqstTx;

        //------ Receive Information ------//
        srlInfo_ps->isLineConfigured_b = (t_bool)False;
        srlInfo_ps->RxInfo_s.bspRxOpe_e = FMKSRL_BSP_RX_OPE_NB;
        srlInfo_ps->RxInfo_s.OpeMode_e = FMKSRL_OPE_RX_NB;
        srlInfo_ps->RxInfo_s.RqstCyclic_b = (t_bool)False;
        srlInfo_ps->RxInfo_s.RxUserCb_pcb = (t_cbFMKSRL_RcvMsgEvent *)(NULL_FUNCTION);
        srlInfo_ps->RxInfo_s.infoMode_u16 = (t_uint16)0;
        srlInfo_ps->RxInfo_s.Buffer_s.buffferSize_u16 = (t_uint16)0;
        srlInfo_ps->RxInfo_s.Buffer_s.bytesPending_u16 = (t_uint16)0;
        srlInfo_ps->RxInfo_s.Buffer_s.readIdx_u16 = (t_uint16)0;
        srlInfo_ps->RxInfo_s.Buffer_s.writeIdx_u16 = (t_uint16)0;
        srlInfo_ps->RxInfo_s.Buffer_s.bufferAdd_pu8 = 
            (t_uint8 *)c_FmkSrl_SerialCfg_as[idxSrlLine_u8].Rx_StartAddressBuffer_pu8;
        srlInfo_ps->RxInfo_s.Buffer_s.buffferSize_u16 = 
            (t_uint16)c_FmkSrl_SerialCfg_as[idxSrlLine_u8].Rx_bufferSize_u16;
        SETBIT_16B(g_SerialInfo_as[idxSrlLine_u8].RxInfo_s.Buffer_s.status_u16, FMKSRL_BUFFSTATUS_READY);

        //------ Transmit Information ------//
        srlInfo_ps->TxInfo_s.bspTxOpe_e = FMKSRL_BSP_TX_OPE_NB;
        srlInfo_ps->TxInfo_s.NotifyUser_b = (t_bool)False;
        srlInfo_ps->TxInfo_s.OpeMode_e = FMKSRL_TX_NB;
        srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)False;
        srlInfo_ps->TxInfo_s.Buffer_s.buffferSize_u16 = (t_uint16)0;
        srlInfo_ps->TxInfo_s.Buffer_s.bytesPending_u16 = (t_uint16)0;
        srlInfo_ps->TxInfo_s.Buffer_s.readIdx_u16 = (t_uint16)0;
        srlInfo_ps->TxInfo_s.Buffer_s.writeIdx_u16 = (t_uint16)0;
        srlInfo_ps->TxInfo_s.Buffer_s.bufferAdd_pu8 = 
            (t_uint8 *)c_FmkSrl_SerialCfg_as[idxSrlLine_u8].Tx_StartAddressBuffer_pu8;
        srlInfo_ps->TxInfo_s.Buffer_s.buffferSize_u16 = 
            (t_uint16)c_FmkSrl_SerialCfg_as[idxSrlLine_u8].Tx_bufferSize_u16;
        SETBIT_16B(g_SerialInfo_as[idxSrlLine_u8].TxInfo_s.Buffer_s.status_u16, FMKSRL_BUFFSTATUS_READY) ;

        g_SavedUserRxOpeMode_ae[idxSrlLine_u8] = FMKSRL_OPE_RX_NB;
        g_MProcessIdUsed[idxSrlLine_u8] = (t_uint8)0;
    }
    
    return RC_OK;
}
/*********************************
 * FMKSRL_Cyclic
 *********************************/
t_eReturnCode FMKSRL_Cyclic(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch (g_FmkSrl_ModState_e)
    {
        case STATE_CYCLIC_CFG:
        {
            g_FmkSrl_ModState_e = STATE_CYCLIC_PREOPE;
            break;
        }
        case STATE_CYCLIC_PREOPE:
        {
            g_FmkSrl_ModState_e = STATE_CYCLIC_OPE;
            break; 
        }
        case STATE_CYCLIC_OPE:
        {
            Ret_e = s_FMKSRL_Operational();
            if(Ret_e < RC_OK)
            {
                g_FmkSrl_ModState_e = STATE_CYCLIC_ERROR;
            }
            break;
        }
        case STATE_CYCLIC_ERROR:
        {
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
 * FMKSRL_GetState
 *********************************/
t_eReturnCode FMKSRL_GetState(t_eCyclicModState *f_State_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_State_pe == (t_eCyclicModState *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        *f_State_pe = g_FmkSrl_ModState_e;
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_SetState
 *********************************/
t_eReturnCode FMKSRL_SetState(t_eCyclicModState f_State_e)
{

    g_FmkSrl_ModState_e = f_State_e;

    return RC_OK;
}


/*********************************
 * FMKSRL_InitDrv
 *********************************/
t_eReturnCode FMKSRL_InitDrv(   t_eFMKSRL_SerialLine f_SrlLine_e, 
                                    t_sFMKSRL_DrvSerialCfg f_SerialCfg_s,
                                    t_cbFMKSRL_RcvMsgEvent * f_rcvMsgEvnt_pcb,
                                    t_cbFMKSRL_TransmitMsgEvent * f_txMsgEvnt_pcb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    if(f_SrlLine_e >= FMKSRL_SERIAL_LINE_NB)
    {
        ASSERT((t_sint32)0);
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(g_SerialInfo_as[f_SrlLine_e].isLineConfigured_b == (t_bool)True)
    {
        ASSERT((t_sint32)0);
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
    }
    if(Ret_e == RC_OK)
    {
        //------ Point to the Info Structure ------//
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)&g_SerialInfo_as[f_SrlLine_e];

        //------ Check user configuration ------//
        Ret_e = s_FMKSRL_CheckConfiguration(srlInfo_ps->c_HwType_e, f_SerialCfg_s.hwProtType_e);

        //------ Active Hardware Clock ------//
        if(Ret_e == RC_OK)
        {
            Ret_e = FMKCPU_Set_HwClock(srlInfo_ps->c_clockPort_e, FMKCPU_CLOCKPORT_OPE_ENABLE);
        }

        //------ Set NVIC State ------//
        if( (Ret_e == RC_OK) 
        &&  (f_SerialCfg_s.runMode_e != FMKSRL_LINE_RUNMODE_POLL))
        {
            Ret_e = FMKCPU_Set_NVICState(srlInfo_ps->c_IRQNType_e, FMKCPU_NVIC_OPE_ENABLE);
        }

        //------ Set IO Configuration ------//
        if(Ret_e == RC_OK)
        {
            Ret_e = FMKIO_Set_ComSerialCfg((t_eFMKIO_ComSigSerial)f_SrlLine_e);
        }

        //------ Call Serial Init Management ------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_SetBspSerialInit(f_SrlLine_e, &f_SerialCfg_s);
        }

        //------ Copy Information ------//
        if(Ret_e == RC_OK)
        {
            srlInfo_ps->RxInfo_s.RxUserCb_pcb = (t_cbFMKSRL_RcvMsgEvent *)(f_rcvMsgEvnt_pcb);
            srlInfo_ps->TxInfo_s.TxUserCb_pcb = (t_cbFMKSRL_TransmitMsgEvent *)(f_txMsgEvnt_pcb);
            srlInfo_ps->runMode_e = f_SerialCfg_s.runMode_e;
            srlInfo_ps->SoftType_e = f_SerialCfg_s.hwProtType_e;
            srlInfo_ps->baudrate_e = f_SerialCfg_s.hwCfg_s.Baudrate_e;
            srlInfo_ps->isLineConfigured_b = (t_bool)True;
        }
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_Transmit
 *********************************/
t_eReturnCode FMKSRL_Transmit(  t_eFMKSRL_SerialLine f_SrlLine_e, 
                                t_eFMKSRL_TxOpeMode f_OpeMode_e,
                                t_uint8 * f_msgData_pu8,
                                t_uint16 f_dataSize_u16,
                                t_uint16 f_InfoMode_u16,
                                t_bool   f_EnableTxCb_b)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKSRL_SerialInfo * srlInfo_ps;

    if( (f_SrlLine_e >= FMKSRL_SERIAL_LINE_NB)
    ||  (f_OpeMode_e >= FMKSRL_TX_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_msgData_pu8 == (t_uint8 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    //------ Check that Serial Line is in the right state ------//
    if( (Ret_e == RC_OK)
    &&  (g_SerialInfo_as[f_SrlLine_e].isLineConfigured_b == (t_bool)False))
    {
        Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
    }    
    if(Ret_e == RC_OK)
    {

        //------ Process logic transmit Depending on f_OpeMode_e ------//
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[f_SrlLine_e]);

        //------ Copy Data Into Txbuffer ------//
        Ret_e = s_FMKSRL_CopyData(  &srlInfo_ps->TxInfo_s.Buffer_s,
                                    f_msgData_pu8,
                                    f_dataSize_u16);
        if(Ret_e == RC_OK)
        {
            switch (f_OpeMode_e)
            {
                //------ Just Transmit Msg  ------//
                case FMKSRL_TX_ONESHOT:
                {
                    s_FMKSRL_BspTxOpeMngmt( FMKSRL_BSP_TX_OPE_TRANSMIT,
                                            srlInfo_ps);
                    
                    break;
                }
                //------ Configure a Reception Msg with Callback Control base on Sized ------//
                //------ Depending on the softType (UART/USART), we don't call the same function ------//
                case FMKSRL_TX_RX_SIZE:
                {
                    srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)True;
                    
                    if(srlInfo_ps->SoftType_e == FMKSRL_HW_PROTOCOL_UART)
                    {
                        Ret_e = s_FMKSRL_BspRxOpeMngmt( FMKSRL_BSP_RX_OPE_RECEIVE,
                                                        srlInfo_ps,
                                                        f_InfoMode_u16);

                        //------ Transmit msg ------//
                        if(Ret_e == RC_OK)
                        {
                            s_FMKSRL_BspTxOpeMngmt( FMKSRL_BSP_TX_OPE_TRANSMIT,
                                                    srlInfo_ps);
                        }
                    }
                    break;
                }

                //------ Configure a Reception Msg with Callback Control base on Idle ------//
                case FMKSRL_TX_RX_IDLE:
                {
                    srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)True;

                    //------ Configure Reception Line in IDLE Mode ------//
                    Ret_e = s_FMKSRL_BspRxOpeMngmt( FMKSRL_BSP_RX_OPE_RECEIVE_IDLE,
                                                    srlInfo_ps,
                                                    (t_uint32)0);

                    //------ Transmit msg ------//
                    if(Ret_e == RC_OK)
                    {
                        Ret_e = s_FMKSRL_BspTxOpeMngmt( FMKSRL_BSP_TX_OPE_TRANSMIT,
                                                srlInfo_ps);
                    }
                    break;
                }
                //------ Configure a Reception Msg with Callback Control base on Timeout ------//
                case FMKSRL_TX_RX_TIMEOUT:
                {
                    srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)True;
                    //------ Configure Timeout Reception ------//
                    Ret_e = s_FMKSRL_TimeOutMngmt(  FMKSRL_TIMEOUT_OPE_ACTIVATE,
                                                    srlInfo_ps,
                                                    f_InfoMode_u16);

                    //------ Transmit msg ------//
                    if(Ret_e == RC_OK)
                    {
                        Ret_e = s_FMKSRL_BspTxOpeMngmt( FMKSRL_BSP_TX_OPE_TRANSMIT,
                                                srlInfo_ps);
                    }
                    break;
                }

                case FMKSRL_USART_TX_RX_SYNC:
                {
                    if((srlInfo_ps->SoftType_e == FMKSRL_HW_PROTOCOL_USART)
                    && (srlInfo_ps->c_HwType_e == FMKSRL_HW_PROTOCOL_USART))
                    {
                        Ret_e =  s_FMKSRL_BspTxOpeMngmt(    FMKSRL_BSP_TX_OPE_TRANSMIT_RECEIVE,
                                                            srlInfo_ps);
                    } 
                }
                case FMKSRL_TX_NB:
                default:
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
                }
            }
        }
        if(Ret_e == RC_OK)
        {
            //--------- Copy Information ---------//
            srlInfo_ps->TxInfo_s.NotifyUser_b = f_EnableTxCb_b;
             srlInfo_ps->TxInfo_s.OpeMode_e = f_OpeMode_e;
        }
        else 
        {
            srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)False;
        }
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_LogUartSend
 *********************************/
void FMKSRL_LogUartSend(t_eFMKSRL_SerialLine f_SrlLine_e,
                        const t_char * fmt,
                        ...)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint16 lenMsg_u16 = 0U;
    t_uint16 lenPrefix_u16 = 0U;
    t_uint32 currentTime_u32;
    va_list Args_s;

    //---- 1- Check serial line ----//
    if(f_SrlLine_e >= FMKSRL_SERIAL_LINE_NB)
    {
        ASSERT((t_sint32)0);
    }
    else if(g_SerialInfo_as[f_SrlLine_e].isLineConfigured_b == (t_bool)False)
    {
        ASSERT((t_sint32)0);
    }
    else
    {
        //---- 2- Get timestamp ----//
        FMKCPU_GetTick(&currentTime_u32);

        //---- 3- Write log prefix ----//
        Ret_e = SafeMem_snprintf(
            (t_char *)g_UartBufferLog_uac,
            FMKSRL_UART_BUFFER_SIZE,
            &lenPrefix_u16,
            "[%lu] ",
            (unsigned long)currentTime_u32
        );

        if(Ret_e == RC_OK)
        {
            //---- 4- Format log message ----//
            va_start(Args_s, fmt);

            lenMsg_u16 = vsnprintf(g_UartBufferLog_uac + lenPrefix_u16, 
                                (FMKSRL_UART_BUFFER_SIZE - lenPrefix_u16), 
                                fmt, 
                                Args_s);

            va_end(Args_s);
        }

        //---- 5- Send formatted log ----//
        if(Ret_e == RC_OK)
        {
            Ret_e = FMKSRL_Transmit(
                f_SrlLine_e,
                FMKSRL_TX_ONESHOT,
                (t_uint8 *)g_UartBufferLog_uac,
                (t_uint16)(lenPrefix_u16 + lenMsg_u16),
                0U,
                FALSE
            );
        }

        if(Ret_e != RC_OK)
        {
            ASSERT((t_sint32)0);
        }
    }

    return;
}
/*********************************
 * FMKSRL_ConfigureReception
 *********************************/
t_eReturnCode FMKSRL_ConfigureReception(  t_eFMKSRL_SerialLine f_SrlLine_e, 
                                              t_eFMKSRL_RxOpeMode f_OpeMode_e,
                                              t_uint16 f_InfoOpe_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKSRL_BspReceiveOpe bspRxOpe_e;
    t_sFMKSRL_SerialInfo *srlInfo_ps;

    if( (f_SrlLine_e >= FMKSRL_SERIAL_LINE_NB)
    ||  (f_OpeMode_e >=FMKSRL_OPE_RX_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(g_SerialInfo_as[f_SrlLine_e].isLineConfigured_b == (t_bool)False)
    {
        Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
    }

    if(Ret_e == RC_OK)
    {
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[f_SrlLine_e]);

        //--------- Cyclic Ope Mngmt Has to be Update First 
        //             'Cause other function depends on it---------//

        if( (f_OpeMode_e == FMKSRL_OPE_RX_CYCLIC_SIZE)
        ||  (f_OpeMode_e == FMKSRL_OPE_RX_CYCLIC_IDLE)
        ||  (f_OpeMode_e == FMKSRL_OPE_RX_CYCLIC_TIMEOUT))
        {
            srlInfo_ps->RxInfo_s.RqstCyclic_b = (t_bool)True;
        }
        else 
        {
            //--------- Update State ---------//
            srlInfo_ps->RxInfo_s.RqstCyclic_b = (t_bool)False;
        }

        //--------- Configure Reception Logic ---------//
        switch (f_OpeMode_e)
        {

            //--------- Ope Rx Size Managment ---------//
            case FMKSRL_OPE_RX_ONESHOT_SIZE:
            case FMKSRL_OPE_RX_CYCLIC_SIZE:
            {
                bspRxOpe_e = FMKSRL_BSP_RX_OPE_RECEIVE;
                break;    
            }

            //--------- Ope Rx Idle Managment ---------//
            case FMKSRL_OPE_RX_ONESHOT_IDLE:
            case FMKSRL_OPE_RX_CYCLIC_IDLE:
            {
                bspRxOpe_e = FMKSRL_BSP_RX_OPE_RECEIVE_IDLE;
                break; 
            }

            //--------- Ope Rx TimeOut Managment ---------//
            case FMKSRL_OPE_RX_ONESHOT_TIMEOUT:
            case FMKSRL_OPE_RX_CYCLIC_TIMEOUT:
            {
                if(FMKSRL_IsRxTimeoutOpeSupported() == (t_bool)TRUE)
                {
                    bspRxOpe_e = FMKSRL_BSP_RX_OPE_RECEIVE_TIMEOUT;
                }
                else
                {
                    Ret_e = RC_ERROR_NOT_ALLOWED;
                }
                break;
            }

            case FMKSRL_OPE_RX_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_BspRxOpeMngmt( bspRxOpe_e,
                                            srlInfo_ps,
                                            f_InfoOpe_u16);
        }
        if(Ret_e == RC_OK)
        {
            //--------- Update Information ---------//
            g_SavedUserRxOpeMode_ae[f_SrlLine_e] = bspRxOpe_e;
            srlInfo_ps->RxInfo_s.OpeMode_e = f_OpeMode_e;
            srlInfo_ps->RxInfo_s.infoMode_u16 = f_InfoOpe_u16;
        }
    }

    return Ret_e;
}

/*********************************
 * FMKSRL_PRIVATE_GetHandleTypeDef
 *********************************/
void FMKSRL_PRIVATE_GetHandleTypeDef( t_eFMKSRL_SerialLine f_SrlLine_u8,
                                        UART_HandleTypeDef ** f_huartHandle_ps,
                                        USART_HandleTypeDef ** f_UsartHandle_ps)
{
    if(f_SrlLine_u8 >= (t_uint8)FMKSRL_SERIAL_LINE_NB)
    {
        
        ASSERT((t_sint32)f_SrlLine_u8);
        *f_huartHandle_ps = (UART_HandleTypeDef *)NULL;
        *f_UsartHandle_ps = (USART_HandleTypeDef *)NULL;
    }
    else 
    {
        if(g_SerialInfo_as[f_SrlLine_u8].isLineConfigured_b != (t_bool)True)
        {
            ASSERT((t_sint32)f_SrlLine_u8);
        }

        if(g_SerialInfo_as[f_SrlLine_u8].SoftType_e == FMKSRL_HW_PROTOCOL_UART)
        {
            *f_huartHandle_ps = (UART_HandleTypeDef *)(&g_SerialInfo_as[f_SrlLine_u8].bspHandle_u.uartH_s);
        }
        else if ((g_SerialInfo_as[f_SrlLine_u8].SoftType_e == FMKSRL_HW_PROTOCOL_USART))
        {
            *f_UsartHandle_ps = (USART_HandleTypeDef *)(&g_SerialInfo_as[f_SrlLine_u8].bspHandle_u.usartH_s);
        }
        else 
        {
            ASSERT(g_SerialInfo_as[f_SrlLine_u8].SoftType_e);
            //---- give him something ----//
            *f_huartHandle_ps = (UART_HandleTypeDef *)(&g_SerialInfo_as[f_SrlLine_u8].bspHandle_u.uartH_s);
        }
    }

    return;
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
/*********************************
 * s_FMKSRL_Operational
 *********************************/
static t_eReturnCode s_FMKSRL_Operational(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxsrlLine_u8 = (t_uint8)0;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    
    for(idxsrlLine_u8 = (t_uint8)0 ; 
        idxsrlLine_u8 < FMKSRL_SERIAL_LINE_NB 
    &&  g_SerialInfo_as[idxsrlLine_u8].isLineConfigured_b == (t_bool)True ; 
        idxsrlLine_u8++)
    {
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[idxsrlLine_u8]);

        //------ Check if the line is in the right state ------//
    
        //------ Check if an error has been detected on a line ------//
        if(srlInfo_ps->flagErrDetected_b == (t_bool)True)
        {
            Ret_e = s_FMKSRL_PerformDiagnostic(idxsrlLine_u8);
        }

        //------ Check if not too many error occured on line ------//
        if(srlInfo_ps->ErrorCnt_u32 >= (t_uint8)FMKSRL_MAX_ERR_CNT)
        {
            // Call Diag Module with debug Info 1 idxSrlLine_u8, 
            //        debugInfo2 srlInfo_ps->Health_u16  
        }
        
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_PerformDiagnostic
 *********************************/
static t_eReturnCode s_FMKSRL_PerformDiagnostic(t_eFMKSRL_SerialLine  f_srlLine_e)
{
    t_eReturnCode Ret_e;
    t_uint32 currentTime_u32;
    t_eFMKSRL_LineHealth srlLineStatus_e;
    t_sFMKSRL_SerialInfo * serialInfo_ps;

    if(f_srlLine_e >= FMKSRL_SERIAL_LINE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    else
    {
        serialInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[f_srlLine_e]);
        Ret_e = s_FMKSRL_GetLineErrorFromBsp(serialInfo_ps, &srlLineStatus_e);

        if(Ret_e == RC_OK)
        {
            //----- Perform Action Based On the Error Code -----//
            switch (srlLineStatus_e)
            {
                case FMKSRL_LINE_ERROR_PE:
                case FMKSRL_LINE_ERROR_NE:
                case FMKSRL_LINE_ERROR_FE:
                case FMKSRL_LINE_ERROR_ORE:
                case FMKSRL_LINE_ERROR_DMA:
                case FMKSRL_LINE_ERROR_RTO:
                case FMKSRL_LINE_ERROR_UDR:
                case FMKSRL_LINE_ERROR_RX_MSG_ABORT:
                case FMKSRL_LINE_ERROR_TX_MSG_ABORT:
                case FMKSRL_LINE_ERROR_CPLT_MSG_ABORT:
                case FMKSRL_LINE_ERROR_SW_ERR:
                {
                    serialInfo_ps->Health_e = srlLineStatus_e;
                    serialInfo_ps->ErrorCnt_u32 += (t_uint8)1;
                    break;
                }
                case FMKSRL_LINE_ERROR_OK:
                break;
                case FMKSRL_LINE_ERROR_NB:
                {
                    Ret_e = RC_WARNING_NO_OPERATION;
                    break;
                }
            }
            FMKCPU_GetTick(&currentTime_u32);
            //---- see if errros is still active ----//
            if(serialInfo_ps->Health_e != FMKSRL_LINE_ERROR_OK)
            {
                APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_FMK_SRL_OPE_ERROR,
                                        APPSDM_DIAG_ITEM_REPORT_FAIL,
                                        (t_uint16)f_srlLine_e,
                                        (t_uint16)serialInfo_ps->Health_e);
                //---- reset the serial line state ans see if callback still call us with errors ----//
                if((currentTime_u32 - serialInfo_ps->lastErrorOcc_u32) > 100)
                {
                    serialInfo_ps->Health_e = FMKSRL_LINE_ERROR_OK;
                }

            }
            else 
            {
                serialInfo_ps->flagErrDetected_b = (t_bool)FALSE;
                APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_FMK_SRL_OPE_ERROR,
                                        APPSDM_DIAG_ITEM_REPORT_PASS,
                                        (t_uint16)f_srlLine_e,
                                        (t_uint16)0);
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspRxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspRxOpeMngmt(    t_eFMKSRL_BspReceiveOpe f_RxBspOpe, 
                                                t_sFMKSRL_SerialInfo *  f_srlInfo_ps,
                                                t_uint16 f_InfoMode_u16)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_RxBspOpe >= FMKSRL_BSP_RX_OPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {   
        switch (f_RxBspOpe)
        {
            //------ Receive Timeout Management ------//
            case FMKSRL_BSP_RX_OPE_RECEIVE_TIMEOUT:
            {
                Ret_e = s_FMKSRL_BspRxOpeTimeOutMngmt(  f_srlInfo_ps, 
                                                        FMKSRL_TIMEOUT_OPE_ACTIVATE,
                                                        f_InfoMode_u16);

                break;
            }
            //------ Receive Size Management ------//
            case FMKSRL_BSP_RX_OPE_RECEIVE:
            {
                Ret_e = s_FMKSRL_BspRxOpeReceiveMngmt(  f_srlInfo_ps,
                                                        f_InfoMode_u16);

                break;    
            }
            //------ Receive Idle Management ------//
            case FMKSRL_BSP_RX_OPE_RECEIVE_IDLE:
            {
                Ret_e = s_FMKSRL_BspRxOpeReceiveIdleMngmt(f_srlInfo_ps);
                
                break;
                
            }
            case FMKSRL_BSP_RX_OPE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
        //------ Update Information ------//
        if(Ret_e == RC_OK)
        {
            f_srlInfo_ps->RxInfo_s.bspRxOpe_e = f_RxBspOpe;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspTxOpeMngmt(    t_eFMKSRL_BspTransmitOpe f_TxBspOpe, 
                                                t_sFMKSRL_SerialInfo *f_srlInfo_ps)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_TxBspOpe >= FMKSRL_BSP_TX_OPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        //------ Depending on Tx Ope, Logic is a bit different ------//
        switch (f_TxBspOpe)
        {
            case FMKSRL_BSP_TX_OPE_TRANSMIT:
            {
                Ret_e = s_FMKSRL_BspTxOpeTransmitMngmt(f_srlInfo_ps);
                break;
            }
            case FMKSRL_BSP_TX_OPE_TRANSMIT_RECEIVE:
            {
                Ret_e = s_FMKSRL_BspTxOpeTransmitReceiveMngmt(f_srlInfo_ps);
                break;
            }
            case FMKSRL_BSP_TX_OPE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
        if(Ret_e == RC_OK)
        {   
            //------  Update Information------//
            f_srlInfo_ps->TxInfo_s.bspTxOpe_e = f_TxBspOpe;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspRxOpeTimeOutMngmt( t_sFMKSRL_SerialInfo     * f_srlInfo_ps, 
                                                    t_eFMKSRL_TimeoutOpe       f_Ope_e,
                                                    t_uint16                   f_timeOutMs_u16)

{
    t_eReturnCode Ret_e = RC_OK;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_Ope_e >= FMKSRL_TIMEOUT_OPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_srlInfo_ps->SoftType_e != FMKSRL_HW_PROTOCOL_UART)
    {
        Ret_e = RC_ERROR_NOT_ALLOWED;
    }
    if(Ret_e == RC_OK)
    {
         //------ Timeout Management ------//
        Ret_e = s_FMKSRL_TimeOutMngmt(  FMKSRL_TIMEOUT_OPE_ACTIVATE,
                                        f_srlInfo_ps,
                                        FMKSRL_TIMEOUT_RECEPTION);
    }
    if(Ret_e == RC_OK)
    {
        #warning('found the right Size to put and if its the right function to call\n')
        Ret_e = s_FMKSRL_BspRxOpeReceiveMngmt(f_srlInfo_ps, 0xFF);
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspRxOpeReceiveMngmt( t_sFMKSRL_SerialInfo * f_srlInfo_ps,
                                                    t_uint16               f_rcvDataSize_u16)

{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_sFMKSRL_BufferInfo * RxBuffer_s = (t_sFMKSRL_BufferInfo *)NULL;
    t_uint16 RxBuffSizeLeft_u16 = (t_uint16)0;
    t_uint16 writeIdx_u16 = (t_uint16)0;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {

        Ret_e = s_FMKSRL_UpdateRxBufferInfo(f_srlInfo_ps, 
                                            f_rcvDataSize_u16, 
                                            &writeIdx_u16,
                                            &RxBuffSizeLeft_u16);   
    }
    if(Ret_e == RC_OK)
    {
        RxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->RxInfo_s.Buffer_s);

        switch (f_srlInfo_ps->runMode_e)
        {
            case FMKSRL_LINE_RUNMODE_POLL:
            {
                //------ Call Receiving Polling UART/USSART Function ------//
                bspRet_e = c_FmkSrl_RxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                            .bspRxTxPoll_pcb(   &f_srlInfo_ps->bspHandle_u,
                                                (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                RxBuffSizeLeft_u16,
                                                FMKSRL_TIMEOUT_POLLING);
                
                break;
            }
            case FMKSRL_LINE_RUNMODE_IT:
            {
                bspRet_e = c_FmkSrl_RxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                            .bspRxTxIT_pcb(   &f_srlInfo_ps->bspHandle_u,
                                                (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                RxBuffSizeLeft_u16);   
                break;
            }
            case FMKSRL_LINE_RUNMODE_DMA:
            {
                bspRet_e = c_FmkSrl_RxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                            .bspRxTxDMA_pcb(   &f_srlInfo_ps->bspHandle_u,
                                                (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                RxBuffSizeLeft_u16);
                break;
            }
            case FMKSRL_LINE_RUNMODE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
            }
        }
        if(bspRet_e == HAL_BUSY)
        {
            Ret_e = RC_WARNING_BUSY;
        }
        else if (bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
        else
        {
            RxBuffer_s->bytesPending_u16 = (t_uint16)f_rcvDataSize_u16;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspRxOpeReceiveIdleMngmt(t_sFMKSRL_SerialInfo * f_srlInfo_ps)

{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_sFMKSRL_BufferInfo * RxBuffer_s = (t_sFMKSRL_BufferInfo *)NULL;
    t_uint16 RxBuffSizeLeft_u16 = (t_uint16)0;
    t_uint16 rcvDataIdle_u16 = (t_uint16)0;
    t_uint16 writeIdx_u16 = (t_uint16)0;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    //------ Task Validity ------//
    if(f_srlInfo_ps->SoftType_e != FMKSRL_HW_PROTOCOL_UART)
    {
        Ret_e = RC_ERROR_NOT_ALLOWED;
    }
    if(Ret_e == RC_OK)
    {
        RxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->RxInfo_s.Buffer_s);
        //------ Here as we don't know the amount of data received, 
        // we ask the maximum available in buffer ------//
        Ret_e = s_FMKSRL_UpdateRxBufferInfo(f_srlInfo_ps,
                                            (t_uint16)(RxBuffer_s->buffferSize_u16 - RxBuffer_s->bytesPending_u16),
                                            &writeIdx_u16,
                                            &RxBuffSizeLeft_u16);
    }
    if(Ret_e == RC_OK)
    {
        switch (f_srlInfo_ps->runMode_e)
        {
            case FMKSRL_LINE_RUNMODE_POLL:
            {
                bspRet_e =  HAL_UARTEx_ReceiveToIdle(   (UART_HandleTypeDef *)(&f_srlInfo_ps->bspHandle_u),
                                                        (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                        RxBuffSizeLeft_u16,
                                                        &rcvDataIdle_u16,
                                                        FMKSRL_TIMEOUT_POLLING);

                //------ Call User with data ------//
                if((bspRet_e == HAL_OK)
                && (f_srlInfo_ps->RxInfo_s.RxUserCb_pcb != NULL_FUNCTION))
                {
                    f_srlInfo_ps->RxInfo_s.RxUserCb_pcb((t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                        rcvDataIdle_u16,
                                                        FMKSRL_CB_INFO_RECEIVE_ENDING);
                }
                //------ CALL user with error ------//
                else if(f_srlInfo_ps->RxInfo_s.RxUserCb_pcb != NULL_FUNCTION)
                {
                    f_srlInfo_ps->RxInfo_s.RxUserCb_pcb((t_uint8 *)NULL,
                                                        (t_uint16)0,
                                                        FMKSRL_CB_INFO_RECEIVE_ERR);
                }
                break;
            }
            case FMKSRL_LINE_RUNMODE_IT:
            {
                bspRet_e =  HAL_UARTEx_ReceiveToIdle_IT(    (UART_HandleTypeDef *)(&f_srlInfo_ps->bspHandle_u),
                                                            (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                            RxBuffSizeLeft_u16);

                break;
            }
            case FMKSRL_LINE_RUNMODE_DMA:
            {
                bspRet_e =  HAL_UARTEx_ReceiveToIdle_DMA(   (UART_HandleTypeDef *)(&f_srlInfo_ps->bspHandle_u),
                                                            (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                            RxBuffSizeLeft_u16);
                break;
            }
            case FMKSRL_LINE_RUNMODE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
            }
        }
        if(bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
    }
    

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspTxOpeTransmitMngmt(t_sFMKSRL_SerialInfo * f_srlInfo_ps)

{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_sFMKSRL_BufferInfo * TxBuffer_s;
    t_uint16 sizeToTransmit_u16;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        TxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->TxInfo_s.Buffer_s);

        //------ Call Function to Manage Size to Send and Flag Transmission of the line ------//
        Ret_e = s_FMKSRL_UpdateTxBufferInfo( f_srlInfo_ps,
                                            &sizeToTransmit_u16);
    }
    if(Ret_e == RC_OK)
    {
        switch (f_srlInfo_ps->runMode_e)
        {
            //------ We transmit the message in Polling Mode ------//
            case FMKSRL_LINE_RUNMODE_POLL:
            {
                bspRet_e = c_FmkSrl_TxBspFunc_apf[f_srlInfo_ps->SoftType_e].
                                bspRxTxPoll_pcb(    &f_srlInfo_ps->bspHandle_u,
                                                    (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->readIdx_u16]),
                                                    sizeToTransmit_u16,
                                                    FMKSRL_TIMEOUT_POLLING);

                break;                           
            }
            
            //------ We transmit the message in Interrupt Mode ------//
            case FMKSRL_LINE_RUNMODE_IT:
            {
                bspRet_e = c_FmkSrl_TxBspFunc_apf[f_srlInfo_ps->SoftType_e].
                                bspRxTxIT_pcb(  &f_srlInfo_ps->bspHandle_u,
                                                (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->readIdx_u16]),
                                                sizeToTransmit_u16);

                break;   
            }
            case FMKSRL_LINE_RUNMODE_DMA:
            {
                bspRet_e = c_FmkSrl_TxBspFunc_apf[f_srlInfo_ps->SoftType_e].
                                bspRxTxDMA_pcb( &f_srlInfo_ps->bspHandle_u,
                                                (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->readIdx_u16]),
                                                sizeToTransmit_u16);
                break;
            }
            case FMKSRL_LINE_RUNMODE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;

        }
        if(bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspTxOpeMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_BspTxOpeTransmitReceiveMngmt(t_sFMKSRL_SerialInfo     * f_srlInfo_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_sFMKSRL_BufferInfo * TxBuffer_s = (t_sFMKSRL_BufferInfo *)NULL;
    t_sFMKSRL_BufferInfo * RxBuffer_s = (t_sFMKSRL_BufferInfo *)NULL;
    t_uint16 sizeToTransmit_u16 = (t_uint16)0;
    t_uint16 buffSizeLeft_u16 = (t_uint16)0;
    t_uint16 writeIdx_u16 = (t_uint16)0;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_srlInfo_ps->SoftType_e != FMKSRL_HW_PROTOCOL_USART)
    {
        Ret_e = RC_ERROR_NOT_ALLOWED;
    }
    if(Ret_e == RC_OK)
    {
        TxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->TxInfo_s.Buffer_s);
        RxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->RxInfo_s.Buffer_s);

        //------ Call Function to Manage Size to Send and Flag Transmission of the line ------//
        Ret_e = s_FMKSRL_UpdateTxBufferInfo(  f_srlInfo_ps,
                                            &sizeToTransmit_u16);
        if(Ret_e == RC_OK)
        {
            //------ USAR Transmit Receive Protocol impose to receive 
            // exactly what we send, in consequence, we ask sizeToTransmit_u16 bytes in RxBuffers------//
            Ret_e = s_FMKSRL_UpdateRxBufferInfo(f_srlInfo_ps,
                                                sizeToTransmit_u16,
                                                &writeIdx_u16,
                                                &buffSizeLeft_u16);
            
            if(buffSizeLeft_u16 < sizeToTransmit_u16)
            {
                Ret_e = RC_WARNING_LIMIT_REACHED;
                SETBIT_16B(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_OVERFLOW);

            }
        }
    }
    if(Ret_e == RC_OK)
    {
        switch (f_srlInfo_ps->runMode_e)
        {
            //------ Transmit/Receive in Polling Mode ------//
            case FMKSRL_LINE_RUNMODE_POLL:
            {
            
                //------ Call Function to Send Message and Receive Message In polling Mode------//
                bspRet_e = HAL_USART_TransmitReceive(   &f_srlInfo_ps->bspHandle_u.usartH_s,
                                                        (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->writeIdx_u16]),
                                                        (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                        sizeToTransmit_u16,
                                                        FMKSRL_TIMEOUT_POLLING);

                //------ Call User Functon with Data
                // as we cannot know the rcv data size, let the user deals with it------//
                if(f_srlInfo_ps->RxInfo_s.RxUserCb_pcb != NULL_FUNCTION)
                {
                    f_srlInfo_ps->RxInfo_s.RxUserCb_pcb(    (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[RxBuffer_s->readIdx_u16]),
                                                            (t_uint32)sizeToTransmit_u16,
                                                            FMKSRL_CB_INFO_RECEIVE_ENDING);
                }
                break;
            }
            
            //------ Transmit/Receive in Interrupt Mode------//
            case FMKSRL_LINE_RUNMODE_IT:
            {
                bspRet_e = HAL_USART_TransmitReceive_IT(    &f_srlInfo_ps->bspHandle_u.usartH_s,
                                                            (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->writeIdx_u16]),
                                                            (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                            sizeToTransmit_u16);
                break;    
            }

            //------ Transmit/Receive in DMA Mode ------//
            case FMKSRL_LINE_RUNMODE_DMA:
            {
                bspRet_e = HAL_USART_TransmitReceive_DMA(   &f_srlInfo_ps->bspHandle_u.usartH_s,
                                                            (t_uint8 *)(&TxBuffer_s->bufferAdd_pu8[TxBuffer_s->writeIdx_u16]),
                                                            (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[writeIdx_u16]),
                                                            sizeToTransmit_u16);
                break;    
            }
            case FMKSRL_LINE_RUNMODE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;

        }
        if(bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_UpdateTxBufferInfo
 *********************************/
static t_eReturnCode s_FMKSRL_UpdateTxBufferInfo(   t_sFMKSRL_SerialInfo * f_srlInfo_ps,
                                                    t_uint16 * f_dataSizeAdmitted_pu16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKSRL_BufferInfo * TxBuffer_s;
    t_uint16 buffEndSize_u16;
    t_uint16 maxAllowedSize_u16;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        TxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->TxInfo_s.Buffer_s);
        buffEndSize_u16 = (t_uint16)(TxBuffer_s->buffferSize_u16 - TxBuffer_s->readIdx_u16);
        maxAllowedSize_u16 = (TxBuffer_s->bytesPending_u16 > FMKSRL_MAX_BYTES_TO_SEND) 
                                ? FMKSRL_MAX_BYTES_TO_SEND 
                                : TxBuffer_s->bytesPending_u16;
                                
        if(maxAllowedSize_u16 > TxBuffer_s->buffferSize_u16)
        {
            maxAllowedSize_u16 = TxBuffer_s->buffferSize_u16;
        }
        //------ Check if a msg is currently send------//
        if(GETBIT(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY) == BIT_IS_SET_16B)
        {
            //---- set bit msg pending ----//
            SETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_MSG_PENDING);
            Ret_e = RC_WARNING_BUSY;
        }

        //------ Transmit Msg OK------//
        else 
        {
            //------ Update Flag ------//
            RESETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_READY);
            SETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY);
            
             //------ Manage Data To send------//
            switch (f_srlInfo_ps->runMode_e)
            {
                case FMKSRL_LINE_RUNMODE_POLL:
                {
                    *f_dataSizeAdmitted_pu16 = (t_uint16)TxBuffer_s->bytesPending_u16;
                    //------ Reset Sending Byte, 'cause all byte is about to be send------//
                    TxBuffer_s->bytesSending_u16 = (t_uint16)0;
                    break;
                }
                //------ Same Logic is Applied to Interrupt or DMA mode------//
                case FMKSRL_LINE_RUNMODE_DMA:
                case FMKSRL_LINE_RUNMODE_IT:
                {
                    //------ Manage size to send ------//

                    //------  Transmission Fragmented ------//
                    if(maxAllowedSize_u16 > buffEndSize_u16)
                    {
                        
                        SETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_MSG_PENDING);
                        *f_dataSizeAdmitted_pu16 = (t_uint16)buffEndSize_u16;
                        TxBuffer_s->bytesSending_u16 = (t_uint16)buffEndSize_u16;
                    }
                    else 
                    {
                        //------ Reset Flag Msg Pending------//
                        RESETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_MSG_PENDING);
                        
                        
                        *f_dataSizeAdmitted_pu16 = maxAllowedSize_u16;
                        TxBuffer_s->bytesSending_u16 = (t_uint16)maxAllowedSize_u16;
                    }
                    break;

                }
                case FMKSRL_LINE_RUNMODE_NB:
                default:
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
            }
        }
        
        if(Ret_e == RC_OK)
        {
            TxBuffer_s->bytesPending_u16 = (t_uint16)(TxBuffer_s->bytesPending_u16 - 
                                                                TxBuffer_s->bytesSending_u16);
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_UpdateRxBufferInfo
 *********************************/
static t_eReturnCode s_FMKSRL_UpdateRxBufferInfo(t_sFMKSRL_SerialInfo * f_srlInfo_ps,
                                                 t_uint16  f_rcvDataClaim_u16,
                                                 t_uint16 *f_WriteIdx_pu16,
                                                 t_uint16 *f_rcvDataSizeAccept_pu16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKSRL_BufferInfo * RxBuffer_s;
    t_uint16 remainingSpace_u16 = (t_uint16)0;
    t_uint16 firstChunk_u16 = (t_uint16)0;

    if( (f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    ||  (f_rcvDataSizeAccept_pu16 == (t_uint16 *)NULL)
    ||  (f_WriteIdx_pu16 == (t_uint16 *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    if(Ret_e == RC_OK)
    {
        RxBuffer_s = (t_sFMKSRL_BufferInfo *)(&f_srlInfo_ps->RxInfo_s.Buffer_s);

        //------ If buffer in error/overflow state, don't accept Task ------//
        if( (GETBIT(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_OVERFLOW) == BIT_IS_SET_16B)
        ||  (GETBIT(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_ERROR) == BIT_IS_SET_16B))
        {
            Ret_e = RC_WARNING_BUSY;
        }

        else 
        {
            //------ Check Buffer status state ------//
            if(GETBIT(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY) == BIT_IS_SET_16B)
            {
                if(f_srlInfo_ps->TxInfo_s.RqstTxRxOpe_b == (t_bool)True)
                {
                    //------ Abort Operation Currently On Going ------//
                    Ret_e = s_FMKSRL_AbortMngmt(f_srlInfo_ps,
                                                FMKSRL_OPE_ABORT_RECEPTION);

                    //------ Disable TimeOut ------//
                    if(Ret_e == RC_OK)
                    {
                        Ret_e = s_FMKSRL_TimeOutMngmt(  FMKSRL_TIMEOUT_OPE_DISACTIVATE,
                                                        f_srlInfo_ps,
                                                        (t_uint16)0);
                    }

                    //------ Update Information ------//
                    RxBuffer_s->bytesPending_u16 = (t_uint16)0;
                    f_srlInfo_ps->TxInfo_s.RqstTxRxOpe_b = (t_bool)False;
                }
                //------ Task Not Accepted, A Reception is in progress ------//
                else 
                {
                    *f_rcvDataSizeAccept_pu16 = (t_uint16)0;
                    *f_WriteIdx_pu16 = (t_uint16)0;
                    Ret_e = RC_WARNING_BUSY;
                }
            }

            //------ Task Accepted ------//
            if(Ret_e == RC_OK)
            {
                //------ Update flag ------//
                SETBIT_16B(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY);
                RESETBIT_16B(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_READY);
                
                //------ Reset pending bytes as no data has been accepted yet ------//
                RxBuffer_s->bytesPending_u16 = (t_uint16)0;
                //------ Size Management ------//
                if(f_rcvDataClaim_u16 > RxBuffer_s->buffferSize_u16)
                {
                    Ret_e = RC_WARNING_LIMIT_REACHED;
                    *f_rcvDataSizeAccept_pu16 = (t_uint16)0;
                    *f_WriteIdx_pu16 = (t_uint16)0;
                }
                else 
                {
                    //------ Update Info ------//
                    *f_rcvDataSizeAccept_pu16 = f_rcvDataClaim_u16;
                    RxBuffer_s->bytesPending_u16 = (t_uint16)(f_rcvDataClaim_u16);

                    //------ Handle Circular Buffer 
                    //  In IT/POLL Mode We reset Read/Write Idx as no Data are beeing received
                    //  In DMA CIRCULAR mode Calculate the next write idx ------//
                    if ((RxBuffer_s->writeIdx_u16 + f_rcvDataClaim_u16) > RxBuffer_s->buffferSize_u16)
                    {                   
                        if(f_srlInfo_ps->runMode_e != FMKSRL_LINE_RUNMODE_DMA)
                        {
                            *f_WriteIdx_pu16 = (t_uint16)0;
                            RxBuffer_s->readIdx_u16 = (t_uint16)0;
                            RxBuffer_s->writeIdx_u16 = (t_uint16)0;
                        }
                        else 
                        {
                            *f_WriteIdx_pu16 = (t_uint16)RxBuffer_s->writeIdx_u16;
                            remainingSpace_u16 = RxBuffer_s->buffferSize_u16 - RxBuffer_s->writeIdx_u16;
                            //------ Wrap around if we exceed the buffer size ------//
                            firstChunk_u16 = f_rcvDataClaim_u16 - remainingSpace_u16;
                            //------ Update write index after wrap-around ------//
                            RxBuffer_s->writeIdx_u16 = firstChunk_u16;
                        }
                    }
                    else 
                    {
                        //------ Normal update of write index ------//
                        *f_WriteIdx_pu16 = (t_uint16)RxBuffer_s->writeIdx_u16;
                        RxBuffer_s->writeIdx_u16 += f_rcvDataClaim_u16;
                    }

                    //------ Check Write Idx Validity ------//
                    if(RxBuffer_s->writeIdx_u16 >= (t_uint16)RxBuffer_s->buffferSize_u16)
                    {
                        RxBuffer_s->writeIdx_u16 = (t_uint16)0;
                    }
                }
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
 * s_FMKSRL_AbortMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_AbortMngmt(t_sFMKSRL_SerialInfo * f_srlInfo_ps, 
                                         t_eFMKSRL_BspAbortOpe f_Ope_e)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_Ope_e >= FMKSRL_OPE_ABORT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {

        switch (f_srlInfo_ps->runMode_e)
        {
            case FMKSRL_LINE_RUNMODE_POLL:
            {
                if( (c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortPoll_pcb != NULL_FUNCTION)
                && f_Ope_e == FMKSRL_OPE_ABORT_RECEPTION)
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortPoll_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if( (c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortPoll_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_TRANSMISSION))
                {
                    c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortPoll_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if ( (c_FmkSrl_AbortBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortPoll_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_BOTH))
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortPoll_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else 
                {
                    Ret_e = RC_WARNING_NO_OPERATION;
                }
                break;
            }
            case FMKSRL_LINE_RUNMODE_IT:
            {
                if( (c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortIT_pcb != NULL_FUNCTION)
                && f_Ope_e == FMKSRL_OPE_ABORT_RECEPTION)
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortIT_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if( (c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortIT_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_TRANSMISSION))
                {
                    c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortIT_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if ( (c_FmkSrl_AbortBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortIT_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_BOTH))
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortIT_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else 
                {
                    Ret_e = RC_WARNING_NO_OPERATION;
                }
                break;
            }
            case FMKSRL_LINE_RUNMODE_DMA:
            {
                if( (c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortDMA_pcb != NULL_FUNCTION)
                && f_Ope_e == FMKSRL_OPE_ABORT_RECEPTION)
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortDMA_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if( (c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortDMA_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_TRANSMISSION))
                {
                    c_FmkSrl_AbortTxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortDMA_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else if ( (c_FmkSrl_AbortBspFunc_apf[f_srlInfo_ps->SoftType_e].bspAbortDMA_pcb != NULL_FUNCTION)
                &&       (f_Ope_e == FMKSRL_OPE_ABORT_BOTH))
                {
                    c_FmkSrl_AbortRxBspFunc_apf[f_srlInfo_ps->SoftType_e]
                        .bspAbortDMA_pcb(&f_srlInfo_ps->bspHandle_u);
                }
                else 
                {
                    Ret_e = RC_WARNING_NO_OPERATION;
                }
                break;
            }
            case FMKSRL_LINE_RUNMODE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
    }

    return Ret_e;
}
/*********************************
 * s_FMKSRL_CheckConfiguration
 *********************************/
static t_eReturnCode s_FMKSRL_CheckConfiguration(t_eFMKSRL_HwProtocolType f_hwCfg_e, t_eFMKSRL_HwProtocolType f_softCfg_e)
{
    t_eReturnCode Ret_e = RC_OK;

    if( (f_hwCfg_e   >= FMKSRL_HW_PROTOCOL_NB)
     || (f_softCfg_e >= FMKSRL_HW_PROTOCOL_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        if( (f_softCfg_e == FMKSRL_HW_PROTOCOL_USART) 
        &&  (f_hwCfg_e == FMKSRL_HW_PROTOCOL_UART))
        {
            Ret_e = RC_ERROR_NOT_ALLOWED;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_SetBspSerialInit
 *********************************/
static t_eReturnCode s_FMKSRL_SetBspSerialInit(t_eFMKSRL_SerialLine f_SrlLine_e,  t_sFMKSRL_DrvSerialCfg *f_DrvSrlCfg_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    UART_InitTypeDef     * bspUartInit_ps;
    USART_InitTypeDef    * bspUsartInit_ps;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    t_eFMKSRL_HwProtocolType SoftType_e;
    t_uint32 bspLineBaudrate_u32    = (t_uint32)0;
    t_uint32 bspLineParity_u32      = (t_uint32)0;
    t_uint32 bspLineStopbit_u32     = (t_uint32)0;
    t_uint32 bspLineMode_u32        = (t_uint32)0;
    t_uint32 bspLineWordLenght_u32  = (t_uint32)0;

    if(f_SrlLine_e >= FMKSRL_SERIAL_LINE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_DrvSrlCfg_ps == (t_sFMKSRL_DrvSerialCfg *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[f_SrlLine_e]);
        SoftType_e = f_DrvSrlCfg_ps->hwProtType_e;
        //--------- Fistly Configure All Common variable to Uart and Usart ---------//

        //--------- Get Bsp Line Baudrate ---------//
        Ret_e = s_FMKSRL_GetBspLineBaudrate(f_DrvSrlCfg_ps->hwCfg_s.Baudrate_e, &bspLineBaudrate_u32);

        //--------- Get Bsp Line Mode ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetBspLineMode(    SoftType_e,
                                                f_DrvSrlCfg_ps->hwCfg_s.Mode_e,
                                                &bspLineMode_u32);
        }
        
        //--------- Get Bsp Line Parity ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetBspLineParity(  SoftType_e,
                                                f_DrvSrlCfg_ps->hwCfg_s.Parity_e,
                                                &bspLineParity_u32);
        }
        
        //--------- Get Bsp Line Word Lenght ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetBspWordLenght(  SoftType_e,
                                                f_DrvSrlCfg_ps->hwCfg_s.wordLenght_e,
                                                &bspLineWordLenght_u32);
        }
        
        //--------- Get Bsp Line Stop Bit ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetBspLineStopbit( SoftType_e,
                                                f_DrvSrlCfg_ps->hwCfg_s.Stopbit_e,
                                                &bspLineStopbit_u32);
        }
        switch (SoftType_e)
        {
            case FMKSRL_HW_PROTOCOL_UART:
            {
                //-------- Copy data in Uart Init --------//
                bspUartInit_ps = &srlInfo_ps->bspHandle_u.uartH_s.Init;

                bspUartInit_ps->BaudRate   = bspLineBaudrate_u32;
                bspUartInit_ps->Mode       = bspLineMode_u32;
                bspUartInit_ps->Parity     = bspLineParity_u32;
                bspUartInit_ps->StopBits   = bspLineStopbit_u32;
                bspUartInit_ps->WordLength = bspLineWordLenght_u32;
                
                //-------- Set the Instance  --------//
                srlInfo_ps->bspHandle_u.uartH_s.Instance = 
                        (USART_TypeDef *)c_FmkSrl_BspInitIstcMapp_pas[f_SrlLine_e];

                //------ Set DMA Configuration if needed ------//
                if(f_DrvSrlCfg_ps->runMode_e == FMKSRL_LINE_RUNMODE_DMA)
                {
                    //------ Rx Line DMA ------//
                    Ret_e = FMKCPU_RqstDmaInit( srlInfo_ps->c_DmaRqstRx, 
                                                FMKCPU_DMA_TYPE_UART_RX,
                                                (void *)(&srlInfo_ps->bspHandle_u.uartH_s));

                    //------ Tx Line DMA ------//
                    if(Ret_e == RC_OK)
                    {
                        Ret_e = FMKCPU_RqstDmaInit( srlInfo_ps->c_DmaRqstTx,
                                                    FMKCPU_DMA_TYPE_UART_TX, 
                                                    (void *)(&srlInfo_ps->bspHandle_u.uartH_s));
                    }                                                

                }

                //-------- Call Uart Bsp Init Managment --------//
                if(Ret_e == RC_OK)
                {
                    Ret_e = s_FMKSRL_SetUartBspInit(    f_SrlLine_e, 
                                                    (&f_DrvSrlCfg_ps->CfgSpec_u.uartCfg_s),
                                                    (&f_DrvSrlCfg_ps->hwCfg_s));
                }
                
                break;
            }

            case FMKSRL_HW_PROTOCOL_USART:
            {
                //-------- Copy data in Usart Init --------//
                bspUsartInit_ps = &srlInfo_ps->bspHandle_u.usartH_s.Init;

                bspUsartInit_ps->BaudRate   = bspLineBaudrate_u32;
                bspUsartInit_ps->Mode       = bspLineMode_u32;
                bspUsartInit_ps->Parity     = bspLineParity_u32;
                bspUsartInit_ps->StopBits   = bspLineStopbit_u32;
                bspUsartInit_ps->WordLength = bspLineWordLenght_u32;
                
                //-------- Set the Instance  --------//
                srlInfo_ps->bspHandle_u.usartH_s.Instance = 
                        (USART_TypeDef *)c_FmkSrl_BspInitIstcMapp_pas[f_SrlLine_e];

                //------ Set DMA Configuration if needed ------//
                if(f_DrvSrlCfg_ps->runMode_e == FMKSRL_LINE_RUNMODE_DMA)
                {
                    //------ Rx Line DMA ------//
                    Ret_e = FMKCPU_RqstDmaInit( srlInfo_ps->c_DmaRqstRx, 
                                                FMKCPU_DMA_TYPE_USART_RX,
                                                (void *)(&srlInfo_ps->bspHandle_u.usartH_s));

                    //------ Tx Line DMA ------//
                    if(Ret_e == RC_OK)
                    {
                        Ret_e = FMKCPU_RqstDmaInit( srlInfo_ps->c_DmaRqstTx, 
                                                    FMKCPU_DMA_TYPE_USART_TX,
                                                    (void *)(&srlInfo_ps->bspHandle_u.usartH_s));
                    }                                                

                }

                //-------- Call Usart Bsp Init Managment --------//
                if(Ret_e == RC_OK)
                {
                    Ret_e = s_FMKSRL_SetUsartBspInit(   srlInfo_ps,
                                                        (&f_DrvSrlCfg_ps->CfgSpec_u.usartCfg_s),
                                                        (&f_DrvSrlCfg_ps->hwCfg_s));
                }
                
                break;
            }
            case FMKSRL_HW_PROTOCOL_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_SetUartBspInit
 *********************************/
static t_eReturnCode s_FMKSRL_SetUartBspInit(   t_eFMKSRL_SerialLine      f_SrlLine_e, 
                                                t_sFMKSRL_UartCfgSpec   * f_UartCfg_ps,
                                                t_sFMKSRL_HwProtocolCfg * f_HwProtCfg_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    UART_InitTypeDef * bspUartInit_ps;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    t_uint32 bspLINBreakLen_u32 = (t_uint32)0;
    t_uint32 bspWakeUpMethod_u32 = (t_uint32)0;
    t_uint8 idxSrlLine_u8;
    t_uint8 MProcessId_u8;

    if( (f_UartCfg_ps == (t_sFMKSRL_UartCfgSpec *)NULL)
    ||  (f_HwProtCfg_ps == (t_sFMKSRL_HwProtocolCfg *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        srlInfo_ps = (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[f_SrlLine_e]);
        bspUartInit_ps = (UART_InitTypeDef *)(&srlInfo_ps->bspHandle_u.uartH_s.Init);

        Ret_e = FMKSRL_Set_UartSpecificInitCfg( &srlInfo_ps->bspHandle_u.uartH_s,
                                                f_UartCfg_ps->hwFlowCtrl_e,
                                                &f_UartCfg_ps->advProtCfg_s);

        if(Ret_e == RC_OK)
        {
            //--------- As The Clock Freq is about 40-50 MHz, 
            // This configuration support all configuration ---------//
            bspUartInit_ps->OverSampling = UART_OVERSAMPLING_8;
            bspUartInit_ps->ClockPrescaler = UART_PRESCALER_DIV2;
            //--------- Change to Enable if Noise on line is expected ---------//
            bspUartInit_ps->OneBitSampling = DISABLE;  

            //--------- Now Call Specfic Init Function Depending On User Cfg ---------//
            switch (f_UartCfg_ps->Type_e)
            {
                //--------- Configure Uart Instance as Normal Set Up ---------//
                case FMKSRL_UART_TYPECFG_UART:
                {
                    bspRet_e = HAL_UART_Init(&srlInfo_ps->bspHandle_u.uartH_s);
                    break;
                }
                //--------- Configure Uart Instance as Half Duplex Set Up ---------//
                case FMKSRL_UART_TYPECFG_HALF_DUPLEX:
                {
                    bspRet_e = HAL_HalfDuplex_Init(&srlInfo_ps->bspHandle_u.uartH_s);
                    break;
                }
                //--------- Configure Uart Instance as LIN Set Up ---------//
                case FMKSRL_UART_TYPECFG_LIN:
                {
                    //--------- Get Bsp LIN break Detection Lenght ---------//
                    Ret_e = s_FMMKSRL_GetBspLinBreakLen(    f_UartCfg_ps->typeCfg_u.linCfg_s.BreakLen_e,
                                                            &bspLINBreakLen_u32);
                    if(Ret_e == RC_OK)
                    {
                        bspRet_e = HAL_LIN_Init(&srlInfo_ps->bspHandle_u.uartH_s, bspLINBreakLen_u32);
                    }
                    break;
                }
                //--------- Configure Uart Instance as Multi Process Set Up ---------//
                case FMKSRL_UART_TYPECFG_MULTI_PROCESS:
                {
                    //--------- Get Bsp Multi Proccesor Wake Up method ---------//
                    Ret_e = s_FMKSRL_GetBspMProcessWakeUpMethod(    f_UartCfg_ps->typeCfg_u.MProcessCfg_s.WakeUpMethod_e,
                                                                    &bspWakeUpMethod_u32);

                    MProcessId_u8 = f_UartCfg_ps->typeCfg_u.MProcessCfg_s.IstcIdentifer_u8;
                    //--------- Check ID validity ---------//
                    for (idxSrlLine_u8 = (t_uint8)0; idxSrlLine_u8 < FMKSRL_SERIAL_LINE_NB ; idxSrlLine_u8++)
                    {
                        if(MProcessId_u8 == g_MProcessIdUsed[idxSrlLine_u8])
                        {
                            Ret_e = RC_ERROR_ALREADY_CONFIGURED;
                            break;
                        }
                    }

                    if(Ret_e == RC_OK)
                    {
                        bspRet_e = HAL_MultiProcessor_Init( &srlInfo_ps->bspHandle_u.uartH_s,
                                                            MProcessId_u8,
                                                            bspWakeUpMethod_u32);
                
                        //--------- Update ID Used ---------//
                        if(bspRet_e == HAL_OK)
                        {
                            g_MProcessIdUsed[f_SrlLine_e] = MProcessId_u8;
                        }
                    }
                    break;
                }
                case FMKSRL_UART_TYPECFG_NB:
                default:
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
            }
            if(bspRet_e == HAL_OK)
            {
                Ret_e = FMKSRL_Set_UartSpecificPostInitCfg(&srlInfo_ps->bspHandle_u.uartH_s);
            }
            if( (bspRet_e != HAL_OK)
            ||  (Ret_e != RC_OK))
            {
                Ret_e = RC_ERROR_WRONG_RESULT;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_SetUsartBspInit
 *********************************/
static t_eReturnCode s_FMKSRL_SetUsartBspInit(  t_sFMKSRL_SerialInfo *    f_srlInfo_ps, 
                                                t_sFMKSRL_UsartCfgSpec *  f_UsartCfg_ps,
                                                t_sFMKSRL_HwProtocolCfg * f_HwProtCfg_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    USART_InitTypeDef * bspUsartInit_ps;
    t_uint32 bspClkPolarity_u32 = (t_uint32)0;
    t_uint32 bspClkPhase_u32 = (t_uint32)0;
    t_uint32 bspClkLastBit_u32 = (t_uint32)0;

    if( (f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    ||  (f_UsartCfg_ps == (t_sFMKSRL_UsartCfgSpec *)NULL)
    ||  (f_HwProtCfg_ps == (t_sFMKSRL_HwProtocolCfg *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        //--------- Get Bsp Clock Polarity ---------//
        Ret_e = s_FMKSRL_GetUsartBspClkPolarity(f_UsartCfg_ps->clkPolarity_e, &bspClkPolarity_u32);

        //--------- Get Bsp Clock Phase ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetUsartBspClkPhase(f_UsartCfg_ps->clockPhase_e, &bspClkPhase_u32);
        }
        //--------- Get Bsp Clock Last Bit ---------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKSRL_GetUsartBspLastbit(f_UsartCfg_ps->lastBit_e, &bspClkLastBit_u32);
        }

        
        if(Ret_e == RC_OK)
        {
            bspUsartInit_ps = (USART_InitTypeDef *)(&f_srlInfo_ps->bspHandle_u.usartH_s.Init);

            //--------- Copy Data ---------//
            bspUsartInit_ps->CLKLastBit = bspClkLastBit_u32;
            bspUsartInit_ps->CLKPhase = bspClkPhase_u32;
            bspUsartInit_ps->CLKPolarity = bspClkPolarity_u32;
            bspUsartInit_ps->ClockPrescaler = USART_PRESC_PRESCALER_0;

            //--------- Get Bsp Usart Init Function Depending on User Cfg ---------//
            switch (f_UsartCfg_ps->Type_e)
            {
                case FMKSRL_USART_TYPECFG_USART:
                {
                    bspRet_e = HAL_USART_Init((USART_HandleTypeDef *)(&f_srlInfo_ps->bspHandle_u.usartH_s));
                }
                case FMKSRL_USART_TYPECFG_NB:
                default:
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
                }
            }
            if(bspRet_e != HAL_OK)
            {
                Ret_e = RC_ERROR_WRONG_RESULT;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_CopyData
 *********************************/
static t_eReturnCode s_FMKSRL_CopyData( t_sFMKSRL_BufferInfo * f_RxTxBuffer_s,
                                        t_uint8  * f_data_pu8,
                                        t_uint16 f_dataSized_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint16 sizeLeft_u16 = (t_uint16)0;
    t_uint16 spaceToEnd_u16;

    if( (f_RxTxBuffer_s == (t_sFMKSRL_BufferInfo *)NULL)
    ||  (f_data_pu8 == (t_uint8 *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        //--------- Know if we can safely copy Msg into buffer ---------//
        sizeLeft_u16 = (t_uint16)(f_RxTxBuffer_s->buffferSize_u16 - f_RxTxBuffer_s->bytesPending_u16);
        if(sizeLeft_u16 < (t_uint16)f_dataSized_u16)
        {
            ASSERT((t_sint32)sizeLeft_u16);
            Ret_e = RC_WARNING_BUSY;
        }
        if(Ret_e == RC_OK)
        {
            spaceToEnd_u16 = (t_uint16)(f_RxTxBuffer_s->buffferSize_u16 - f_RxTxBuffer_s->writeIdx_u16);
            if(f_dataSized_u16 < (t_uint16)spaceToEnd_u16)
            {
                Ret_e = SafeMem_memcpy( (void *)(&f_RxTxBuffer_s->bufferAdd_pu8[f_RxTxBuffer_s->writeIdx_u16]),
                                        f_data_pu8,
                                        f_dataSized_u16);
                f_RxTxBuffer_s->writeIdx_u16 = (t_uint16)((f_RxTxBuffer_s->writeIdx_u16
                                                            + f_dataSized_u16) % f_RxTxBuffer_s->buffferSize_u16);
            }
            else 
            {
                Ret_e = SafeMem_memcpy( (void *)(&f_RxTxBuffer_s->bufferAdd_pu8[f_RxTxBuffer_s->writeIdx_u16]),
                                        f_data_pu8,
                                        spaceToEnd_u16);
                if(Ret_e == RC_OK)
                {
                    Ret_e = SafeMem_memcpy( (void *)(f_RxTxBuffer_s->bufferAdd_pu8),
                                        (const void *)(&f_data_pu8[spaceToEnd_u16]),
                                        (t_uint16)(f_dataSized_u16 - spaceToEnd_u16));
                }
                if(Ret_e == RC_OK)
                {
                    f_RxTxBuffer_s->writeIdx_u16 = (t_uint16)(f_dataSized_u16 - spaceToEnd_u16);
                }
            }
        }
        if(Ret_e == RC_OK)
        {
            f_RxTxBuffer_s->bytesPending_u16 += f_dataSized_u16;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_BspRxEventCbMngmt
 *********************************/
static void s_FMKSRL_BspRxEventCbMngmt(t_uFMKSRL_HardwareHandle * f_Handle_pu, 
                                       t_eFMKSRL_BspCbRxEvnt f_Evnt_e, 
                                       t_uint16 f_InfoCb_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxSerialLine_u8;
    t_eFMKSRL_SerialLine srlLine_e;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    t_sFMKSRL_BufferInfo * RxBuffer_s;

    if (f_Handle_pu == NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (f_Evnt_e >= FMKSRL_BSP_RX_CB_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        for (idxSerialLine_u8 = 0; idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB; idxSerialLine_u8++)
        {
            if (&g_SerialInfo_as[idxSerialLine_u8].bspHandle_u == f_Handle_pu)
            {
                break;
            }
        }
        if (idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB)
        {
            //--------- Reach Information for tis Serial Line ---------//
            srlLine_e = (t_eFMKSRL_SerialLine)idxSerialLine_u8;
            srlInfo_ps = &g_SerialInfo_as[srlLine_e];
            RxBuffer_s = &srlInfo_ps->RxInfo_s.Buffer_s;

            switch (f_Evnt_e)
            {
                case FMKSRL_BSP_RX_CB_HALCPLT:
                    //--------- NOthing to do, Callback Not Use ---------//
                    break;

                case FMKSRL_BSP_RX_CB_CPLT:
                case FMKSRL_BSP_RX_CB_EVENT:
                {
                    //--------- Call User with data ---------//
                    Ret_e = s_FMKSRL_CallUserMngmt( srlInfo_ps, 
                                                    f_InfoCb_u16);
                    break;
                }
                case FMKSRL_BSP_RX_CB_NB:
                default:
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
            }

            //--------- Update Buffer Status Flag ---------//
            if (f_Evnt_e != FMKSRL_BSP_RX_CB_HALCPLT)
            {
                SETBIT_16B(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_READY);
                RESETBIT_16B(RxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY);
            }

            if (Ret_e == RC_OK)
            {
                //--------- In DMA, Cyclic Ope Perform by Hardware, no need to relaunch cfg ---------//
                if ( (srlInfo_ps->RxInfo_s.RqstCyclic_b == true)
                &&   (srlInfo_ps->runMode_e != FMKSRL_LINE_RUNMODE_DMA))
                {
                    Ret_e = s_FMKSRL_BspRxOpeMngmt(g_SavedUserRxOpeMode_ae[srlLine_e], srlInfo_ps, srlInfo_ps->RxInfo_s.infoMode_u16);
                    if (Ret_e < RC_OK)
                    {
                        if(srlInfo_ps->RxInfo_s.RxUserCb_pcb != NULL_FUNCTION)
                        {
                            // Notify the user of error
                            srlInfo_ps->RxInfo_s.RxUserCb_pcb(NULL, 0, FMKSRL_LINE_ERROR_SW_ERR);
                        }
                    }
                }
            }
        }
    }
}
/*********************************
 * s_FMKSRL_BspTxEventCbMngmt
 *********************************/
static void s_FMKSRL_BspTxEventCbMngmt(   t_uFMKSRL_HardwareHandle * f_Handle_pu,
                                        t_eFMKSRL_BspCbTxEvnt f_Evnt_e)
{   
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxSerialLine_u8;
    t_sFMKSRL_BufferInfo * TxBuffer_s;
    t_sFMKSRL_SerialInfo * srlInfo_ps;
    t_eFMKSRL_SerialLine srlLine_e;

    if(f_Handle_pu == (t_uFMKSRL_HardwareHandle *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_Evnt_e >= FMKSRL_BSP_TX_CB_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        //--------- Found Serial Line ---------//
        for(idxSerialLine_u8 = (t_uint8)0 ;
            idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB ;
            idxSerialLine_u8 ++)
        {
            if(&g_SerialInfo_as[idxSerialLine_u8].bspHandle_u == f_Handle_pu)
            {
                break;
            }
        }
        if(idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB)
        {
            //--------- Reach Information from the line ---------//
            srlLine_e = (t_eFMKSRL_SerialLine)idxSerialLine_u8;
            srlInfo_ps =  (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[srlLine_e]);
            TxBuffer_s = (t_sFMKSRL_BufferInfo *)(&srlInfo_ps->TxInfo_s.Buffer_s);

            //--------- Manage Logic depending on Whom Make the Interruption ---------//
            switch (f_Evnt_e)
            {
                case FMKSRL_BSP_TX_CB_HALCPLT:
                {
                    // NOthing to do, for know, callback not used
                    break;
                }
                case FMKSRL_BSP_TX_CB_CPLT:
                {
                    //--------- Update Tx Buffer Information ---------//
                    RESETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_BUSY);
                    TxBuffer_s->readIdx_u16 = (t_uint16)((TxBuffer_s->readIdx_u16 + 
                                                    TxBuffer_s->bytesSending_u16) % TxBuffer_s->buffferSize_u16);

                    //--------- if datas are pending, send more data from the callback ---------//
                    if(GETBIT(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_MSG_PENDING) == BIT_IS_SET_16B)
                    {
                        Ret_e = s_FMKSRL_BspTxOpeMngmt(srlInfo_ps->TxInfo_s.bspTxOpe_e, srlInfo_ps);
                        
                        //--------- Callback user with error if he wants it ---------//
                        if( (Ret_e != RC_OK)
                        &&  (srlInfo_ps->TxInfo_s.NotifyUser_b == (t_bool)True)
                        &&  (srlInfo_ps->TxInfo_s.TxUserCb_pcb != (t_cbFMKSRL_TransmitMsgEvent *)NULL_FUNCTION))
                        {
                            srlInfo_ps->TxInfo_s.TxUserCb_pcb(False, FMKSRL_CB_INFO_TRANSMIT_ERR);
                        }
                    }
                    else 
                    {
                        //--------- End Transmission, callback user Mngmt ---------//
                        if( (srlInfo_ps->TxInfo_s.NotifyUser_b == (t_bool)True)
                        &&  (srlInfo_ps->TxInfo_s.TxUserCb_pcb != (t_cbFMKSRL_TransmitMsgEvent *)NULL_FUNCTION))
                        {
                            srlInfo_ps->TxInfo_s.TxUserCb_pcb(True, srlInfo_ps->Health_e);
                        }

                        //--------- See if a msg is pending ---------//
                        if(GETBIT(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_MSG_PENDING) == BIT_IS_SET_16B)
                        {
                           Ret_e = s_FMKSRL_BspTxOpeMngmt(srlInfo_ps->TxInfo_s.bspTxOpe_e, srlInfo_ps);

                            //--------- Callback user with error if he wants it ---------//
                            if(  (Ret_e != RC_OK)
                            &&  (srlInfo_ps->TxInfo_s.NotifyUser_b == (t_bool)True)
                            &&  (srlInfo_ps->TxInfo_s.TxUserCb_pcb != (t_cbFMKSRL_TransmitMsgEvent *)NULL_FUNCTION))
                            {
                                srlInfo_ps->TxInfo_s.TxUserCb_pcb(False, FMKSRL_CB_INFO_TRANSMIT_ERR);
                            }
                        }
                        else 
                        {
                            //--------- Update Tx Line Information ---------//
                            TxBuffer_s->bytesSending_u16 = (t_uint16)0;
                            SETBIT_16B(TxBuffer_s->status_u16, FMKSRL_BUFFSTATUS_READY);
                        }
                    }
                    break;
                }
                #warning ('To Implement TxRx Complete')
                case FMKSRL_BSP_TX_RX_CB_CPLT:
                case FMKSRL_BSP_TX_CB_NB:
                default:
                {
                    break;
                }
            }
        }
    }

    return;
}

/*********************************
 * s_FMKSRL_BspErrorEventCbMngmt
 *********************************/
static void s_FMKSRL_BspErrorEventCbMngmt(  t_uFMKSRL_HardwareHandle * f_Handle_pu,
                                            t_eFMKSRL_BspCbErrEvnt f_Evnt_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxSerialLine_u8;
    t_sFMKSRL_SerialInfo * srlInfo_ps;

    if(f_Handle_pu == (t_uFMKSRL_HardwareHandle *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_Evnt_e >= FMKSRL_BSP_ERR_CB_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        //--------- Found Serial Line ---------//
        for(idxSerialLine_u8 = (t_uint8)0 ;
            idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB ;
            idxSerialLine_u8 ++)
        {
            if(&g_SerialInfo_as[idxSerialLine_u8].bspHandle_u == f_Handle_pu)
            {
                break;
            }
        }
        if(idxSerialLine_u8 < FMKSRL_SERIAL_LINE_NB)
        {
            //--------- Update Info ---------//
            srlInfo_ps =  (t_sFMKSRL_SerialInfo *)(&g_SerialInfo_as[idxSerialLine_u8]);
            srlInfo_ps->flagErrDetected_b = (t_bool)True;
            FMKCPU_GetTick(&srlInfo_ps->lastErrorOcc_u32);

            switch(f_Evnt_e)
            {
                case FMKSRL_BSP_ERR_CB_ABORT_TX:
                {
                    srlInfo_ps->Health_e = FMKSRL_LINE_ERROR_TX_MSG_ABORT;
                    //--------- call user with error ---------//
                    if((srlInfo_ps->TxInfo_s.NotifyUser_b == (t_bool)True)
                    && (srlInfo_ps->TxInfo_s.TxUserCb_pcb != (t_cbFMKSRL_TransmitMsgEvent *)NULL_FUNCTION))
                    {
                        srlInfo_ps->TxInfo_s.TxUserCb_pcb(false, FMKSRL_CB_INFO_TRANSMIT_ERR);
                    }
                    break;
                }
                case FMKSRL_BSP_ERR_CB_ABORT_RX:
                {
                    //--------- If TxRx Ope OK abort reception, else not ok ---------//
                    if(srlInfo_ps->TxInfo_s.RqstTxRxOpe_b == (t_bool)False)
                    {
                        srlInfo_ps->Health_e = FMKSRL_LINE_ERROR_RX_MSG_ABORT;
                        srlInfo_ps->flagErrDetected_b = (t_bool)True;
                        FMKCPU_GetTick(&srlInfo_ps->lastErrorOcc_u32);
                        if(srlInfo_ps->RxInfo_s.RxUserCb_pcb != NULL_FUNCTION)
                        {
                            //--------- call user with error ---------//
                            srlInfo_ps->RxInfo_s.RxUserCb_pcb(  (t_uint8 *)NULL, 
                                                                0,
                                                                FMKSRL_LINE_ERROR_SW_ERR);
                        }
                    }
                    break;
                }
                case FMKSRL_BSP_ERR_CB_ABORT_ALL:
                {
                    srlInfo_ps->Health_e = FMKSRL_LINE_ERROR_CPLT_MSG_ABORT;
                    break;
                }
                case FMKSRL_BSP_ERR_CB_ERROR:
                case FMKSRL_BSP_ERR_CB_NB:
                default:
                {
                    break;
                }
            }
        }
    }

    return;
}
/*********************************
 * s_FMKSRL_CallUserMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_CallUserMngmt(t_sFMKSRL_SerialInfo * f_srlInfo_ps, 
                                            t_uint16 f_InfoCb_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint16 dataLength_u16 = (t_uint16)0;
    t_uint16 startIdx_u16 = (t_uint16)0;
    t_uint16 endIdx_u16 = (t_uint16)0;
    t_sFMKSRL_RxMngmt * RxMngmt_ps;
    t_sFMKSRL_BufferInfo * RxBuffer_s;

    if ((f_srlInfo_ps == NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    if (Ret_e == RC_OK)
    {
        RxMngmt_ps = (t_sFMKSRL_RxMngmt *)(&f_srlInfo_ps->RxInfo_s);
        RxBuffer_s = (t_sFMKSRL_BufferInfo *)(&RxMngmt_ps->Buffer_s);

        //--------- Retrieve data received depending on the Rx Mode ---------//
        switch (f_srlInfo_ps->RxInfo_s.bspRxOpe_e)
        {
            case FMKSRL_BSP_RX_OPE_RECEIVE:
            {
                dataLength_u16 = f_srlInfo_ps->RxInfo_s.infoMode_u16;
                // don't reset byte pending 'cause we don't know 
                break;
            }
            case FMKSRL_BSP_RX_OPE_RECEIVE_IDLE:
            {
                dataLength_u16 = f_InfoCb_u16;
                RxBuffer_s->bytesPending_u16 = (t_uint16)0; // may be inaccurate
                
                break;
            }
            case FMKSRL_BSP_RX_OPE_NB:
            default:
            {
                dataLength_u16 = (t_uint16)0;
                break;
            }
        }

        //--------- Get End Index & Start Index ---------//
        startIdx_u16 = RxBuffer_s->readIdx_u16;
        endIdx_u16 = (startIdx_u16 + dataLength_u16) % RxBuffer_s->buffferSize_u16;

        //--------- If data are separate between the end and the begninning
        //          of the buffer call user twice ---------//
        if (endIdx_u16 < startIdx_u16)
        {
            //--------- First Part Data ---------//
            if(RxMngmt_ps->RxUserCb_pcb != NULL_FUNCTION)
            {
                RxMngmt_ps->RxUserCb_pcb(   (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[startIdx_u16]),
                                            (t_uint16)(RxBuffer_s->buffferSize_u16 - startIdx_u16),
                                            FMKSRL_CB_INFO_RECEIVE_PENDING);

                //--------- Second Part Data ---------//
                RxMngmt_ps->RxUserCb_pcb(   (t_uint8 *)(RxBuffer_s->bufferAdd_pu8),
                                            (t_uint16)endIdx_u16,
                                            FMKSRL_CB_INFO_RECEIVE_ENDING);
            } 
        }
        else
        {
            if(RxMngmt_ps->RxUserCb_pcb != NULL_FUNCTION)
            {
                //--------- All Data are aline, call User ---------//
                RxMngmt_ps->RxUserCb_pcb(   (t_uint8 *)(&RxBuffer_s->bufferAdd_pu8[startIdx_u16]),
                                            (t_uint16)dataLength_u16,
                                            FMKSRL_CB_INFO_RECEIVE_ENDING);
            }

        }

        //--------- Update End Idx Buffer & BytesPending---------//
        //--------- if runMode Dma, Dma buffer is exact size expected
        //          which means readIdx = 0 
        if(f_srlInfo_ps->runMode_e == FMKSRL_LINE_RUNMODE_DMA)
        {
            RxBuffer_s->readIdx_u16  = (t_uint16)0;
            RxBuffer_s->writeIdx_u16 = (t_uint16)0;
        }
        else
        {
            RxBuffer_s->readIdx_u16 = RxBuffer_s->writeIdx_u16;
        }
        
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_TimeOutMngmt
 *********************************/
static t_eReturnCode s_FMKSRL_TimeOutMngmt( t_eFMKSRL_TimeoutOpe f_Ope_e,
                                            t_sFMKSRL_SerialInfo * f_srlInfo_ps,
                                            t_uint16 f_timeOutMs_u16)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_uint32 baudrate_u32 = (t_uint32)0;
    t_uint32 bspTimeout_u32 = (t_uint32)0;

    if(f_Ope_e >= FMKSRL_TIMEOUT_OPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL; 
    }

    if(Ret_e == RC_OK)
    {
        switch(f_Ope_e)
        {
            case FMKSRL_TIMEOUT_OPE_ACTIVATE:
            {
                //------ Check that Software Type is UART ------//
                if(f_srlInfo_ps->SoftType_e != FMKSRL_HW_PROTOCOL_UART)
                {
                    Ret_e = RC_ERROR_NOT_ALLOWED;
                }
                if(Ret_e == RC_OK)
                {
                    //------ Compute Timeout Value ------//
                    Ret_e = s_FMKSRL_GetBspLineBaudrate(f_srlInfo_ps->baudrate_e, &baudrate_u32);
                    if(Ret_e == RC_OK)
                    {
                        bspTimeout_u32 = (t_uint32)((t_float32)((t_uint32)f_timeOutMs_u16 / CST_MSEC_TO_SEC) * (t_float32)baudrate_u32);

                        //------Configure and Activate Reception Timeout Trigger ------//
                        HAL_UART_ReceiverTimeout_Config(    &f_srlInfo_ps->bspHandle_u.uartH_s,
                                                            bspTimeout_u32);

                        bspRet_e = HAL_UART_EnableReceiverTimeout(&f_srlInfo_ps->bspHandle_u.uartH_s);
                    }
                }
                break;
            }
            case FMKSRL_TIMEOUT_OPE_DISACTIVATE:
            {
                bspRet_e = HAL_UART_DisableReceiverTimeout(&f_srlInfo_ps->bspHandle_u.uartH_s);
                break;
            }
            case FMKSRL_TIMEOUT_OPE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
    }
    if(bspRet_e != HAL_OK)
    {
        Ret_e = RC_WARNING_BUSY;
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_GetLineErrorFromBsp
 *********************************/
static t_eReturnCode s_FMKSRL_GetLineErrorFromBsp(  t_sFMKSRL_SerialInfo * f_srlInfo_ps,
                                                    t_eFMKSRL_LineHealth * f_health_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 bspErrorCode_u32 = 0;

    if(f_srlInfo_ps == (t_sFMKSRL_SerialInfo *)NULL
    || (f_health_e == (t_eFMKSRL_LineHealth *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        //---- Retreive Error Code Depending on Software Mode ----//
        switch (f_srlInfo_ps->SoftType_e)
        {
            case FMKSRL_HW_PROTOCOL_UART:
            {
                bspErrorCode_u32 = HAL_UART_GetError(&f_srlInfo_ps->bspHandle_u.uartH_s);
                //---- Make Mapping ----//
                switch (bspErrorCode_u32)
                {
                    case HAL_UART_ERROR_NONE:
                        *f_health_e = FMKSRL_LINE_ERROR_OK;
                        break;
                    case HAL_UART_ERROR_PE:
                        *f_health_e = FMKSRL_LINE_ERROR_PE;
                        break;
                    case HAL_UART_ERROR_NE:
                        *f_health_e = FMKSRL_LINE_ERROR_NE;
                        break;
                    case HAL_UART_ERROR_FE:
                        *f_health_e = FMKSRL_LINE_ERROR_FE;
                        break;
                    case HAL_UART_ERROR_ORE:
                        *f_health_e = FMKSRL_LINE_ERROR_ORE;
                        break;
                    case HAL_UART_ERROR_DMA:
                        *f_health_e = FMKSRL_LINE_ERROR_DMA;
                        break;
                    case HAL_UART_ERROR_RTO:
                        *f_health_e = FMKSRL_LINE_ERROR_RTO;
                        break;
                    default:
                        *f_health_e = FMKSRL_LINE_ERROR_OK;
                        break;
                }
                break;
            }
            case FMKSRL_HW_PROTOCOL_USART:
            {
                bspErrorCode_u32 = HAL_USART_GetError(&f_srlInfo_ps->bspHandle_u.usartH_s);
                //---- Make Mapping ----//
                switch (bspErrorCode_u32)
                {
                    case HAL_USART_ERROR_NONE:
                        *f_health_e = FMKSRL_LINE_ERROR_OK;
                        break;
                    case HAL_USART_ERROR_PE:
                        *f_health_e = FMKSRL_LINE_ERROR_PE;
                        break;
                    case HAL_USART_ERROR_NE:
                        *f_health_e = FMKSRL_LINE_ERROR_NE;
                        break;
                    case HAL_USART_ERROR_FE:
                        *f_health_e = FMKSRL_LINE_ERROR_FE;
                        break;
                    case HAL_USART_ERROR_ORE:
                        *f_health_e = FMKSRL_LINE_ERROR_ORE;
                        break;
                    case HAL_USART_ERROR_DMA:
                        *f_health_e = FMKSRL_LINE_ERROR_DMA;
                        break;
                    case HAL_USART_ERROR_UDR:
                        *f_health_e = FMKSRL_LINE_ERROR_UDR;
                        break;
                    case HAL_USART_ERROR_RTO:
                        *f_health_e = FMKSRL_LINE_ERROR_RTO;
                        break;
                    default:
                        *f_health_e = FMKSRL_LINE_ERROR_OK;
                        break;
                }
            }
            case FMKSRL_HW_PROTOCOL_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_GetBspLineBaudrate
 *********************************/
static t_eReturnCode s_FMKSRL_GetBspLineBaudrate(t_eFMKSRL_LineBaudrate f_lineBaudrate_e, t_uint32 *f_bspLineBaudrate_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_lineBaudrate_e >= FMKSRL_LINE_BAUDRATE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspLineBaudrate_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_lineBaudrate_e)
        {
            case FMKSRL_LINE_BAUDRATE_300:
                *f_bspLineBaudrate_pu32 = (t_uint32)300;
                break;
            case FMKSRL_LINE_BAUDRATE_1200:
                *f_bspLineBaudrate_pu32 = (t_uint32)1200;
                break;
            case FMKSRL_LINE_BAUDRATE_2400:
                *f_bspLineBaudrate_pu32 = (t_uint32)2400;
                break;
            case FMKSRL_LINE_BAUDRATE_4800:
                *f_bspLineBaudrate_pu32 = (t_uint32)4800;
                break;
            case FMKSRL_LINE_BAUDRATE_9600:
                *f_bspLineBaudrate_pu32 = (t_uint32)9600;
                break;
            case FMKSRL_LINE_BAUDRATE_19200:
                *f_bspLineBaudrate_pu32 = (t_uint32)19200;
                break;
            case FMKSRL_LINE_BAUDRATE_38400:
                *f_bspLineBaudrate_pu32 = (t_uint32)38400;
                break;  
            case FMKSRL_LINE_BAUDRATE_57600:
                *f_bspLineBaudrate_pu32 = (t_uint32)57600;
                break;
            case FMKSRL_LINE_BAUDRATE_74880:
                *f_bspLineBaudrate_pu32 = (t_uint32)74880;
                break;
            case FMKSRL_LINE_BAUDRATE_115200:
                *f_bspLineBaudrate_pu32 = (t_uint32)115200;
                break;
            case FMKSRL_LINE_BAUDRATE_230400:
                *f_bspLineBaudrate_pu32 = (t_uint32)230400;
                break;
            case FMKSRL_LINE_BAUDRATE_250000:
                *f_bspLineBaudrate_pu32 = (t_uint32)250000;
                break;
            case FMKSRL_LINE_BAUDRATE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_GetBspLineStopbit
 *********************************/
static t_eReturnCode s_FMKSRL_GetBspLineStopbit(    t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                    t_eFMKSRL_LineSoptbit f_lineStopbit_e, 
                                                    t_uint32 *f_bspLineStopbit_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    // Validation des paramètres d'entrée
    if ((f_HwProtUsed_e >= FMKSRL_HW_PROTOCOL_NB) 
        || (f_lineStopbit_e >= FMKSRL_LINE_STOPBIT_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspLineStopbit_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    if (Ret_e == RC_OK)
    {
        //--------- Switch on Software Stop Bit Used ---------//
        switch (f_lineStopbit_e)
        {
            case FMKSRL_LINE_STOPBIT_0_5:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)UART_STOPBITS_0_5;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)USART_STOPBITS_0_5;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_STOPBIT_1:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)UART_STOPBITS_1;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)USART_STOPBITS_1;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_STOPBIT_1_5:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)UART_STOPBITS_1_5;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)USART_STOPBITS_1_5;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_STOPBIT_2:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)UART_STOPBITS_2;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineStopbit_pu32 = (t_uint32)USART_STOPBITS_2;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_STOPBIT_NB:
            default:
                Ret_e = RC_ERROR_PARAM_INVALID;
                break;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_GetBspLineParity
 *********************************/
static t_eReturnCode s_FMKSRL_GetBspLineParity( t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_eFMKSRL_LineParity f_lineParity_e,
                                                t_uint32 *f_bspLineParity_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    // Validation des paramètres d'entrée
    if ((f_HwProtUsed_e >= FMKSRL_HW_PROTOCOL_NB) 
        || (f_lineParity_e >= FMKSRL_LINE_PARITY_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspLineParity_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    if (Ret_e == RC_OK)
    {
        //--------- Switch on Software Stop Bit Used ---------//
        switch (f_lineParity_e)
        {
            case FMKSRL_LINE_PARITY_NONE:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)UART_PARITY_NONE;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)USART_PARITY_NONE;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_PARITY_EVEN:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)UART_PARITY_EVEN;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)USART_PARITY_EVEN;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_PARITY_ODD:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)UART_PARITY_EVEN;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineParity_pu32 = (t_uint32)USART_PARITY_EVEN;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_PARITY_NB:
            default:
                Ret_e = RC_ERROR_PARAM_INVALID;
                break;
        }
    }

    return Ret_e;
} 

/*********************************
 * s_FMKSRL_GetBspLineParity
 *********************************/
static t_eReturnCode s_FMKSRL_GetBspLineMode(   t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_eFMKSRL_LineMode f_lineMode_e,
                                                t_uint32 *f_bspLineMode_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    // Validation des paramètres d'entrée
    if ((f_HwProtUsed_e >= FMKSRL_HW_PROTOCOL_NB) 
        || (f_lineMode_e >= FMKSRL_LINE_MODE_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_bspLineMode_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }

    if (Ret_e == RC_OK)
    {
        //--------- Switch on Software Stop Bit Used ---------//
        switch (f_lineMode_e)
        {
            case FMKSRL_LINE_MODE_RX:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)UART_MODE_RX;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)USART_MODE_RX;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_MODE_TX:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)UART_MODE_TX;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)USART_MODE_TX;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_MODE_RX_TX:
                //--------- Depend on Hardware Protocol Used ---------//
                if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_UART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)UART_MODE_TX_RX;
                }
                else if (f_HwProtUsed_e == FMKSRL_HW_PROTOCOL_USART)
                {
                    *f_bspLineMode_pu32 = (t_uint32)USART_MODE_TX_RX;
                }
                else 
                {
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                }
                break;

            case FMKSRL_LINE_MODE_NB:
            default:
                Ret_e = RC_ERROR_PARAM_INVALID;
                break;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKSRL_GetBspLineParity
 *********************************/
static t_eReturnCode s_FMKSRL_GetBspWordLenght( t_eFMKSRL_HwProtocolType f_HwProtUsed_e,
                                                t_eFMKSRL_LineWordLenght f_lineWordLenght_e, 
                                                t_uint32 *f_bspLineWordLenght_pu32)
{
    return FMKSRL_Get_BspWordLength(   f_HwProtUsed_e,
                                        f_lineWordLenght_e,
                                        f_bspLineWordLenght_pu32);
} 
/*********************************
 * s_FMMKSRL_GetBspLinBreakLen
 *********************************/
static t_eReturnCode s_FMMKSRL_GetBspLinBreakLen(t_eFMKSRL_LinBreakLenght f_BreakLenght_e, t_uint32 * f_bspBreakLenght_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_BreakLenght_e >= FMKSRL_LIN_BREAK_LENGHT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspBreakLenght_pu32 == (t_uint32 * )NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_BreakLenght_e)
        {
            case FMKSRL_LIN_BREAK_LENGHT_10B:
                *f_bspBreakLenght_pu32 = (t_uint32)UART_LINBREAKDETECTLENGTH_10B;
                break;

            case FMKSRL_LIN_BREAK_LENGHT_11B:
                *f_bspBreakLenght_pu32 = (t_uint32)UART_LINBREAKDETECTLENGTH_11B;
                break;

            case FMKSRL_LIN_BREAK_LENGHT_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}

/***************************************
 * s_FMKSRL_GetBspMProcessWakeUpMethod
 ***************************************/
static t_eReturnCode s_FMKSRL_GetBspMProcessWakeUpMethod(t_eFMKSRL_MProcessWakeUpMeth f_WakeUpMeth_e, t_uint32 * f_bspWakeUpMeth_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_WakeUpMeth_e >= FMKSRL_MPROCESS_WAKEUP_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspWakeUpMeth_pu32 == (t_uint32 * )NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_WakeUpMeth_e)
        {
            case FMKSRL_MPROCESS_WAKEUP_IDLE:
                *f_bspWakeUpMeth_pu32 = (t_uint32)UART_WAKEUPMETHOD_IDLELINE;
                break;

            case FMKSRL_MPROCESS_WAKEUP_ADDMARK:
                *f_bspWakeUpMeth_pu32 = (t_uint32)UART_WAKEUPMETHOD_ADDRESSMARK;
                break;

            case FMKSRL_MPROCESS_WAKEUP_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}

/***************************************
 * s_FMKSRL_GetUsartBspClkPolarity
 ***************************************/
static t_eReturnCode s_FMKSRL_GetUsartBspClkPolarity(t_eFMKSRL_UsartClkPolarity f_ClkPolarity_e, t_uint32 * f_bspClkPolarity_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_ClkPolarity_e >= FMKSRL_USART_CLK_POLARITY_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspClkPolarity_pu32 == (t_uint32 * )NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_ClkPolarity_e)
        {
            case FMKSRL_USART_CLK_POLARITY_HIGH:
                *f_bspClkPolarity_pu32 = (t_uint32)USART_POLARITY_HIGH;
                break;

            case FMKSRL_USART_CLK_POLARITY_LOW:
                *f_bspClkPolarity_pu32 = (t_uint32)USART_POLARITY_LOW;
                break;

            case FMKSRL_USART_CLK_POLARITY_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}

/***************************************
 * s_FMKSRL_GetUsartBspLastbit
 ***************************************/
static t_eReturnCode s_FMKSRL_GetUsartBspLastbit(t_eFMKSRL_UsartLastBit f_LastBit_e, t_uint32 * f_bspLastbit_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_LastBit_e >= FMKSRL_USART_LAST_BIT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspLastbit_pu32 == (t_uint32 * )NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_LastBit_e)
        {
            case FMKSRL_USART_LAST_BIT_ENABLE:
                *f_bspLastbit_pu32 = (t_uint32)USART_LASTBIT_ENABLE;
                break;

            case FMKSRL_USART_LAST_BIT_DISABLE:
                *f_bspLastbit_pu32 = (t_uint32)USART_LASTBIT_DISABLE;
                break;

            case FMKSRL_USART_LAST_BIT_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}

/***************************************
 * s_FMKSRL_GetUsartBspClkPhase
 ***************************************/
static t_eReturnCode s_FMKSRL_GetUsartBspClkPhase(t_eFMKSRL_UsartClockPhase f_ClkPhase_e, t_uint32 * f_bspClkPhase_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_ClkPhase_e >= FMKSRL_USART_PHASE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_bspClkPhase_pu32 == (t_uint32 * )NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch (f_ClkPhase_e)
        {
            case FMKSRL_USART_PHASE_1EDGE:
                *f_bspClkPhase_pu32 = (t_uint32)USART_PHASE_1EDGE;
                break;

            case FMKSRL_USART_PHASE_2EDGE:
                *f_bspClkPhase_pu32 = (t_uint32)USART_PHASE_2EDGE;
                break;

            case FMKSRL_USART_PHASE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    return Ret_e;
}
//************************************************************************************
//                                      BSP CALLBACK MAPPING
//************************************************************************************

/***********************************************************
 * UART CALLBACK MANAGEMENT
 **********************************************************/
// UNUSED void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
/***************************************
 * HAL_UART_TxCpltCallback
 ***************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    return s_FMKSRL_BspTxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)huart, 
                                        FMKSRL_BSP_TX_CB_CPLT); 
}

/***************************************
 * HAL_UART_RxHalfCpltCallback
 ***************************************/
// UNUSED void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)

/***************************************
 * HAL_UART_RxCpltCallback
 ***************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    return s_FMKSRL_BspRxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)huart, 
                                        FMKSRL_BSP_RX_CB_CPLT,
                                        (t_uint16)0); 
}

/***************************************
 * HAL_UARTEx_RxEventCallback
 ***************************************/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    return s_FMKSRL_BspRxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)huart, 
                                        FMKSRL_BSP_RX_CB_CPLT,
                                        (t_uint16)Size); 
}

/***************************************
 * HAL_USART_TxHalfCpltCallback
 ***************************************/
// UNUSED void HAL_USART_TxHalfCpltCallback(USART_HandleTypeDef *husart)

/***************************************
 * HAL_USART_TxCpltCallback
 ***************************************/
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
    return s_FMKSRL_BspTxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)husart, 
                                        FMKSRL_BSP_TX_CB_CPLT); 
}
/***************************************
 * HAL_USART_RxCpltCallback
 ***************************************/
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
    return s_FMKSRL_BspRxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)husart, 
                                        FMKSRL_BSP_RX_CB_CPLT,
                                        (t_uint16)0); 
}
/***************************************
 * HAL_USART_RxHalfCpltCallback
 ***************************************/
// UNUSED void HAL_USART_RxHalfCpltCallback(USART_HandleTypeDef *husart)

/***************************************
 * HAL_USART_TxRxCpltCallback
 ***************************************/
void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart)
{
    return s_FMKSRL_BspTxEventCbMngmt(  (t_uFMKSRL_HardwareHandle *)husart, 
                                        FMKSRL_BSP_TX_RX_CB_CPLT); 
}

/***************************************
 * Error Callback
 ***************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)              { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)huart,  FMKSRL_BSP_ERR_CB_ERROR);}
void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)          { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)huart,  FMKSRL_BSP_ERR_CB_ABORT_ALL);}
void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)  { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)huart,  FMKSRL_BSP_ERR_CB_ABORT_RX);}
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)   { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)huart, FMKSRL_BSP_ERR_CB_ABORT_TX);}
void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart)           { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)husart,  FMKSRL_BSP_ERR_CB_ERROR);}
void HAL_USART_AbortCpltCallback(USART_HandleTypeDef *husart)       { return s_FMKSRL_BspErrorEventCbMngmt((t_uFMKSRL_HardwareHandle *)husart,  FMKSRL_BSP_ERR_CB_ABORT_ALL);}
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
