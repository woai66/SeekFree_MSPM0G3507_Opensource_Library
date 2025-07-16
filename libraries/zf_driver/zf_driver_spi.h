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

#ifndef _zf_driver_spi_h_
#define _zf_driver_spi_h_

#include "ti_msp_dl_config.h"

#include "zf_common_typedef.h"

#include "zf_driver_gpio.h"

//#define SPI_SPEED_PRIORITY                                                      // SPI 通信速度优先

#define SPI_NUM             ( 2 )

typedef enum                                                                    // 枚举 SPI 编号 此枚举定义不允许用户修改
{
    SPI_0,
    SPI_1,
}spi_index_enum;

typedef enum                                                                    // 枚举 SPI 模式 此枚举定义不允许用户修改
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_enum;

// 定义引脚要用的宏定义 根据各单片机不同 可以自由定义需要什么信息
// bit[11:0 ] 固定为引脚索引
// bit[15:12] 固定为引脚复用
// bit[19:16] 在 SPI 模块中为 SPI 索引

#define     SPI_PIN_INDEX_OFFSET    ( 0      )                                  // bit[11:0 ] 存储 GPIO 的索引号
#define     SPI_PIN_INDEX_MASK      ( 0x0FFF )                                  // 宽度 12bit 因此掩码为 0x0FFF

#define     SPI_PIN_AF_OFFSET       ( 12     )                                  // bit[15:12] 存储 GPIO 的复用功能索引
#define     SPI_PIN_AF_MASK         ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

#define     SPI_INDEX_OFFSET        ( 16     )                                  // bit[19:16] 存储 SPI 索引
#define     SPI_INDEX_MASK          ( 0x0F   )                                  // 宽度 4bit 因此掩码为 0x0F

typedef enum                                                                    // 枚举 SPI SCK 引脚 此枚举定义不允许用户修改
{
    // SPI0 引脚 SCK
    SPI0_SCK_A6         = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A6 )),
    SPI0_SCK_A11        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A11)),
    SPI0_SCK_A12        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A12)),
    SPI0_SCK_B18        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B18)),

    // SPI1 引脚 SCK
    SPI1_SCK_A17        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A17)),
    SPI1_SCK_B9         = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B9 )),
    SPI1_SCK_B16        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B16)),
    SPI1_SCK_B23        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF2  << SPI_PIN_AF_OFFSET) | (B23)),
}spi_sck_pin_enum;

typedef enum                                                                    // 枚举 SPI MOSI 引脚 此枚举定义不允许用户修改
{
    // SPI1 引脚 MOSI
    SPI0_MOSI_A5        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A5 )),
    SPI0_MOSI_A9        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A9 )),
    SPI0_MOSI_A14       = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A14)),
    SPI0_MOSI_B17       = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B17)),

    // SPI2 引脚 MOSI
    SPI1_MOSI_A18       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A18)),
    SPI1_MOSI_B8        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B8 )),
    SPI1_MOSI_B15       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B15)),
    SPI1_MOSI_B22       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF2  << SPI_PIN_AF_OFFSET) | (B22)),
}spi_mosi_pin_enum;

typedef enum                                                                    // 枚举 SPI MISO 引脚 此枚举定义不允许用户修改
{
    SPI_MISO_NULL       = 0xFFFFF,                                              // 不使用硬件 MISO 引脚就填这个

    // SPI1 引脚 MISO
    SPI0_MISO_A4        = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A4 )),
    SPI0_MISO_A10       = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A10)),
    SPI0_MISO_A13       = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A13)),
    SPI0_MISO_B19       = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B19)),

    // SPI2 引脚 MISO
    SPI1_MISO_A16       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A16)),
    SPI1_MISO_B7        = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B7 )),
    SPI1_MISO_B14       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B14)),
    SPI1_MISO_B21       = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF2  << SPI_PIN_AF_OFFSET) | (B21)),
}spi_miso_pin_enum;

typedef enum                                                                    // 枚举 SPI CS 引脚 此枚举定义不允许用户修改
{
    SPI_CS_NULL         = 0xFFFFF,                                              // 不使用硬件 CS 引脚就填这个

    // SPI1 引脚 CS
    SPI0_CS_A2          = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A2 )),
    SPI0_CS_A8          = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A8 )),
    SPI0_CS_B25         = ((SPI_0 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B25)),

    // SPI2 引脚 CS
    SPI1_CS_A2          = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF5  << SPI_PIN_AF_OFFSET) | (A2 )),
    SPI1_CS_A26         = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (A26)),
    SPI1_CS_B6          = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B6 )),
    SPI1_CS_B20         = ((SPI_1 << SPI_INDEX_OFFSET) | (GPIO_AF3  << SPI_PIN_AF_OFFSET) | (B20)),
}spi_cs_pin_enum;

void        spi_write_8bit                  (spi_index_enum spi_index, const uint8 data);
void        spi_write_8bit_array            (spi_index_enum spi_index, const uint8 *data, uint32 len);

void        spi_write_16bit                 (spi_index_enum spi_index, const uint16 data);
void        spi_write_16bit_array           (spi_index_enum spi_index, const uint16 *data, uint32 len);

void        spi_write_8bit_register         (spi_index_enum spi_index, const uint8 register_name, const uint8 data);
void        spi_write_8bit_registers        (spi_index_enum spi_index, const uint8 register_name, const uint8 *data, uint32 len);

void        spi_write_16bit_register        (spi_index_enum spi_index, const uint16 register_name, const uint16 data);
void        spi_write_16bit_registers       (spi_index_enum spi_index, const uint16 register_name, const uint16 *data, uint32 len);

uint8       spi_read_8bit                   (spi_index_enum spi_index);
void        spi_read_8bit_array             (spi_index_enum spi_index, uint8 *data, uint32 len);

uint16      spi_read_16bit                  (spi_index_enum spi_index);
void        spi_read_16bit_array            (spi_index_enum spi_index, uint16 *data, uint32 len);

uint8       spi_read_8bit_register          (spi_index_enum spi_index, const uint8 register_name);
void        spi_read_8bit_registers         (spi_index_enum spi_index, const uint8 register_name, uint8 *data, uint32 len);

uint16      spi_read_16bit_register         (spi_index_enum spi_index, const uint16 register_name);
void        spi_read_16bit_registers        (spi_index_enum spi_index, const uint16 register_name, uint16 *data, uint32 len);

void        spi_transfer_8bit               (spi_index_enum spi_index, const uint8 *write_buffer, uint8 *read_buffer, uint32 len);
void        spi_transfer_16bit              (spi_index_enum spi_index, const uint16 *write_buffer, uint16 *read_buffer, uint32 len);

void        spi_init                        (spi_index_enum spi_index, spi_mode_enum mode, uint32 baud, spi_sck_pin_enum sck_pin, spi_mosi_pin_enum mosi_pin, spi_miso_pin_enum miso_pin, spi_cs_pin_enum cs_pin);

#endif
