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
* 文件名称          zf_driver_spi
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

#include "zf_driver_spi.h"

static SPI_Regs *spi_list[SPI_NUM] = {SPI0, SPI1};

#define SPI_CTRL_CS(x, e)           (void)x,(void)e                             // SPI 片选 ENABLE/DISABLE 控制片选 拉低/拉高
#define SPI_CTRL_RX(x, e)           (void)x,(void)e                             // SPI RX 使能 ENABLE/DISABLE

#define SPI_DATA_RX(x)              (spi_list[x]->RXDATA)                       // SPI 读数据
#define SPI_DATA_TX(x, data)        (spi_list[x]->TXDATA = (data))              // SPI 写数据

#define SPI_STATE_BUY(x)            (spi_list[x]->STAT & SPI_STAT_BUSY_ACTIVE)  // SPI 忙 正在传输中
#define SPI_STATE_TX_FIFO_FULL(x)   (spi_list[x]->STAT & SPI_STAT_TNF_NOT_FULL) // SPI 发送缓冲器满了
#define SPI_STATE_RX_AVAILABLE(x)   (!(spi_list[x]->STAT & SPI_STAT_RFE_EMPTY)) // SPI 接收缓冲器收到一个有效数据
#define SPI_STATE_TX_EMPTY(x)       (spi_list[x]->STAT & SPI_STAT_TFE_EMPTY)    // SPI 发送缓冲器空了

#ifndef SPI_SPEED_PRIORITY
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 SPI 8bit 数据读写
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     data            数据
// 返回参数     uint8           读取的数据
// 使用示例     spi_8bit_data_handler(spi_index, 5);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 spi_8bit_data_handler (spi_index_enum spi_index, const uint8 data)
{
    uint8 read_data = 0;

    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, data);                                                   // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    read_data = SPI_DATA_RX(spi_index);                                             // 读取数据
		while(SPI_STATE_BUY(spi_index) ){};                                                // 等待传输完毕

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);

    return read_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 SPI 16bit 数据读写
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     data            数据
// 返回参数     uint16          读取的数据
// 使用示例     spi_16bit_data_handler(spi_index, 5);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint16 spi_16bit_data_handler (spi_index_enum spi_index, const uint16 data)
{
    uint16 read_data = 0;
 
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((data & 0xFF00) >> 8));                          // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    read_data = SPI_DATA_RX(spi_index);                                             // 读取数据

    SPI_DATA_TX(spi_index, (uint8)(data & 0x00FF));                                 // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    read_data = ((read_data << 8) | SPI_DATA_RX(spi_index));                        // 读取数据
		while(SPI_STATE_BUY(spi_index)){};                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);

    return read_data;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口写 8bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     data            数据
// 返回参数     void
// 使用示例     spi_write_8bit(SPI_1, 0x11);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit (spi_index_enum spi_index, const uint8 data)
{
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, data);                                                   // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_8bit_data_handler(spi_index, data);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口写 8bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_write_8bit_array(SPI_1, data, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_array (spi_index_enum spi_index, const uint8 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)
    {
        SPI_DATA_TX(spi_index, *data ++);                                           // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        spi_8bit_data_handler(spi_index, *data ++);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口写 16bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     data            数据
// 返回参数     void
// 使用示例     spi_write_16bit(SPI_1, 0x1101);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit (spi_index_enum spi_index, const uint16 data)
{
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((data & 0xFF00) >> 8));                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(data & 0x00FF));                                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_16bit_data_handler(spi_index, data);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口写 16bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_write_16bit_array(SPI_1, data, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_array (spi_index_enum spi_index, const uint16 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)
    {
        SPI_DATA_TX(spi_index, (uint8)((*data & 0xFF00) >> 8));                     // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空

        SPI_DATA_TX(spi_index, (uint8)(*data ++ & 0x00FF));                         // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        spi_16bit_data_handler(spi_index, *data ++);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口向传感器的寄存器写 8bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     data            数据
// 返回参数     void
// 使用示例     spi_write_8bit_register(SPI_1, 0x11, 0x01);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_register (spi_index_enum spi_index, const uint8 register_name, const uint8 data)
{
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, register_name);                                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, data);                                                   // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_8bit_data_handler(spi_index, register_name);
    spi_8bit_data_handler(spi_index, data);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口向传感器的寄存器写 8bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_write_8bit_registers(SPI_1, 0x11, data, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_registers (spi_index_enum spi_index, const uint8 register_name, const uint8 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, register_name);                                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(len --)
    {
        SPI_DATA_TX(spi_index, *data ++);                                           // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_8bit_data_handler(spi_index, register_name);
    while(len --)
    {
        spi_8bit_data_handler(spi_index, *data ++);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口向传感器的寄存器写 16bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     data            数据
// 返回参数     void
// 使用示例     spi_write_16bit_register(SPI_1, 0x1011, 0x0101);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_register (spi_index_enum spi_index, const uint16 register_name, const uint16 data)
{
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((register_name & 0xFF00) >> 8));                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(register_name & 0x00FF));                        // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)((data & 0xFF00) >> 8));                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(data & 0x00FF));                                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_16bit_data_handler(spi_index, register_name);
    spi_16bit_data_handler(spi_index, data);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口向传感器的寄存器写 16bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_write_16bit_registers(SPI_1, 0x1011, data, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_registers (spi_index_enum spi_index, const uint16 register_name, const uint16 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((register_name & 0xFF00) >> 8));                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(register_name & 0x00FF));                        // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    while(len --)
    {
        SPI_DATA_TX(spi_index, (uint8)((*data & 0xFF00) >> 8));                     // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空

        SPI_DATA_TX(spi_index, (uint8)(*data ++ & 0x00FF));                         // 发送数据
        while(!SPI_STATE_TX_EMPTY(spi_index));                                      // 发送为空
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
#else
    spi_16bit_data_handler(spi_index, register_name);
    while(len--)
    {
        spi_16bit_data_handler(spi_index, *data++);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口读 8bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 返回参数     uint8           数据
// 使用示例     spi_read_8bit(SPI_1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_read_8bit (spi_index_enum spi_index)
{
#ifdef SPI_SPEED_PRIORITY
    uint8 data = 0;

    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    data = SPI_DATA_RX(spi_index);                                                  // 读取数据

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
    return data;
#else
    return spi_8bit_data_handler(spi_index, 0);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口读 8bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             发送缓冲区长度
// 返回参数     void
// 使用示例     spi_read_8bit_array(SPI_1, data, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_array (spi_index_enum spi_index, uint8 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)
    {
        SPI_DATA_TX(spi_index, 0);                                                  // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 接收到有效数据
        *data ++ = SPI_DATA_RX(spi_index);                                          // 读取数据
    }

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        *data ++ = spi_8bit_data_handler(spi_index, 0);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口读 16bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 返回参数     uint16          数据
// 使用示例     spi_read_16bit(SPI_1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint16 spi_read_16bit (spi_index_enum spi_index)
{
#ifdef SPI_SPEED_PRIORITY
    uint16 data = 0;

    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    data = SPI_DATA_RX(spi_index);                                                  // 读取数据

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 接收到有效数据
    data = ((data << 8)| SPI_DATA_RX(spi_index));                                   // 读取数据

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
    return data;
#else
    return spi_16bit_data_handler(spi_index, 0);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口读 16bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             发送缓冲区长度
// 返回参数     void
// 使用示例     spi_read_16bit_array(SPI_1, data, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_array (spi_index_enum spi_index, uint16 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)
    {
        SPI_DATA_TX(spi_index, 0);                                                  // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 接收到有效数据
        *data = SPI_DATA_RX(spi_index);                                             // 读取数据

        SPI_DATA_TX(spi_index, 0);                                                  // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 接收到有效数据
        *data = ((*data << 8)| SPI_DATA_RX(spi_index));                             // 读取数据
        data ++;
    }

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        *data ++ = spi_16bit_data_handler(spi_index, 0);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口从传感器的寄存器读 8bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 返回参数     uint8           数据
// 使用示例     spi_read_8bit_register(SPI_1, 0x11);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_read_8bit_register (spi_index_enum spi_index, const uint8 register_name)
{
#ifdef SPI_SPEED_PRIORITY
    uint8 data = 0;

    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, register_name);                                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_CTRL_RX(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 等待读取一个数据
    data = SPI_DATA_RX(spi_index);                                                  // 读取数据

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
    return data;
#else
    spi_8bit_data_handler(spi_index, register_name);
    return spi_8bit_data_handler(spi_index, 0);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口从传感器的寄存器读 8bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             发送缓冲区长度
// 返回参数     void
// 使用示例     spi_read_8bit_registers(SPI_1, 0x11, data, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_registers (spi_index_enum spi_index, const uint8 register_name, uint8 *data, uint32 len)
{
     uint8  aaa =0 ,aaa1,aaa2,aaa3;
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, register_name);                                          // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空
    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
	
	
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    while(len --)
    {
        SPI_DATA_TX(spi_index, 0);                                                   // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 等待读取一个数据
        *data ++  = SPI_DATA_RX(spi_index);                                              // 读取数据
		
		while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕

		
    }

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    spi_8bit_data_handler(spi_index, register_name);
    while(len --)
    {
        *data ++ = spi_8bit_data_handler(spi_index, 0);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口从传感器的寄存器读 16bit 数据
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 返回参数     uint16          数据
// 使用示例     spi_read_16bit_register(SPI_1, 0x1011);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint16 spi_read_16bit_register (spi_index_enum spi_index, const uint16 register_name)
{
#ifdef SPI_SPEED_PRIORITY
    uint16 data = 0;
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((register_name & 0xFF00) >> 8));                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(register_name & 0x00FF));                        // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_CTRL_RX(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 等待读取一个数据
    data = SPI_DATA_RX(spi_index);                                                  // 读取数据

    SPI_DATA_TX(spi_index, 0);                                                      // 发送数据
    while(!SPI_STATE_RX_AVAILABLE(spi_index));                                      // 等待读取一个数据
    data = ((data << 8)| SPI_DATA_RX(spi_index));                                   // 读取数据

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
    return data;
#else
    spi_16bit_data_handler(spi_index, register_name);
    return spi_16bit_data_handler(spi_index, 0);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口从传感器的寄存器读 16bit 数组
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     register_name   寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             发送缓冲区长度
// 返回参数     void
// 使用示例     spi_read_16bit_registers(SPI_1, 0x1101, data, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_registers (spi_index_enum spi_index, const uint16 register_name, uint16 *data, uint32 len)
{
    zf_assert(NULL != data);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    SPI_DATA_TX(spi_index, (uint8)((register_name & 0xFF00) >> 8));                 // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_DATA_TX(spi_index, (uint8)(register_name & 0x00FF));                        // 发送数据
    while(!SPI_STATE_TX_EMPTY(spi_index));                                          // 发送为空

    SPI_CTRL_RX(spi_index, ZF_ENABLE);
 
    while(len --)
    {
        SPI_DATA_TX(spi_index, 0);                                                  // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 等待读取一个数据
        *data = SPI_DATA_RX(spi_index);                                             // 读取数据

        SPI_DATA_TX(spi_index, 0);                                                  // 发送数据
        while(!SPI_STATE_RX_AVAILABLE(spi_index));                                  // 等待读取一个数据
        *data = ((*data << 8)| SPI_DATA_RX(spi_index));                             // 读取数据
        data ++;
    }

    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    spi_16bit_data_handler(spi_index, register_name);
    while(len --)
    {
        *data ++ = spi_16bit_data_handler(spi_index, 0);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 8bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     write_buffer    发送的数据缓冲区地址
// 参数说明     read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_transfer_8bit(SPI_1, buf, buf, 1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit (spi_index_enum spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len)
{
    zf_assert(NULL != write_buffer);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)                                                               // 判断长度
    {
        SPI_DATA_TX(spi_index, *write_buffer ++);                                   // 发送数据
        if(NULL != read_buffer)                                                 // 接收有效
        {
            while(!SPI_STATE_RX_AVAILABLE(spi_index));                              // 等待读取一个数据
            *read_buffer ++ = SPI_DATA_RX(spi_index);                               // 读取数据
        }
        else
        {
            while(!SPI_STATE_TX_EMPTY(spi_index));                                  // 发送为空
        }
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        if(NULL != read_buffer)
        {
            *read_buffer = spi_8bit_data_handler(spi_index, *write_buffer);
            write_buffer ++;
            read_buffer ++;
        }
        else
        {
            spi_8bit_data_handler(spi_index, *write_buffer);
            write_buffer ++;
        }
    }
}
#endif


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     write_buffer    发送的数据缓冲区地址
// 参数说明     read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     spi_transfer_16bit(SPI_1, buf, buf, 1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit (spi_index_enum spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len)
{
    zf_assert(NULL != write_buffer);
#ifdef SPI_SPEED_PRIORITY
    SPI_CTRL_RX(spi_index, ZF_ENABLE);
    SPI_CTRL_CS(spi_index, ZF_ENABLE);

    while(len --)                                                               // 判断长度
    {
        if(NULL != read_buffer)                                                 // 接收有效
        {
            SPI_DATA_TX(spi_index, (*write_buffer & 0xFF00) >> 8);                  // 发送数据
            while(!SPI_STATE_RX_AVAILABLE(spi_index));                              // 等待读取一个数据
            *read_buffer = (SPI_DATA_RX(spi_index) & 0x00FF);                       // 读取数据

            SPI_DATA_TX(spi_index, (*write_buffer & 0x00FF));                       // 发送数据
            while(!SPI_STATE_RX_AVAILABLE(spi_index));                              // 等待读取一个数据
            *read_buffer = (*read_buffer << 8) | (SPI_DATA_RX(spi_index)& 0x00FF);  // 读取数据

            write_buffer ++;
            read_buffer ++;
        }
        else
        {
            SPI_DATA_TX(spi_index, (*write_buffer & 0xFF00) >> 8);                  // 发送数据
            while(!SPI_STATE_TX_EMPTY(spi_index));                                  // 发送为空

            SPI_DATA_TX(spi_index, (*write_buffer & 0x00FF));                     	// 发送数据
            while(!SPI_STATE_TX_EMPTY(spi_index));                                  // 发送为空

            write_buffer ++;
        }
    }

    while(SPI_STATE_BUY(spi_index));                                                // 等待传输完毕
    SPI_CTRL_CS(spi_index, ZF_DISABLE);
    SPI_CTRL_RX(spi_index, ZF_DISABLE);
#else
    while(len --)
    {
        if(NULL != read_buffer)
        {
            *read_buffer = spi_16bit_data_handler(spi_index, *write_buffer);
            write_buffer ++;
            read_buffer ++;
        }
        else
        {
            spi_16bit_data_handler(spi_index, *write_buffer);
            write_buffer ++;
        }
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口初始化
// 参数说明     spi_index           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// 参数说明     mode            SPI 模式 参照 zf_driver_spi.h 内 spi_mode_enum 枚举体定义
// 参数说明     baud            设置 SPI 的波特率 不超过系统时钟的一半 部分速率会被适配成相近的速率
// 参数说明     sck_pin         选择 SCK 引脚 参照 zf_driver_spi.h 内 spi_sck_pin_enum 枚举体定义
// 参数说明     mosi_pin        选择 MOSI 引脚 参照 zf_driver_spi.h 内 spi_mosi_pin_enum 枚举体定义
// 参数说明     miso_pin        选择 MISO 引脚 参照 zf_driver_spi.h 内 spi_miso_pin_enum 枚举体定义
// 参数说明     cs_pin          选择 CS 引脚 参照 zf_driver_spi.h 内 spi_cs_pin_enum 枚举体定义
// 返回参数     void
// 使用示例     spi_init(SPI_1, 0, 1*1000*1000, SPI1_SCK_A05, SPI1_MOSI_A07, SPI1_MISO_A06, SPI1_NSS_A04);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_init (spi_index_enum spi_index, spi_mode_enum mode, uint32 baud, spi_sck_pin_enum sck_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin)
{
    zf_assert(((sck_pin   >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK) == spi_index);
    zf_assert(((mosi_pin  >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK) == spi_index);
    zf_assert((((miso_pin >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK) == spi_index) || (SPI_MISO_NULL == miso_pin));
    zf_assert((((cs_pin   >> SPI_INDEX_OFFSET) & SPI_INDEX_MASK) == spi_index) || (SPI_CS_NULL == cs_pin));

    afio_init(
        (gpio_pin_enum)(sck_pin  & SPI_PIN_INDEX_MASK),
        GPO,
        (gpio_af_enum)((sck_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK),
        GPO_PUSH_PULL);
    afio_init(
        (gpio_pin_enum)(mosi_pin & SPI_PIN_INDEX_MASK),
        GPO,
        (gpio_af_enum)((mosi_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK),
        GPO_AF_PUSH_PULL);
    if(SPI_MISO_NULL != miso_pin)
    {
        afio_init(
            (gpio_pin_enum)(miso_pin &SPI_PIN_INDEX_MASK),
            GPI,
            (gpio_af_enum)((miso_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK),
            GPI_FLOATING_IN);
    }
    if(SPI_CS_NULL != cs_pin)
    {
        afio_init(
            (gpio_pin_enum)(cs_pin &SPI_PIN_INDEX_MASK),
            GPO,
            (gpio_af_enum)((cs_pin >> SPI_PIN_AF_OFFSET) & SPI_PIN_AF_MASK),
            GPO_AF_PUSH_PULL);
    }

    DL_SPI_Config spi_default_config =
    {
        .mode           = DL_SPI_MODE_CONTROLLER,
        .frameFormat    = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0,
        .parity         = DL_SPI_PARITY_NONE,
        .dataSize       = DL_SPI_DATA_SIZE_8,
        .bitOrder       = DL_SPI_BIT_ORDER_MSB_FIRST,
        .chipSelectPin  = DL_SPI_CHIP_SELECT_0,
    };
    DL_SPI_ClockConfig spi_default_clock_config =
    {
        .clockSel       = DL_SPI_CLOCK_BUSCLK,
        .divideRatio    = DL_SPI_CLOCK_DIVIDE_RATIO_1
    };

    switch(mode)
    {
        case SPI_MODE0: spi_default_config.frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0; break;
        case SPI_MODE1: spi_default_config.frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA1; break;
        case SPI_MODE2: spi_default_config.frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA0; break;
        case SPI_MODE3: spi_default_config.frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA1; break;
    }
    DL_SPI_setClockConfig(spi_list[spi_index], (DL_SPI_ClockConfig *)&spi_default_clock_config);
    DL_SPI_init(spi_list[spi_index], (DL_SPI_Config *)&spi_default_config);

    uint16  div             = 0;
    uint32  clock_selcet    = 80 * 1000 * 1000;
    uint32  baud_large      = clock_selcet / ((div + 1) * 2);
    uint32  baud_temp       = clock_selcet / ((div + 1) * 2);
    for(div = 0; 0x3FF > div; div ++)
    {
        baud_temp = clock_selcet / ((div + 1) * 2);
        if(baud_temp <= baud)
        {
            break;
        }
        baud_large = baud_temp;
    }
    DL_SPI_setBitRateSerialClockDivider(spi_list[spi_index], div);
    DL_SPI_setFIFOThreshold(spi_list[spi_index], DL_SPI_RX_FIFO_LEVEL_1_4_FULL, DL_SPI_TX_FIFO_LEVEL_EMPTY);
    DL_SPI_enable(spi_list[spi_index]);
}
