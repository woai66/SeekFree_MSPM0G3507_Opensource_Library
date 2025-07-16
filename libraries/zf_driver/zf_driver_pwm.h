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
* 文件名称          zf_driver_pwm
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

#ifndef _zf_driver_pwm_h_
#define _zf_driver_pwm_h_

#include "zf_common_typedef.h"

#include "zf_driver_timer.h"
#include "zf_driver_gpio.h"

#define PWM_DUTY_MAX        ( 10000 )

#define PWM_NUM_MAX         ( 7     )                                           // 总共最多 PWM_NUM_MAX 个 PWM

typedef enum                                                                    // 枚举 PWM
{
    PWM_TIM_A0      ,
    PWM_TIM_A1      ,
    PWM_TIM_G0      ,
    PWM_TIM_G6      ,
    PWM_TIM_G7      ,
    PWM_TIM_G8      ,
    PWM_TIM_G12     ,
}pwm_index_enum;

// 定义引脚要用的宏定义
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 PWM 模块中为 PWM 索引
// bit[23:20] 在 PWM 模块中为 CHANNEL 索引

#define     PWM_PIN_INDEX_OFFSET    ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     PWM_PIN_INDEX_MASK      ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     PWM_PIN_AF_OFFSET       ( 12     )                                  // bit[15:12] 存储 GPIO 的复用功能索引
#define     PWM_PIN_AF_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

#define     PWM_INDEX_OFFSET        ( 16     )                                  // bit[19:16] 存储 PWM 索引
#define     PWM_INDEX_MASK          ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

#define     PWM_CHANNEL_OFFSET      ( 20     )                                  // bit[23:20] 存储 CHANNEL 索引
#define     PWM_CHANNEL_MASK        ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 PWM 通道索引 此枚举定义不允许用户修改
{
    PWM_CH0     ,
    PWM_CH1     ,
    PWM_CH2     ,
    PWM_CH3     ,
}pwm_channel_index_enum;

typedef enum                                                                    // 枚举 PWM 通道    此枚举定义不允许用户修改
{
    // ----------------------------------------------- PWM_TIM_A0 PWM CHANNEL -----------------------------------------------
    PWM_TIM_A0_CH0_A0   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A0 )),
    PWM_TIM_A0_CH0_A8   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A8 )),
    PWM_TIM_A0_CH0_A21  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A21)),
    PWM_TIM_A0_CH0_B8   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B8 )),
    PWM_TIM_A0_CH0_B14  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B14)),

    PWM_TIM_A0_CH1_A1   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A1 )),
    PWM_TIM_A0_CH1_A3   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A3 )),
    PWM_TIM_A0_CH1_A7   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A7 )),
    PWM_TIM_A0_CH1_A9   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A9 )),
    PWM_TIM_A0_CH1_A22  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A22)),
    PWM_TIM_A0_CH1_B9   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B9 )),
    PWM_TIM_A0_CH1_B12  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B12)),
    PWM_TIM_A0_CH1_B20  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B20)),

    PWM_TIM_A0_CH2_A3   = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A3 )),
    PWM_TIM_A0_CH2_A7   = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A7 )),
    PWM_TIM_A0_CH2_A10  = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A10)),
    PWM_TIM_A0_CH2_A15  = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A15)),
    PWM_TIM_A0_CH2_B0   = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B0 )),
    PWM_TIM_A0_CH2_B4   = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B4 )),
    PWM_TIM_A0_CH2_B12  = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B12)),
    PWM_TIM_A0_CH2_B17  = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B17)),
    PWM_TIM_A0_CH2_B20  = ((PWM_CH2  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B20)),

    PWM_TIM_A0_CH3_A4   = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A4 )),
    PWM_TIM_A0_CH3_A12  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A12)),
    PWM_TIM_A0_CH3_A17  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A17)),
    PWM_TIM_A0_CH3_A23  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A23)),
    PWM_TIM_A0_CH3_A25  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A25)),
    PWM_TIM_A0_CH3_A28  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A28)),
    PWM_TIM_A0_CH3_B2   = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B2 )),
    PWM_TIM_A0_CH3_B13  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B13)),
    PWM_TIM_A0_CH3_B24  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B24)),
    PWM_TIM_A0_CH3_B26  = ((PWM_CH3  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B26)),
    // ----------------------------------------------- PWM_TIM_A0 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_A1 PWM CHANNEL -----------------------------------------------
    PWM_TIM_A1_CH0_A10  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A10)),
    PWM_TIM_A1_CH0_A15  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A15)),
    PWM_TIM_A1_CH0_A17  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A17)),
    PWM_TIM_A1_CH0_A28  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A28)),
    PWM_TIM_A1_CH0_B0   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B0 )),
    PWM_TIM_A1_CH0_B2   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (B2 )),
    PWM_TIM_A1_CH0_B4   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B4 )),
    PWM_TIM_A1_CH0_B17  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B17)),
    PWM_TIM_A1_CH0_B26  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B26)),

    PWM_TIM_A1_CH1_A11  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A11)),
    PWM_TIM_A1_CH1_A16  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A16)),
    PWM_TIM_A1_CH1_A18  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A18)),
    PWM_TIM_A1_CH1_A24  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A24)),
    PWM_TIM_A1_CH1_A31  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A31)),
    PWM_TIM_A1_CH1_B1   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B1 )),
    PWM_TIM_A1_CH1_B3   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (B3 )),
    PWM_TIM_A1_CH1_B5   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B5 )),
    PWM_TIM_A1_CH1_B18  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B18)),
    PWM_TIM_A1_CH1_B27  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_A1  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B27)),
    // ----------------------------------------------- PWM_TIM_A1 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_G0 PWM CHANNEL -----------------------------------------------
    PWM_TIM_G0_CH0_A5   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A5 )),
    PWM_TIM_G0_CH0_A12  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A12)),
    PWM_TIM_G0_CH0_A23  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A23)),
    PWM_TIM_G0_CH0_B10  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (B10)),

    PWM_TIM_G0_CH1_A6   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A6 )),
    PWM_TIM_G0_CH1_A13  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A13)),
    PWM_TIM_G0_CH1_A24  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A24)),
    PWM_TIM_G0_CH1_B11  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G0  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (B11)),
    // ----------------------------------------------- PWM_TIM_G0 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_G6 PWM CHANNEL -----------------------------------------------
    PWM_TIM_G6_CH0_A5   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A5 )),
    PWM_TIM_G6_CH0_A21  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A21)),
    PWM_TIM_G6_CH0_A29  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A29)),
    PWM_TIM_G6_CH0_B2   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B2 )),
    PWM_TIM_G6_CH0_B6   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B6 )),
    PWM_TIM_G6_CH0_B10  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B10)),
    PWM_TIM_G6_CH0_B26  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B26)),

    PWM_TIM_G6_CH1_A6   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A6 )),
    PWM_TIM_G6_CH1_A22  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A22)),
    PWM_TIM_G6_CH1_A30  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A30)),
    PWM_TIM_G6_CH1_B3   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B3 )),
    PWM_TIM_G6_CH1_B7   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (B7 )),
    PWM_TIM_G6_CH1_B11  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B11)),
    PWM_TIM_G6_CH1_B27  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G6  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B27)),
    // ----------------------------------------------- PWM_TIM_G6 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_G7 PWM CHANNEL -----------------------------------------------
    PWM_TIM_G7_CH0_A3   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A3 )),
    PWM_TIM_G7_CH0_A17  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A17)),
    PWM_TIM_G7_CH0_A23  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A23)),
    PWM_TIM_G7_CH0_A26  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A26)),
    PWM_TIM_G7_CH0_A28  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A28)),
    PWM_TIM_G7_CH0_B15  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B15)),

    PWM_TIM_G7_CH1_A2   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A2 )),
    PWM_TIM_G7_CH1_A4   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A4 )),
    PWM_TIM_G7_CH1_A7   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A7 )),
    PWM_TIM_G7_CH1_A18  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A18)),
    PWM_TIM_G7_CH1_A24  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A24)),
    PWM_TIM_G7_CH1_A27  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A27)),
    PWM_TIM_G7_CH1_A31  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A31)),
    PWM_TIM_G7_CH1_B16  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B16)),
    PWM_TIM_G7_CH1_B19  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G7  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (B19)),
    // ----------------------------------------------- PWM_TIM_G7 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_G8 PWM CHANNEL -----------------------------------------------
    PWM_TIM_G8_CH0_A1   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF7  << PWM_PIN_AF_OFFSET) | (A1 )),
    PWM_TIM_G8_CH0_A3   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A3 )),
    PWM_TIM_G8_CH0_A5   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A5 )),
    PWM_TIM_G8_CH0_A7   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A7 )),
    PWM_TIM_G8_CH0_A21  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (A21)),
    PWM_TIM_G8_CH0_A23  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF8  << PWM_PIN_AF_OFFSET) | (A23)),
    PWM_TIM_G8_CH0_A26  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A26)),
    PWM_TIM_G8_CH0_A29  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A29)),
    PWM_TIM_G8_CH0_B6   = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B6 )),
    PWM_TIM_G8_CH0_B10  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B10)),
    PWM_TIM_G8_CH0_B15  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B15)),
    PWM_TIM_G8_CH0_B21  = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B21)),

    PWM_TIM_G8_CH1_A0   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A0 )),
    PWM_TIM_G8_CH1_A2   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A2 )),
    PWM_TIM_G8_CH1_A4   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A4 )),
    PWM_TIM_G8_CH1_A6   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF2  << PWM_PIN_AF_OFFSET) | (A6 )),
    PWM_TIM_G8_CH1_A22  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (A22)),
    PWM_TIM_G8_CH1_A27  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A27)),
    PWM_TIM_G8_CH1_A30  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A30)),
    PWM_TIM_G8_CH1_B7   = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B7 )),
    PWM_TIM_G8_CH1_B11  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B11)),
    PWM_TIM_G8_CH1_B16  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B16)),
    PWM_TIM_G8_CH1_B19  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B19)),
    PWM_TIM_G8_CH1_B22  = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G8  << PWM_INDEX_OFFSET) | (GPIO_AF3  << PWM_PIN_AF_OFFSET) | (B22)),
    // ----------------------------------------------- PWM_TIM_G8 PWM CHANNEL -----------------------------------------------

    // ----------------------------------------------- PWM_TIM_G12 PWM CHANNEL -----------------------------------------------
    PWM_TIM_G12_CH0_A10 = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF6  << PWM_PIN_AF_OFFSET) | (A10)),
    PWM_TIM_G12_CH0_A14 = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A14)),
    PWM_TIM_G12_CH0_B13 = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (B13)),
    PWM_TIM_G12_CH0_B20 = ((PWM_CH0  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B20)),

    PWM_TIM_G12_CH1_A25 = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF4  << PWM_PIN_AF_OFFSET) | (A25)),
    PWM_TIM_G12_CH1_A31 = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (A31)),
    PWM_TIM_G12_CH1_B14 = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B14)),
    PWM_TIM_G12_CH1_B24 = ((PWM_CH1  << PWM_CHANNEL_OFFSET) | (PWM_TIM_G12 << PWM_INDEX_OFFSET) | (GPIO_AF5  << PWM_PIN_AF_OFFSET) | (B24)),
    // ----------------------------------------------- PWM_TIM_G12 PWM CHANNEL -----------------------------------------------
}pwm_channel_enum;

void    pwm_set_duty    (pwm_channel_enum pin, const uint32 duty);
void    pwm_init        (pwm_channel_enum pin, const uint32 freq, const uint32 duty);

#endif
