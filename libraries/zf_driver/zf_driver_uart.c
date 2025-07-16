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

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_uart.h"

static  UART_Regs                   *uart_list[UART_NUM]                = {UART0, UART1, UART2, UART3};
static  IRQn_Type                   uart_irq[UART_NUM]                  = {UART0_INT_IRQn, UART1_INT_IRQn, UART2_INT_IRQn, UART3_INT_IRQn};
static  uint8                       uart_interrupt_state[UART_NUM]      = {0, 0, 0, 0};

static  void uart_callbakc_defalut (uint32 event, void *ptr);
        void                        *uart_callback_ptr_list[UART_NUM]   = {NULL, NULL, NULL, NULL};
        void_callback_uint32_ptr    uart_callback_list[UART_NUM]        = {uart_callbakc_defalut, uart_callbakc_defalut, uart_callbakc_defalut, uart_callbakc_defalut};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void uart_callbakc_defalut (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 字节输出
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     data                需要发送的字节
// 返回参数     void        
// 使用示例     uart_write_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_byte (uart_index_enum uart_index, const uint8 dat)
{
	while(true == DL_UART_isBusy(uart_list[uart_index]));
	DL_UART_Main_transmitData(uart_list[uart_index], dat);                      // 写入发送数据
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送数组
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *buff               要发送的数组地址
// 参数说明     len                 发送长度
// 返回参数     void
// 使用示例     uart_write_buffer(uart_index, buff, len);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_buffer (uart_index_enum uart_index, const uint8 *buff, uint32 len)
{
    zf_assert(NULL != buff);
    while(len)                                                                  // 循环到发送完
    {
        while(true == DL_UART_isBusy(uart_list[uart_index]));
        DL_UART_Main_transmitData(uart_list[uart_index], *buff ++);             // 写入发送数据
        len --;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 发送字符串
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *str                要发送的字符串地址
// 返回参数     void
// 使用示例     uart_write_string(uart_index, str); 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_write_string (uart_index_enum uart_index, const char *str)
{
    zf_assert(NULL != str);
    while(*str)                                                                 // 一直循环到结尾
    {
        while(true == DL_UART_isBusy(uart_list[uart_index]));
        DL_UART_Main_transmitData(uart_list[uart_index], *str ++);              // 写入发送数据
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（阻塞式 whlie 等待 会超时退出）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               ZF_TRUE - 接收成功   ZF_FALSE - 未接收到数据
// 使用示例     uart_read_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_read_byte (uart_index_enum uart_index, uint8 *data)
{
    zf_assert(NULL != data);

    uint8   return_state    = ZF_FALSE;
    vuint32 timeout_count   = UART_DEFAULT_TIMEOUT_COUNT;

    while(timeout_count)
    {
        if(false == DL_UART_isRXFIFOEmpty(uart_list[uart_index]))
        {
            break;
        }
        timeout_count --;
    }
    if(timeout_count)
    {
        *data = DL_UART_Main_receiveData(uart_list[uart_index]);
        return_state = ZF_TRUE;
    }

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 读取接收的数据（查询接收）
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     *data               数据存放缓冲区
// 返回参数     uint8               ZF_TRUE - 接收成功   ZF_FALSE - 未接收到数据
// 使用示例     uart_query_byte(uart_index, data);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query_byte (uart_index_enum uart_index, uint8 *data)
{
    zf_assert(NULL != data);

    uint8   return_state    = ZF_FALSE;

    if(false == DL_UART_isRXFIFOEmpty(uart_list[uart_index]))                   // 读取到一个数据
    {
        *data = DL_UART_Main_receiveData(uart_list[uart_index]);                // 存储一个数据
        return_state = ZF_TRUE;
    }

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断回调函数
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void        
// 使用示例     uart_set_callback(uart_index, callback, ptr);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_set_callback (uart_index_enum uart_index, void_callback_uint32_ptr callback, void *ptr)
{
    uart_callback_ptr_list[uart_index]  = ptr;
    uart_callback_list[uart_index]      = (NULL == callback) ? (uart_callbakc_defalut) : (callback);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART 中断设置
// 参数说明     uart_index          UART 模块号     (详见 zf_driver_uart.h 内 uart_index_enum 定义)
// 参数说明     config              中断配置        (详见 zf_driver_uart.h 内 uart_interrupt_config_enum 定义)
// 返回参数     void                
// 使用示例     uart_set_interrupt_config(uart_index, config);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void uart_set_interrupt_config (uart_index_enum uart_index, uart_interrupt_config_enum config)
{
    switch(config)
    {
        case UART_INTERRUPT_CONFIG_TX_DISABLE:
        {
            DL_UART_Main_disableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_TX);
            uart_interrupt_state[uart_index] &= 0xEF;
            if(0x00 == uart_interrupt_state[uart_index])
            {
                interrupt_disable(uart_irq[uart_index]);
            }
        }break;
        case UART_INTERRUPT_CONFIG_RX_DISABLE:
        {
            DL_UART_Main_disableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_RX);
            uart_interrupt_state[uart_index] &= 0xFE;
            if(0x00 == uart_interrupt_state[uart_index])
            {
                interrupt_disable(uart_irq[uart_index]);
            }
        }break;
        case UART_INTERRUPT_CONFIG_ALL_DISABLE:
        {
            DL_UART_Main_disableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_TX);
            DL_UART_Main_disableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_RX);
            interrupt_disable(uart_irq[uart_index]);
        }break;
        case UART_INTERRUPT_CONFIG_TX_ENABLE:
        {
            DL_UART_Main_enableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_TX);
            uart_interrupt_state[uart_index] |= 0x10;
            interrupt_enable(uart_irq[uart_index]);
        }break;
        case UART_INTERRUPT_CONFIG_RX_ENABLE:
        {
            DL_UART_Main_enableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_RX);
            uart_interrupt_state[uart_index] |= 0x01;
            interrupt_enable(uart_irq[uart_index]);
        }break;
        case UART_INTERRUPT_CONFIG_ALL_ENABLE:
        {
            DL_UART_Main_enableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_TX);
            DL_UART_Main_enableInterrupt(uart_list[uart_index], DL_UART_MAIN_INTERRUPT_RX);
            interrupt_enable(uart_irq[uart_index]);
        }break;
    }
}

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
void uart_init (uart_index_enum uart_index, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin)
{
    static const DL_UART_Main_ClockConfig uart_clock_config =
    {
        .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
        .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
    };

    static const DL_UART_Main_Config uart_default_config =
    {
        .mode        = DL_UART_MAIN_MODE_NORMAL,
        .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
        .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
        .parity      = DL_UART_MAIN_PARITY_NONE,
        .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
        .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
    };

    afio_init
    (
        (gpio_pin_enum)(tx_pin & UART_PIN_INDEX_MASK),
        GPO,
        (gpio_af_enum)((tx_pin >> UART_PIN_AF_OFFSET) & UART_PIN_AF_MASK),
        GPO_AF_PUSH_PULL
    );
    afio_init
    (
        (gpio_pin_enum)(rx_pin & UART_PIN_INDEX_MASK),
        GPI,
        (gpio_af_enum)((rx_pin >> UART_PIN_AF_OFFSET) & UART_PIN_AF_MASK),
        GPI_PULL_UP
    );

    switch(uart_index)
    {
        case UART_0:    DL_UART_Main_setClockConfig(uart_list[uart_index], (DL_UART_Main_ClockConfig *) &uart_clock_config);   break;
        case UART_1:    DL_UART_Main_setClockConfig(uart_list[uart_index], (DL_UART_Main_ClockConfig *) &uart_clock_config);   break;
        case UART_2:    DL_UART_Main_setClockConfig(uart_list[uart_index], (DL_UART_Main_ClockConfig *) &uart_clock_config);   break;
        case UART_3:    DL_UART_Main_setClockConfig(uart_list[uart_index], (DL_UART_Main_ClockConfig *) &uart_clock_config);   break;
        default:        break;
    }

    DL_UART_Main_init(uart_list[uart_index], (DL_UART_Main_Config *) &uart_default_config);
    DL_UART_Main_setOversampling(uart_list[uart_index], DL_UART_OVERSAMPLING_RATE_8X);

    double buadrate_double = 40000000.0 / (double)baud / 8.0;
    uint16 buadrate_integer = (uint16)buadrate_double;
    uint16 buadrate_fractional = (uint16)((buadrate_double - buadrate_integer) * 64.0 + 0.5);
    DL_UART_Main_setBaudRateDivisor(uart_list[uart_index], buadrate_integer, buadrate_fractional);

    DL_UART_Main_enable(uart_list[uart_index]);
}
