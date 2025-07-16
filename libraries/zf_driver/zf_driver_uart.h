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
* 文件名称          zf_driver_uart
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

#ifndef _zf_driver_uart_h_
#define _zf_driver_uart_h_

#include "ti_msp_dl_config.h"

#include "zf_common_typedef.h"

#include "zf_driver_gpio.h"

#define     UART_NUM                     ( 4 )

typedef enum                                                                    // 枚举串口号 此枚举定义不允许用户修改
{
    UART_0,
    UART_1,
    UART_2,
    UART_3,
}uart_index_enum;

// 定义引脚要用的宏定义 根据各单片机不同 可以自由定义需要什么信息
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 UART 模块中为 UART 索引

#define     UART_PIN_INDEX_OFFSET   ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     UART_PIN_INDEX_MASK     ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     UART_PIN_AF_OFFSET      ( 12     )                                  // bit[15:12] 存储 GPIO 的复用功能索引
#define     UART_PIN_AF_MASK        ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

#define     UART_INDEX_OFFSET       ( 16     )                                  // bit[19:16] 存储 UART 索引
#define     UART_INDEX_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

typedef enum                                                                    // 枚举串口引脚 此枚举定义不允许用户修改
{
    // UART 0 TX 引脚
    UART0_TX_A0     = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A0 )),
    UART0_TX_A10    = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A10)),
    UART0_TX_A28    = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A28)),
    UART0_TX_B0     = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B0 )),

    // UART 1 TX 引脚
    UART1_TX_A8     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A8 )),
    UART1_TX_A17    = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A17)),
    UART1_TX_B4     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B4 )),
    UART1_TX_B6     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B6 )),

    // UART 2 TX 引脚
    UART2_TX_A21    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A21)),
    UART2_TX_A23    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A23)),
    UART2_TX_B15    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B15)),
    UART2_TX_B17    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B17)),

    // UART 3 TX 引脚
    UART3_TX_A14    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF4  << UART_PIN_AF_OFFSET) | (A14)),
    UART3_TX_A26    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A26)),
    UART3_TX_B2     = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B2 )),
    UART3_TX_B12    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B12)),
}uart_tx_pin_enum;

typedef enum                                                                // 枚举串口引脚 此枚举定义不允许用户修改
{
    // UART 0 RX 引脚
    UART0_RX_A1     = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A1 )),
    UART0_RX_A11    = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A11)),
    UART0_RX_A31    = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A31)),
    UART0_RX_B1     = ((UART_0 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B1 )),

    // UART 1 RX 引脚
    UART1_RX_A9     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A9 )),
    UART1_RX_A18    = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A18)),
    UART1_RX_B5     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B5 )),
    UART1_RX_B7     = ((UART_1 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B7 )),

    // UART 2 RX 引脚
    UART2_RX_A22    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A22)),
    UART2_RX_A24    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A24)),
    UART2_RX_B16    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B16)),
    UART2_RX_B18    = ((UART_2 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B18)),

    // UART 3 RX 引脚
    UART3_RX_A13    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF4  << UART_PIN_AF_OFFSET) | (A13)),
    UART3_RX_A25    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (A25)),
    UART3_RX_B3     = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B3 )),
    UART3_RX_B13    = ((UART_3 << UART_INDEX_OFFSET) | (GPIO_AF2  << UART_PIN_AF_OFFSET) | (B13)),
}uart_rx_pin_enum;

typedef enum                                                                    // 枚举 UART 中断配置 此枚举定义不允许用户修改
{
    UART_INTERRUPT_CONFIG_TX_DISABLE  ,                                         // 禁止 发送中断
    UART_INTERRUPT_CONFIG_RX_DISABLE  ,                                         // 禁止 接收中断
    UART_INTERRUPT_CONFIG_ALL_DISABLE ,                                         // 禁止 所有中断

    UART_INTERRUPT_CONFIG_TX_ENABLE   ,                                         // 使能 发送中断
    UART_INTERRUPT_CONFIG_RX_ENABLE   ,                                         // 使能 接收中断
    UART_INTERRUPT_CONFIG_ALL_ENABLE  ,                                         // 使能 所有中断
}uart_interrupt_config_enum;

typedef enum                                                                    // 枚举 UART 中断状态 此枚举定义不允许用户修改
{
    UART_INTERRUPT_STATE_NONE   =   0x00    ,

    UART_INTERRUPT_STATE_RX     =   0x01    ,
    UART_INTERRUPT_STATE_TX     =   0x02    ,

    UART_INTERRUPT_STATE_ALL    =   0x03    ,
}uart_interrupt_state_enum;

#define     UART_DEFAULT_TIMEOUT_COUNT      ( 0x001FFFFF )                      // 超时计数不能太短 否则会直接导致通信异常

extern void_callback_uint32_ptr uart_callback_list[UART_NUM];
extern void *uart_callback_ptr_list[UART_NUM];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 字节输出
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     data                需要发送的字节
// 返回参数     void        
// 使用示例     uart_write_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_byte (uart_index_enum uart_index, const uint8 dat);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送数组
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *buff               要发送的数组地址
// 参数说明     len                 发送长度
// 返回参数     void
// 使用示例     uart_write_buffer(uart_index, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_buffer (uart_index_enum uart_index, const uint8 *buff, uint32 len);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送字符串
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *str                要发送的字符串地址
// 返回参数     void
// 使用示例     uart_write_string(uart_index, str); 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_string (uart_index_enum uart_index, const char *str);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（阻塞式 whlie 等待 会超时退出）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               ZF_TRUE - 接收成功   ZF_FALSE - 未接收到数据
// 使用示例     uart_read_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_read_byte (uart_index_enum uart_index, uint8 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（查询接收）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               ZF_TRUE - 接收成功   ZF_FALSE - 未接收到数据
// 使用示例     uart_query_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query_byte (uart_index_enum uart_index, uint8 *data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断回调函数
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void        
// 使用示例     uart_set_callback(uart_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_set_callback (uart_index_enum uart_index, void_callback_uint32_ptr callback, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     config              中断配置        (详见 zf_driver_uart.h 内 uart_interrupt_config_enum 定义)
// 返回参数     void                
// 使用示例     uart_set_interrupt_config(uart_index, config);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_set_interrupt_config (uart_index_enum uart_index, uart_interrupt_config_enum config);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 初始化
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     baudrate            UART 波特率
// 参数说明     tx_pin              UART 发送引脚   (详见 zf_driver_uart.h 内 uart_tx_pin_enum 定义)
// 参数说明     rx_pin              UART 接收引脚   (详见 zf_driver_uart.h 内 uart_rx_pin_enum 定义)
// 返回参数     void                
// 使用示例     uart_init(uart_index, baudrate, tx_pin, rx_pin);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_init (uart_index_enum uart_index, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);

#endif
