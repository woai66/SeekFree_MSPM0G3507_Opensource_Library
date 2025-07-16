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
* 文件名称          zf_driver_flash
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

#include "zf_driver_flash.h"

flash_data_union flash_union_buffer[FLASH_DATA_BUFFER_SIZE];                    // FLASHCTL 操作的数据缓冲区

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     校验 FLASHCTL 是否有数据
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 <0>
// 返回参数     uint8           1-有数据 0-没有数据 如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
// 使用示例     flash_check(0, 0);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check (uint32 sector_num, uint32 page_num)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    uint8 return_state = 0;
    uint16 temp_loop = 0;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址

    for(temp_loop = 0; FLASH_PAGE_SIZE > temp_loop; temp_loop += 8)             // 循环读取 Flash 的值
    {
        if(0xffffffff != (*(__IO uint32*) (flash_addr + temp_loop)))            // 如果不是 0xffffffff 那就是有值
        {
            return_state = 1;
            break;
        }
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     擦除扇区
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 <0>
// 返回参数     uint8           1-表示失败 0-表示成功
// 使用示例     flash_erase_page(0, 0);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_page (uint32 sector_num, uint32 page_num)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    volatile uint32 time_out = FLASH_OPERATION_TIME_OUT;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址

    uint32 primask = interrupt_global_disable();

    DL_FlashCTL_unprotectSector(
        FLASHCTL, flash_addr, DL_FLASHCTL_REGION_SELECT_MAIN);


    FLASHCTL->GEN.CMDTYPE =
         DL_FLASHCTL_COMMAND_SIZE_SECTOR | DL_FLASHCTL_COMMAND_TYPE_ERASE;

	FLASHCTL->GEN.CMDADDR = flash_addr;

    FLASHCTL->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;

    do
    {
        time_out--;
    }while((DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS == FLASHCTL->GEN.STATCMD) && (0x00 != time_out));                    // 等待超时或者完成


    if(DL_FLASHCTL_COMMAND_STATUS_PASSED & FLASHCTL->GEN.STATCMD)                                                  // 如果不是操作完成 例如超时或出错
    {
        time_out = 0;
    }

	DL_FlashCTL_executeClearStatus(FLASHCTL);
    interrupt_global_enable(primask);

    return ((!time_out) ? 1 : 0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取一页
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 <0>
// 参数说明     buf             需要读取的数据地址   传入的数组类型必须为uint32
// 参数说明     len             需要写入的数据长度   参数范围 1-256
// 返回参数     void
// 使用示例     flash_read_page(0, 0, data_buffer, 256);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page (uint32 sector_num, uint32 page_num, uint32 *buf, uint16 len)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    zf_assert(FLASH_DATA_BUFFER_SIZE >= len);                                   // 参数范围 1-256
    zf_assert(NULL != buf);
    uint16 temp_loop = 0;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < len; temp_loop ++)                           // 根据指定长度读取
    {
        *buf ++ = *(__IO uint32*)(flash_addr + temp_loop * 8);                  // 循环读取 Flash 的值
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     编程一页
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 <>
// 参数说明     buf             需要写入的数据地址   传入的数组类型必须为 uint32
// 参数说明     len             需要写入的数据长度   参数范围 1-256
// 返回参数     uint8           1-表示失败 0-表示成功
// 使用示例     flash_write_page(0, 0, data_buffer, 256);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_write_page (uint32 sector_num, uint32 page_num, const uint32 *buf, uint16 len)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    zf_assert(FLASH_DATA_BUFFER_SIZE >= len);                                   // 参数范围 1-256
    zf_assert(NULL != buf);
    uint8 return_state = 0;
    volatile uint32 time_out = FLASH_OPERATION_TIME_OUT;
    uint32 *data_pointer = (uint32 *)buf;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址

    if(flash_check(sector_num, page_num))                                       // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
    {
        flash_erase_page(sector_num, page_num);                                 // 擦除这一页
    }

    uint32 primask = interrupt_global_disable();
		

//    len *= 2;
    while(len --)                                                               // 根据长度
    {
		
				DL_FlashCTL_unprotectSector(
				FLASHCTL, (FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num), DL_FLASHCTL_REGION_SELECT_MAIN);
				FLASHCTL->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
							DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

				FLASHCTL->GEN.CMDBYTEN = DL_FLASHCTL_PROGRAM_32_WITH_ECC;
					
				FLASHCTL->GEN.CMDADDR = flash_addr;
				
				FLASHCTL->GEN.CMDDATA0 = (*data_pointer ++);

				FLASHCTL->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
					
				time_out = FLASH_OPERATION_TIME_OUT;		
				do
				{
						time_out --;
					
				}while((DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS == FLASHCTL->GEN.STATCMD) && (0x00 != time_out));                // 等待超时或者完成
				
				
				if((!(DL_FLASHCTL_COMMAND_STATUS_PASSED & FLASHCTL->GEN.STATCMD)) || (0 == time_out))                         // 如果不是操作完成 例如超时或出错
				{
						return_state = 1;
						break;
				}
						
				DL_FlashCTL_executeClearStatus(FLASHCTL);
						
						flash_addr += 8;
		}
		
			
    interrupt_global_enable(primask);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从指定 FLASHCTL 的扇区的指定页码读取数据到缓冲区
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 < 0 >
// 返回参数     void
// 使用示例     flash_read_page_to_buffer(0, 0);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page_to_buffer (uint32 sector_num, uint32 page_num)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    uint16 temp_loop = 0;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址

    for(temp_loop = 0; FLASH_DATA_BUFFER_SIZE > temp_loop; temp_loop ++)                            // 根据指定长度读取
    {
        flash_union_buffer[temp_loop].uint32_type = *(__IO uint32*)(flash_addr + temp_loop * 8);    // 循环读取 Flash 的值
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向指定 FLASHCTL 的扇区的指定页码写入缓冲区的数据
// 参数说明     sector_num      需要写入的扇区编号 参数范围 <0 - 3>
// 参数说明     page_num        当前扇区页的编号   参数范围 <0>
// 返回参数     uint8           1-表示失败 0-表示成功
// 使用示例     flash_write_page_from_buffer(0, 0);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_write_page_from_buffer (uint32 sector_num, uint32 page_num)
{
    zf_assert(FLASH_MAX_SECTION_INDEX > sector_num);                            // 参数范围 0-63
    zf_assert(FLASH_MAX_PAGE_INDEX > page_num);                                 // 参数范围 0-3
    uint8 return_state = 0;
    volatile uint32 time_out = FLASH_OPERATION_TIME_OUT;
    uint32 flash_addr = ((FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num)); // 提取当前 Flash 地址
		uint32 *data_pointer1 = (uint32 *)flash_union_buffer;

    uint16 data_len = FLASH_DATA_BUFFER_SIZE *2;
	

    if(flash_check(sector_num, page_num))                                       // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
    {
        flash_erase_page(sector_num, page_num);                                 // 擦除这一页
    }

    uint32 primask = interrupt_global_disable();
		

		
    while(data_len --)                                                          // 根据长度
    {

				DL_FlashCTL_unprotectSector(
				FLASHCTL,(FLASH_BASE_ADDR + FLASH_SECTION_SIZE * sector_num + FLASH_PAGE_SIZE * page_num) , DL_FLASHCTL_REGION_SELECT_MAIN);

				FLASHCTL->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
				DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

				FLASHCTL->GEN.CMDBYTEN = DL_FLASHCTL_PROGRAM_32_WITH_ECC;
					
				FLASHCTL->GEN.CMDADDR = flash_addr;
				
				FLASHCTL->GEN.CMDDATA0 = (*data_pointer1 ++);

				FLASHCTL->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
			
        time_out = FLASH_OPERATION_TIME_OUT;
        do
        {
            time_out --;
        }
				while((DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS == FLASHCTL->GEN.STATCMD) && (0x00 != time_out));                // 等待超时或者完成
				
        if((!(DL_FLASHCTL_COMMAND_STATUS_PASSED & FLASHCTL->GEN.STATCMD)) || (0 == time_out))                      // 如果不是操作完成 例如超时或出错
        {
            return_state = 1;
            break;
        }
				
				DL_FlashCTL_executeClearStatus(FLASHCTL);
        flash_addr += 8;
	
    }
			
    interrupt_global_enable(primask);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清空数据缓冲区
// 参数说明     void
// 返回参数     void
// 使用示例     flash_buffer_clear();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void flash_buffer_clear (void)
{
    memset(flash_union_buffer, 0xFF, FLASH_PAGE_SIZE);
}
