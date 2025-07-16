/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_VREF_init();
    SYSCFG_DL_SYSTICK_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_VREF_reset(VREF);


    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_VREF_enablePower(VREF);

    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXOUT_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXOUT_IOMUX);

    DL_GPIO_initDigitalOutputFeatures(LED_A14_PIN_14_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_initDigitalInputFeatures(KEY_BSL_PIN_18_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, LED_A14_PIN_14_PIN);
    DL_GPIO_enableOutput(GPIOA, LED_A14_PIN_14_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOA, KEY_BSL_PIN_18_PIN);
    DL_GPIO_enableInterrupt(GPIOA, KEY_BSL_PIN_18_PIN);

}


static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq              = DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ,
	.rDivClk2x              = 1,
	.rDivClk1               = 0,
	.rDivClk0               = 0,
	.enableCLK2x            = DL_SYSCTL_SYSPLL_CLK2X_DISABLE,
	.enableCLK1             = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
	.enableCLK0             = DL_SYSCTL_SYSPLL_CLK0_ENABLE,
	.sysPLLMCLK             = DL_SYSCTL_SYSPLL_MCLK_CLK0,
	.sysPLLRef              = DL_SYSCTL_SYSPLL_REF_HFCLK,
	.qDiv                   = 3,
	.pDiv                   = DL_SYSCTL_SYSPLL_PDIV_1
};
static const DL_SYSCTL_LFCLKConfig gLFCLKConfig = {
    .lowCap   = false,
    .monitor  = false,
    .xt1Drive = DL_SYSCTL_LFXT_DRIVE_STRENGTH_HIGHEST,
};


///////////////////////////////////////////////////////////////


void DL_SYSCTL_setHFCLKSourceHFXTParams (DL_SYSCTL_HFXT_RANGE range, uint32_t startupTime, bool monitorEnable)
{
    uint32_t register_temp = SYSCTL->SOCLOCK.HFCLKCLKCFG;

    register_temp &= ~(SYSCTL_HFCLKCLKCFG_HFXTRSEL_MASK | SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_MASK);

    switch(range)
    {
        case DL_SYSCTL_HFXT_RANGE_4_8_MHZ  :    register_temp |= (SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE4TO8);   break;
        case DL_SYSCTL_HFXT_RANGE_8_16_MHZ :    register_temp |= (SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE8TO16);  break;
        case DL_SYSCTL_HFXT_RANGE_16_32_MHZ:    register_temp |= (SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE16TO32); break;
        case DL_SYSCTL_HFXT_RANGE_32_48_MHZ:    register_temp |= (SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE32TO48); break;
    }
    register_temp |= (monitorEnable) ? (SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_ENABLE) : (SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_DISABLE);

    SYSCTL->SOCLOCK.HFCLKCLKCFG = register_temp;

	SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_HFXTEN_ENABLE;								// 必须启动 否则频率不准
		
    SYSCTL->SOCLOCK.HSCLKEN =   SYSCTL_HSCLKEN_SYSPLLEN_ENABLE | SYSCTL_HSCLKEN_USEEXTHFCLK_ENABLE;
    SYSCTL->SOCLOCK.HSCLKEN |=  SYSCTL_HSCLKCFG_HSCLKSEL_HFCLKCLK;




}





void DL_SYSCTL_configSYSPLL (const DL_SYSCTL_SYSPLLConfig *config)
{
    uint32_t register_temp = 0;
	
	
			 SYSCTL->SOCLOCK.HSCLKEN &= ~(SYSCTL_HSCLKEN_SYSPLLEN_MASK);

    {
        register_temp = SYSCTL->SOCLOCK.SYSPLLCFG0;

        register_temp   &= ~(SYSCTL_SYSPLLCFG0_SYSPLLREF_MASK   | SYSCTL_SYSPLLCFG0_MCLK2XVCO_MASK  
                        |   SYSCTL_SYSPLLCFG0_ENABLECLK0_MASK   | SYSCTL_SYSPLLCFG0_ENABLECLK1_MASK | SYSCTL_SYSPLLCFG0_ENABLECLK2X_MASK
                        |   SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK     | SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK   | SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK  );
        register_temp   |=  config->rDivClk2x   ;
        register_temp   |=  config->rDivClk1    ;
        register_temp   |=  config->rDivClk0    ;
        register_temp   |=  config->enableCLK2x ;
        register_temp   |=  config->enableCLK1  ;
        register_temp   |=  config->enableCLK0  ;
        register_temp   |=  config->sysPLLMCLK  ;
        register_temp   |=  config->sysPLLRef   ;

        SYSCTL->SOCLOCK.SYSPLLCFG0 = register_temp;
    }
		

    {
        register_temp = SYSCTL->SOCLOCK.SYSPLLCFG1;

        register_temp &= ~(SYSCTL_SYSPLLCFG1_PDIV_MASK | SYSCTL_SYSPLLCFG1_QDIV_MASK);
        register_temp   |=  (config->qDiv << SYSCTL_SYSPLLCFG1_QDIV_OFS );
        register_temp   |=  (config->pDiv);

        SYSCTL->SOCLOCK.SYSPLLCFG1 = register_temp;
    }
		
				{
    SYSCTL->SOCLOCK.SYSPLLPARAM0 =
        *(volatile uint32_t *) ((uint32_t) config->inputFreq);
    SYSCTL->SOCLOCK.SYSPLLPARAM1 =
        *(volatile uint32_t *) ((uint32_t) config->inputFreq + (uint32_t) 0x4);
		}


			SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_SYSPLLEN_ENABLE;

    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_SYSPLLGOOD_MASK) !=
           DL_SYSCTL_CLK_STATUS_SYSPLL_GOOD) {
        ;
						 }
						 
						 


}

void DL_SYSCTL_setLFCLKSourceLFXT (const DL_SYSCTL_LFCLKConfig *config)
{
    uint32_t register_temp = SYSCTL->SOCLOCK.LFCLKCFG;

    register_temp &= ~(SYSCTL_LFCLKCFG_XT1DRIVE_MASK | SYSCTL_LFCLKCFG_MONITOR_MASK | SYSCTL_LFCLKCFG_LOWCAP_MASK);
    register_temp   |=  (config->lowCap) ? (SYSCTL_LFCLKCFG_LOWCAP_ENABLE) : (SYSCTL_LFCLKCFG_LOWCAP_DISABLE);
    register_temp   |=  (config->monitor) ? (SYSCTL_LFCLKCFG_MONITOR_ENABLE) : (SYSCTL_LFCLKCFG_MONITOR_DISABLE);
    register_temp   |=  (config->xt1Drive);

    SYSCTL->SOCLOCK.LFCLKCFG = register_temp;




}




void DL_SYSCTL_switchMCLKfromSYSOSCtoHSCLK (DL_SYSCTL_HSCLK_SOURCE source)
{

    SYSCTL->SOCLOCK.HSCLKCFG = (uint32_t) source;
    SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

}


////////////////////////////////////////////////////////////////////////////

SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setHFCLKSourceHFXTParams(DL_SYSCTL_HFXT_RANGE_32_48_MHZ,0, false);
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_setHFCLKDividerForMFPCLK(DL_SYSCTL_HFCLK_MFPCLK_DIVIDER_10);
    DL_SYSCTL_setLFCLKSourceLFXT((DL_SYSCTL_LFCLKConfig *) &gLFCLKConfig);
    DL_SYSCTL_enableMFCLK();
    DL_SYSCTL_enableMFPCLK();
	DL_SYSCTL_setMFPCLKSource(DL_SYSCTL_MFPCLK_SOURCE_SYSOSC);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);

}



static const DL_VREF_Config gVREFConfig = {
    .vrefEnable     = DL_VREF_ENABLE_DISABLE,
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_2_5V,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
};

SYSCONFIG_WEAK void SYSCFG_DL_VREF_init(void) {
    DL_VREF_configReference(VREF,
        (DL_VREF_Config *) &gVREFConfig);
}


SYSCONFIG_WEAK void SYSCFG_DL_SYSTICK_init(void)
{
    /* Initialize the period to 1.00 μs */
    DL_SYSTICK_init(80);
    /* Enable the SysTick and start counting */
    DL_SYSTICK_enable();
}

