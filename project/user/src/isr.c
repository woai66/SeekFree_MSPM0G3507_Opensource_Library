/*********************************************************************************************************************
* MSPM0G3507 Opensource Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          isr
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
********************************************************************************************************************/


#include "isr.h"

void TIMA0_IRQHandler (void)
{
    pit_callback_list[0](0, pit_callback_ptr_list[0]);
}

void TIMA1_IRQHandler (void)
{
    pit_callback_list[1](0, pit_callback_ptr_list[1]);
}

void TIMG0_IRQHandler (void)
{
    pit_callback_list[2](0, pit_callback_ptr_list[2]);
}

void TIMG6_IRQHandler (void)
{
    pit_callback_list[3](0, pit_callback_ptr_list[3]);
}

void TIMG7_IRQHandler (void)
{
    pit_callback_list[4](0, pit_callback_ptr_list[4]);
}

void TIMG8_IRQHandler (void)
{
    pit_callback_list[5](0, pit_callback_ptr_list[5]);
}

void TIMG12_IRQHandler (void)
{
    pit_callback_list[6](0, pit_callback_ptr_list[6]);
}

void UART0_IRQHandler (void)
{
	switch(DL_UART_getPendingInterrupt(UART0))
	{
		case DL_UART_IIDX_TX:
        {
            uart_callback_list[0](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[0]);
        }break;
		case DL_UART_IIDX_RX:
        {
            uart_callback_list[0](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[0]);
#if DEBUG_UART_USE_INTERRUPT
                debug_interrupr_handler();
#endif
        }break;

		default:    break;
	}
    DL_UART_clearInterruptStatus(UART0, UART0->CPU_INT.RIS);
}

void UART1_IRQHandler (void)
{
	switch(DL_UART_getPendingInterrupt(UART1))
	{
		case DL_UART_IIDX_TX:
        {
            uart_callback_list[1](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[1]);
        }break;
		case DL_UART_IIDX_RX:
        {
            uart_callback_list[1](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[1]);
					
					 //wifi_uart_callback();
			
			wireless_module_uart_handler();                 // ??????????
					
        }break;

		default:    break;
	}
    DL_UART_clearInterruptStatus(UART1, UART1->CPU_INT.RIS);
}

void UART2_IRQHandler (void)
{
	switch(DL_UART_getPendingInterrupt(UART2))
	{
		case DL_UART_IIDX_TX:
        {
            uart_callback_list[2](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[2]);
        }break;
		case DL_UART_IIDX_RX:
        {
            uart_callback_list[2](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[2]);
        }break;

		default:    break;
	}
    DL_UART_clearInterruptStatus(UART2, UART2->CPU_INT.RIS);
}

void UART3_IRQHandler (void)
{
	switch(DL_UART_getPendingInterrupt(UART3))
	{
		case DL_UART_IIDX_TX:
        {
            uart_callback_list[3](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[3]);
        }break;
		case DL_UART_IIDX_RX:
        {
            uart_callback_list[3](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[3]);
        }break;

		default:    break;
	}
    DL_UART_clearInterruptStatus(UART3, UART3->CPU_INT.RIS);
}

void GROUP1_IRQHandler (void)
{
    uint8 exti_index = 0;
    uint8 exti_event = 0;

    uint32  register_temp = gpio_group[0]->CPU_INT.IIDX;
    if(register_temp)
    {
        exti_index = register_temp - 1;

        if(15 >= exti_index)
        {
            exti_event  = (gpio_group[0]->POLARITY15_0 >> ((exti_index % 16) * 2)) & 0x03;
        }
        else
        {
            exti_event  = (gpio_group[0]->POLARITY31_16 >> ((exti_index % 16) * 2)) & 0x03;
        }
    }
    else
    {
        register_temp = gpio_group[1]->CPU_INT.IIDX;
        exti_index = register_temp - 1;

        if(15 >= exti_index)
        {
            exti_event  = (gpio_group[1]->POLARITY15_0 >> ((exti_index % 16) * 2)) & 0x03;
        }
        else
        {
            exti_event  = (gpio_group[1]->POLARITY31_16 >> ((exti_index % 16) * 2)) & 0x03;
        }
    }

    exti_callback_list[exti_index](exti_event, exti_callback_ptr_list[exti_index]);
}
