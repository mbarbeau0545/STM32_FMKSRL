/*********************************************************************
 * @file        FMKIO_ConfigSpecific.h
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
#include "./FMKIO_ConfigSpecific.h"
#include "FMK_HAL/FMK_IO/Src/FMK_IO.h"
// ********************************************************************
// *                      Defines
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************

//-----------------------------ENUM TYPES-----------------------------//

//-----------------------------STRUCT TYPES---------------------------//

//-----------------------------TYPEDEF TYPES---------------------------//
// ********************************************************************
// *                      Prototypes
// ********************************************************************
	
// ********************************************************************
// *                      Variables
// ********************************************************************

//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************

//****************************************************************************
//                      Public functions - Implementation
//********************************************************************************
/*********************************
 * FMKIO_Get_BspGpioPort
 *********************************/
t_eReturnCode FMKIO_Get_BspGpioPort(t_eFMKIO_GpioPort f_GpioPort_e, GPIO_TypeDef **f_BspGpio_ps)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_GpioPort_e >= FMKIO_GPIO_PORT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_BspGpio_ps == (GPIO_TypeDef **)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_GpioPort_e)
        {
            /* CAUTION : Automatic generated code section for GPIO switch case: Start */
            case FMKIO_GPIO_PORT_A:
                *f_BspGpio_ps = GPIOA;
                break;
            case FMKIO_GPIO_PORT_B:
                *f_BspGpio_ps = GPIOB;
                break;
            case FMKIO_GPIO_PORT_C:
                *f_BspGpio_ps = GPIOC;
                break;
            case FMKIO_GPIO_PORT_D:
                *f_BspGpio_ps = GPIOD;
                break;
            case FMKIO_GPIO_PORT_E:
                *f_BspGpio_ps = GPIOE;
                break;
            case FMKIO_GPIO_PORT_F:
                *f_BspGpio_ps = GPIOF;
                break;
            case FMKIO_GPIO_PORT_G:
                *f_BspGpio_ps = GPIOG;
                break;
            /* CAUTION : Automatic generated code section for GPIO switch case: End */
            case FMKIO_GPIO_PORT_NB:
            default:
                Ret_e = RC_WARNING_NO_OPERATION;
                break;
        }
    }
    return Ret_e;
}

/*********************************
 * FMKIO_GetGpioRccClock
 *********************************/
t_eReturnCode FMKIO_GetGpioRccClock(t_eFMKIO_GpioPort f_gpioPort_e, t_eFMKCPU_ClockPort * f_clockPort_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_gpioPort_e >= FMKIO_GPIO_PORT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_clockPort_pe == (t_eFMKCPU_ClockPort *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_gpioPort_e)
        {
        /* CAUTION : Automatic generated code section for Switch Case GPIO to RCC: Start */
            case FMKIO_GPIO_PORT_A:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOA;
                break;
            case FMKIO_GPIO_PORT_B:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOB;
                break;
            case FMKIO_GPIO_PORT_C:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOC;
                break;
            case FMKIO_GPIO_PORT_D:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOD;
                break;
            case FMKIO_GPIO_PORT_E:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOE;
                break;
            case FMKIO_GPIO_PORT_F:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOF;
                break;
            case FMKIO_GPIO_PORT_G:
                *f_clockPort_pe = FMKCPU_RCC_CLK_GPIOG;
                break;
        /* CAUTION : Automatic generated code section for Switch Case GPIO to RCC: End */

            case FMKIO_GPIO_PORT_NB:
            default:
                Ret_e = RC_WARNING_NO_OPERATION;
                break;
        }
    }
    return Ret_e;
}

/* CAUTION : Automatic generated code section for Exti IRQN_Handler: Start */
/*********************************
 * EXTI0_IRQHandler
*********************************/
void EXTI0_IRQHandler(void)               {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI1_IRQHandler
*********************************/
void EXTI1_IRQHandler(void)               {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI2_IRQHandler
*********************************/
void EXTI2_IRQHandler(void)               {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI3_IRQHandler
*********************************/
void EXTI3_IRQHandler(void)               {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI4_IRQHandler
*********************************/
void EXTI4_IRQHandler(void)               {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI9_5_IRQHandler
*********************************/
void EXTI9_5_IRQHandler(void)             {return FMKIO_BspRqst_InterruptMngmt();}

/*********************************
 * EXTI15_10_IRQHandler
*********************************/
void EXTI15_10_IRQHandler(void)           {return FMKIO_BspRqst_InterruptMngmt();}

/* CAUTION : Automatic generated code section for Exti IRQN_Handler: End */
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
 *	@param[in] 
 *	@param[out]
 *	 
 *
 *
 */
