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
* 文件名称          zf_driver_exti
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

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_exti.h"

static  void exti_callbakc_defalut (uint32 event, void *ptr);
        void_callback_uint32_ptr exti_callback_list[GPIO_GROUP_PIN_NUMBER_MAX] = 
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
        void *exti_callback_ptr_list[GPIO_GROUP_PIN_NUMBER_MAX] = 
{
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut,
    exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut, exti_callbakc_defalut
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void exti_callbakc_defalut (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断使能
// 参数说明     pin                 选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     exti_enable(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void exti_enable (gpio_pin_enum pin)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    gpio_group[io_group]->CPU_INT.IMASK |= (0x00000001 << io_pin);
    interrupt_enable(GPIOA_INT_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断失能
// 参数说明     pin                 选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     exti_disable(pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void exti_disable (gpio_pin_enum pin)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    gpio_group[io_group]->CPU_INT.IMASK &= ~(0x00000001 << io_pin);

    if(!gpio_group[io_group]->CPU_INT.IMASK)
    {
        interrupt_disable(GPIOA_INT_IRQn);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断初始化
// 参数说明     pin                 选择 EXTI 引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     trigger             选择触发的信号方式 [EXTI_TRIGGER_RISING/EXTI_TRIGGER_FALLING/EXTI_TRIGGER_BOTH]
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     exti_init(pin, trigger, callback, ptrs);
// 备注信息     以 Line 为准 例如初始化了 A0 就不能初始化 B0 否则就会导致两个 IO 的中断冲突
//-------------------------------------------------------------------------------------------------------------------
void exti_init (gpio_pin_enum pin, exti_trigger_enum trigger, void_callback_uint32_ptr callback, void *ptr)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    uint32  register_temp   = 0;

    gpio_init(pin, GPI, GPIO_HIGH, GPI_PULL_UP);                                // 初始化选中的引脚

    exti_callback_ptr_list[io_pin]  = ptr;
    exti_callback_list[io_pin]      = (NULL == callback) ? (exti_callbakc_defalut) : (callback);

    if(15 >= io_pin)
    {
        register_temp  = gpio_group[io_group]->POLARITY15_0;
        register_temp &= ~(0x00000003 << ((io_pin % 16) * 2));
        register_temp |= ((uint32)trigger << ((io_pin % 16) * 2));
        gpio_group[io_group]->POLARITY15_0 = register_temp;
    }
    else
    {
        register_temp = gpio_group[io_group]->POLARITY31_16;
        register_temp &= ~(0x00000003 << ((io_pin % 16) * 2));
        register_temp |= ((uint32)trigger << ((io_pin % 16) * 2));
        gpio_group[io_group]->POLARITY31_16 = register_temp;
    }
    gpio_group[io_group]->CPU_INT.ICLR  |= (0x00000001 << io_pin);
    gpio_group[io_group]->CPU_INT.IMASK |= (0x00000001 << io_pin);

    interrupt_enable(GPIOA_INT_IRQn);
}
