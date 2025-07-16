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
* 文件名称          zf_common_clock
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2025-06-1        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_function.h"
#include "zf_common_interrupt.h"

#include "zf_common_clock.h"

uint32 system_clock = SYSTEM_CLOCK_80M;                                         // 系统时钟信息
uint32 bus_clock = SYSTEM_CLOCK_80M / 2;                                        // 总线时钟信息
    extern void system_delay_init (void);
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     核心时钟恢复初始设置
// 参数说明     void
// 返回参数     void
// 使用示例     clock_reset();
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static void clock_reset (void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);

    DL_TimerA_reset(TIMA0);
    DL_TimerA_reset(TIMA1);
    DL_TimerA_enablePower(TIMA0);
    DL_TimerA_enablePower(TIMA1);

    DL_TimerG_reset(TIMG0 );
    DL_TimerG_reset(TIMG6 );
    DL_TimerG_reset(TIMG7 );
    DL_TimerG_reset(TIMG8 );
    DL_TimerG_reset(TIMG12);
    DL_TimerG_enablePower(TIMG0 );
    DL_TimerG_enablePower(TIMG6 );
    DL_TimerG_enablePower(TIMG7 );
    DL_TimerG_enablePower(TIMG8 );
    DL_TimerG_enablePower(TIMG12);

    DL_UART_Main_reset(UART0);
    DL_UART_Main_reset(UART1);
    DL_UART_Main_reset(UART2);
    DL_UART_Main_enablePower(UART0);
    DL_UART_Main_enablePower(UART1);
    DL_UART_Main_enablePower(UART2);

    DL_SPI_reset(SPI0);
    DL_SPI_reset(SPI1);
    DL_SPI_enablePower(SPI0);
    DL_SPI_enablePower(SPI1);

    DL_ADC12_reset(ADC0);
    DL_ADC12_reset(ADC1);
    DL_ADC12_enablePower(ADC0);
    DL_ADC12_enablePower(ADC1);

    DL_VREF_reset(VREF);
    DL_VREF_enablePower(VREF);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     核心时钟设置
// 参数说明     clock       时钟频率 推荐使用 zf_common_clock.h 中 system_clock_enum 定义的选项
// 返回参数     void
// 使用示例     clock_set_freq(clock);
// 备注信息     本函数在文件内部调用 用户不用关注 也不可修改
//-------------------------------------------------------------------------------------------------------------------
static void clock_set_freq (uint32 clock)
{
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     核心时钟初始化
// 参数说明     clock       时钟频率 推荐使用 zf_common_clock.h 中 system_clock_enum 定义的选项
// 返回参数     void
// 使用示例     clock_init(SYSTEM_CLOCK_80M);
//-------------------------------------------------------------------------------------------------------------------
void clock_init (uint32 clock)
{
    clock_reset();
		SYSCFG_DL_init();
    interrupt_init();
    system_delay_init();
}
