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
* 文件名称          zf_driver_gpio
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

#include "zf_driver_gpio.h"

GPIO_Regs *gpio_group[GPIO_GROUP_MAX] = {GPIOA, GPIOB};

const uint8 gpio_iomux_index[60] = 
{
//  A0 ,    A1 ,    A2 ,    A3 ,    A4 ,    A5 ,    A6 ,    A7 ,
    1  - 1, 2  - 1, 7  - 1, 8  - 1, 9  - 1, 10 - 1, 11 - 1, 14 - 1,
//  A8 ,    A9 ,    A10,    A11,    A12,    A13,    A14,    A15,
    19 - 1, 20 - 1, 21 - 1, 22 - 1, 34 - 1, 35 - 1, 36 - 1, 37 - 1,
//  A16,    A17,    A18,    A19,    A20,    A21,    A22,    A23,
    38 - 1, 39 - 1, 40 - 1, 41 - 1, 42 - 1, 46 - 1, 47 - 1, 53 - 1,
//  A24,    A25,    A26,    A27,    A28,    A29,    A30,    A31,
    54 - 1, 55 - 1, 59 - 1, 60 - 1, 3  - 1, 4  - 1, 5  - 1, 6  - 1,

//  B0 ,    B1 ,    B2 ,    B3 ,    B4 ,    B5 ,    B6 ,    B7 ,
    12 - 1, 13 - 1, 15 - 1, 16 - 1, 17 - 1, 18 - 1, 23 - 1, 24 - 1,
//  B8 ,    B9 ,    B10,    B11,    B12,    B13,    B14,    B15,
    25 - 1, 26 - 1, 27 - 1, 28 - 1, 29 - 1, 30 - 1, 31 - 1, 32 - 1,
//  B16,    B17,    B18,    B19,    B20,    B21,    B22,    B23,
    33 - 1, 43 - 1, 44 - 1, 45 - 1, 48 - 1, 49 - 1, 50 - 1, 51 - 1,
//  B24,    B25,    B26,    B27,
    52 - 1, 56 - 1, 57 - 1, 58 - 1,
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 输出设置
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     dat         0：低电平 1：高电平
// 返回参数     void
// 使用示例     gpio_set_level(D5, 1);                                          // D5 输出高电平
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_level (gpio_pin_enum pin, const uint8 dat)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);

    (   (dat) ?
        (gpio_group[io_group]->DOUTSET31_0 |= (0x00000001 << io_pin)) :
        (gpio_group[io_group]->DOUTCLR31_0 |= (0x00000001 << io_pin)));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 电平获取
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     uint8       引脚当前电平
// 使用示例     uint8 status = gpio_get_level(D5);                              // 获取 D5 引脚电平
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get_level (gpio_pin_enum pin)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);

    return ((gpio_group[io_group]->DIN31_0 & (0x00000001 << io_pin)) ? (1) : (0));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 翻转电平
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_toggle_level(D5);                                          // 翻转 D5 电平
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle_level (gpio_pin_enum pin)
{
    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);

    if((gpio_group[io_group]->DOUT31_0 & (0x00000001 << (pin & 0x1F))))
    {
        (gpio_group[io_group]->DOUTCLR31_0 |= (0x00000001 << io_pin));
    }
    else
    {
        (gpio_group[io_group]->DOUTSET31_0 |= (0x00000001 << io_pin));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 方向设置
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     dir         引脚的方向   输出：GPO   输入：GPI
// 参数说明     mode        引脚的模式 (可选择范围由 zf_driver_gpio.h 内 gpio_mode_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_set_dir(D5, GPI, GPI_PULL_UP);                             // 设置 D5 为上拉输入
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_dir (gpio_pin_enum pin, gpio_dir_enum dir, gpio_mode_enum mode)
{
    zf_assert(((GPO == dir) && (GPO == (dir & mode))) || ((GPI == dir) && (GPO != (dir & mode))));

    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    uint32  register_temp   = (GPO_AF_PUSH_PULL == mode || GPO_AF_OPEN_DTAIN == mode)
                            ? (IOMUX->SECCFG.PINCM[gpio_iomux_index[io_group * 32 + io_pin]] & IOMUX_PINCM_PF_MASK)
                            : 0; 

    register_temp |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE);
    switch(mode)
    {
        case GPI_ANAOG_IN       : register_temp  = 0;                           break;
        case GPI_FLOATING_IN    : register_temp |= 0;                           break;
        case GPI_PULL_DOWN      : register_temp |= IOMUX_PINCM_PIPD_ENABLE;     break;
        case GPI_PULL_UP        : register_temp |= IOMUX_PINCM_PIPU_ENABLE;     break;

        case GPO_PUSH_PULL      : register_temp |= 0;                           break;
        case GPO_OPEN_DTAIN     : register_temp |= IOMUX_PINCM_HIZ1_ENABLE;     break;
        case GPO_AF_PUSH_PULL   : register_temp |= 0;                           break;
        case GPO_AF_OPEN_DTAIN  : register_temp |= IOMUX_PINCM_HIZ1_ENABLE;     break;
    }

    IOMUX->SECCFG.PINCM[gpio_iomux_index[io_group * 32 + io_pin]] = register_temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 初始化
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     mode        引脚的方向 [GPI/GPIO]
// 参数说明     dat         引脚初始化时设置的电平状态，输出时有效 0：低电平 1：高电平 仅在设置为输出模式时有效
// 参数说明     mode        引脚的模式 (可选择范围由 zf_driver_gpio.h 内 gpio_mode_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_init(D1, GPI, GPIO_HIGH, GPI_PULL_UP);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gpio_init (gpio_pin_enum pin, gpio_dir_enum dir, const uint8 dat, gpio_mode_enum mode)
{
    zf_assert(((GPO == dir) && (GPO == (dir & mode))) || ((GPI == dir) && (GPO != (dir & mode))));

    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    uint32  register_temp   = 0; 

    register_temp |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE);
    switch(mode)
    {
        case GPI_ANAOG_IN       : register_temp  = 0;                                       break;
        case GPI_FLOATING_IN    : register_temp |= (0x00000001);                            break;
        case GPI_PULL_DOWN      : register_temp |= (0x00000001 | IOMUX_PINCM_PIPD_ENABLE);  break;
        case GPI_PULL_UP        : register_temp |= (0x00000001 | IOMUX_PINCM_PIPU_ENABLE);  break;

        case GPO_PUSH_PULL      : register_temp |= (0x00000001);                            break;
        case GPO_OPEN_DTAIN     : register_temp |= (0x00000001 | IOMUX_PINCM_HIZ1_ENABLE);  break;
        case GPO_AF_PUSH_PULL   : register_temp |= 0;                                       break;
        case GPO_AF_OPEN_DTAIN  : register_temp |= IOMUX_PINCM_HIZ1_ENABLE;                 break;
		
    }

    IOMUX->SECCFG.PINCM[gpio_iomux_index[io_group * 32 + io_pin]] = register_temp;

    if(GPO == dir)
    {
		gpio_group[io_group]->DOESET31_0 = (0x00000001 << io_pin);
        gpio_set_level(pin, dat);                                               // 初始化电平设置
    }
    if(GPI_PULL_DOWN == mode)
    {
        gpio_set_level(pin, GPIO_LOW);
    }
    if(GPI_PULL_UP == mode)
    {
        gpio_set_level(pin, GPIO_HIGH);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 复用功能初始化 一般是内部调用
// 参数说明     pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     mode        引脚的方向 [GPI/GPIO]
// 参数说明     af          引脚的功能选择 (可选择范围由 zf_driver_gpio.h 内 gpio_af_enum 枚举值确定)
// 参数说明     mode        引脚的模式 (可选择范围由 zf_driver_gpio.h 内 gpio_mode_enum 枚举值确定)
// 返回参数     void
// 使用示例     afio_init(D5, GPO, GPIO_AF0, GPO_AF_PUSH_PULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void afio_init (gpio_pin_enum pin, gpio_dir_enum dir, gpio_af_enum af, gpio_mode_enum mode)
{
    zf_assert(((GPO == dir) && (GPO == (dir & mode))) || ((GPI == dir) && (GPO != (dir & mode))));

    uint8   io_group        = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8   io_pin          = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);
    uint32  register_temp   = af;

    register_temp |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE);
    switch(mode)
    {
        case GPI_ANAOG_IN       : register_temp  = 0;                           break;
        case GPI_FLOATING_IN    : register_temp |= 0;                           break;
        case GPI_PULL_DOWN      : register_temp |= IOMUX_PINCM_PIPD_ENABLE;     break;
        case GPI_PULL_UP        : register_temp |= IOMUX_PINCM_PIPU_ENABLE;     break;

        case GPO_PUSH_PULL      : register_temp |= 0;                           break;
        case GPO_OPEN_DTAIN     : register_temp |= IOMUX_PINCM_HIZ1_ENABLE;     break;
        case GPO_AF_PUSH_PULL   : register_temp |= 0;                           break;
        case GPO_AF_OPEN_DTAIN  : register_temp |= IOMUX_PINCM_HIZ1_ENABLE;     break;
    }

    IOMUX->SECCFG.PINCM[gpio_iomux_index[io_group * 32 + io_pin]] = register_temp;
}
