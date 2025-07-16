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

#ifndef _zf_driver_gpio_h_
#define _zf_driver_gpio_h_

#include "ti_msp_dl_config.h"

#include "zf_common_typedef.h"

// 根据实际的芯片来进行定义
#define     GPIO_GROUP_MAX                  (  2 )                              // 引脚组数量
#define     GPIO_GROUP_PIN_NUMBER_MAX       ( 32 )                              // 每个组引脚上限

// 宏定义函数需要通过枚举来检索 IO 因此需要获取 IO 组别和下标信息 这里就定义偏移量和掩码
#define     GPIO_PIN_INDEX_OFFSET           ( 0    )                            // 将下标信息保存在低位 方便直接使用
#define     GPIO_PIN_INDEX_MASK             ( 0x1F )                            // 由于范围是 [0, 31] 占 5bit 掩码就是 0x1F

#define     GPIO_GROUP_INDEX_OFFSET         ( 5    )                            // 低 5bit 存储了下标 那么位移 5
#define     GPIO_GROUP_INDEX_MASK           ( 0x07 )                            // 这里根据情况来设置掩码

typedef enum                                                                    // 枚举 GPIO 引脚  此枚举定义不允许用户修改
{
    A0 = GPIO_GROUP_PIN_NUMBER_MAX * 0,
            A1 ,    A2 ,    A3 ,    A4 ,    A5 ,    A6 ,    A7 ,
    A8 ,    A9 ,    A10,    A11,    A12,    A13,    A14,    A15,
    A16,    A17,    A18,    A19,    A20,    A21,    A22,    A23,
    A24,    A25,    A26,    A27,    A28,    A29,    A30,    A31,

    B0 = GPIO_GROUP_PIN_NUMBER_MAX * 1,
            B1 ,    B2 ,    B3 ,    B4 ,    B5 ,    B6 ,    B7 ,
    B8 ,    B9 ,    B10,    B11,    B12,    B13,    B14,    B15,
    B16,    B17,    B18,    B19,    B20,    B21,    B22,    B23,
    B24,    B25,    B26,    B27,
}gpio_pin_enum;

typedef enum                                                                    // 枚举端口方向   此枚举定义不允许用户修改
{
    GPI = 0x00,                                                                 // 定义管脚输入
    GPO = 0x10,                                                                 // 定义管脚输出
}gpio_dir_enum;

typedef enum                                                                    // 枚举端口模式   此枚举定义不允许用户修改
{
    GPI_ANAOG_IN        = 0x01,                                                 // [输入] [定义管脚模拟输入]
    GPI_FLOATING_IN     = 0x02,                                                 // [输入] [定义管脚浮空输入]
    GPI_PULL_DOWN       = 0x03,                                                 // [输入] [定义管脚下拉输入]
    GPI_PULL_UP         = 0x04,                                                 // [输入] [定义管脚上拉输入]

    GPO_PUSH_PULL       = 0x11,                                                 // [输出] [定义管脚推挽输出]
    GPO_OPEN_DTAIN      = 0x12,                                                 // [输出] [定义管脚开漏输出]
    GPO_AF_PUSH_PULL    = 0x13,                                                 // [输出] [定义管脚复用推挽输出]
    GPO_AF_OPEN_DTAIN   = 0x14,                                                 // [输出] [定义管脚复用开漏输出]
}gpio_mode_enum;

typedef enum                                                                    // 枚举端口复用   此枚举定义不允许用户修改
{
    GPIO_AF0            = 0 ,                                                   // 引脚复用功能选项  0
    GPIO_AF1            = 1 ,                                                   // 引脚复用功能选项  1
    GPIO_AF2            = 2 ,                                                   // 引脚复用功能选项  2
    GPIO_AF3            = 3 ,                                                   // 引脚复用功能选项  3
    GPIO_AF4            = 4 ,                                                   // 引脚复用功能选项  4
    GPIO_AF5            = 5 ,                                                   // 引脚复用功能选项  5
    GPIO_AF6            = 6 ,                                                   // 引脚复用功能选项  6
    GPIO_AF7            = 7 ,                                                   // 引脚复用功能选项  7
    GPIO_AF8            = 8 ,                                                   // 引脚复用功能选项  8
    GPIO_AF9            = 9 ,                                                   // 引脚复用功能选项  9
    GPIO_AF10           = 10,                                                   // 引脚复用功能选项 10
    GPIO_AF11           = 11,                                                   // 引脚复用功能选项 11
    GPIO_AF12           = 12,                                                   // 引脚复用功能选项 12
    GPIO_AF13           = 13,                                                   // 引脚复用功能选项 13
    GPIO_AF14           = 14,                                                   // 引脚复用功能选项 14
    GPIO_AF15           = 15,                                                   // 引脚复用功能选项 15
}gpio_af_enum;

typedef enum                                                                    // 枚举端口电平   此枚举定义不允许用户修改
{
    GPIO_LOW            = 0x00,                                                 // 定义管脚初始化电平为低
    GPIO_HIGH           = 0x01,                                                 // 定义管脚初始化电平为高
}gpio_level_enum;

extern GPIO_Regs *gpio_group[GPIO_GROUP_MAX];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 复位为低电平
// 参数说明     x           选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_low(D5);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define     gpio_low(pin)               (gpio_group[((pin) >> GPIO_GROUP_INDEX_OFFSET)]->DOUTCLR31_0 |= (0x00000001 << ((pin) & GPIO_PIN_INDEX_MASK)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 置位为高电平
// 参数说明     x           选择的引脚 (可选择范围由 zf_driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_high(D5);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#define     gpio_high(pin)              (gpio_group[((pin) >> GPIO_GROUP_INDEX_OFFSET)]->DOUTSET31_0 |= (0x00000001 << ((pin) & GPIO_PIN_INDEX_MASK)))

void        gpio_set_level              (gpio_pin_enum pin, const uint8 dat);
uint8       gpio_get_level              (gpio_pin_enum pin);
void        gpio_toggle_level           (gpio_pin_enum pin);
void        gpio_set_dir                (gpio_pin_enum pin, gpio_dir_enum dir, gpio_mode_enum mode);

void        gpio_init                   (gpio_pin_enum pin, gpio_dir_enum dir, const uint8 dat, gpio_mode_enum mode);
void        afio_init                   (gpio_pin_enum pin, gpio_dir_enum dir, gpio_af_enum af, gpio_mode_enum mode);

#ifdef COMPATIBLE_WITH_OLDER_VERSIONS                                           // 兼容旧版本开源库接口名称
#define     gpio_set(pin, dat)          (gpio_set_level((pin), (dat)))
#define     gpio_get(pin)               (gpio_get_level((pin)))
#define     gpio_dir(pin, dir, mode)    (gpio_set_dir((pin), (dir), (mode)))
#define     gpio_toggle(pin)            (gpio_toggle_level((pin)))
#endif

#endif
