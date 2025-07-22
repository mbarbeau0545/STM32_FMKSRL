/*********************************************************************
 * @file        FMKIO_ConfigSpecific.h
 * @brief       Template_BriefDescription.
 * @details     TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKIO_CONFIGSPECIFIC_H_INCLUDED
#define FMKIO_CONFIGSPECIFIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "../FMKCFG_ConfigFiles/FMKIO_ConfigPublic.h"
    #include "../FMKCFG_ConfigFiles/FMKTIM_ConfigPublic.h"
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
    //                      Public functions - Prototyupes
    //********************************************************************************
    /**
    *
    *	@brief      Get the Bsp Gpio Port Instance 
    *
    *
    *	@param[in]  f_GpioPort_e       : GPIOPort enum from @ref t_eFMKIO_GpioPort
    *	@param[in]  f_BspGpio_ps       : pointor to pointor to change the direction a the pointer
    *
    * @retval RC_OK                             @ref RC_OK
    * @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    * @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
    *
    */
    t_eReturnCode FMKIO_Get_BspGpioPort(t_eFMKIO_GpioPort f_GpioPort_e, GPIO_TypeDef **f_BspGpio_ps);

    /**
    *
    *	@brief      Get the Rcc Clock Port For a GPIO Clock Port
    *
    *
    *	@param[in]  f_GpioPort_e       : GPIOPort enum from @ref t_eFMKIO_GpioPort
    *	@param[in]  f_BspGpio_ps       : pointor to pointor to change the direction a the pointer
    *
    * @retval RC_OK                             @ref RC_OK
    * @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    * @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    * @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
    *
    */
    t_eReturnCode FMKIO_GetGpioRccClock(t_eFMKIO_GpioPort f_gpioPort_e, t_eFMKCPU_ClockPort * f_clockPort_pe);
#endif // FMKIO_CONFIGSPECIFIC_H_INCLUDED           
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
