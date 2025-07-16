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

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"

#include "zf_driver_pwm.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 更新占空比
// 参数说明     pin             选择 PWM 引脚
// 参数说明     duty            设置占空比
// 返回参数     void
// 使用示例     pwm_set_duty(TIM5_PWMCH1_A0, PWM_DUTY_MAX / 4);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_duty (pwm_channel_enum pin, const uint32 duty)
{
    // 如果是这一行报错 那你得去看看最大占空比是限定的多少 占空比写入错误
    zf_assert(PWM_DUTY_MAX >= duty);                                            // 占空比写入错误

    GPTIMER_Regs *timer_obj;
    switch((pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK)
    {
        case PWM_TIM_A0 :   timer_obj = TIMA0 ; break;
        case PWM_TIM_A1 :   timer_obj = TIMA1 ; break;
        case PWM_TIM_G0 :   timer_obj = TIMG0 ; break;
        case PWM_TIM_G6 :   timer_obj = TIMG6 ; break;
        case PWM_TIM_G7 :   timer_obj = TIMG7 ; break;
        case PWM_TIM_G8 :   timer_obj = TIMG8 ; break;
        case PWM_TIM_G12:   timer_obj = TIMG12; break;
        default:    break;
    }

    uint16  period_temp = timer_obj->COUNTERREGS.LOAD;                          // 获取自动重装载值
    uint16  match_temp = (uint16)(period_temp * ((float)duty / PWM_DUTY_MAX));  // 计算占空比
    uint32  channel_index = (pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK;

    if(0 == duty)
    {
        timer_obj->COMMONREGS.ODIS |= (GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW << channel_index);
    }
    if(2 > channel_index)
    {
        timer_obj->COUNTERREGS.CC_01[channel_index % 2] = 
            (match_temp == period_temp) ? period_temp + 1 : match_temp;
    }
    else
    {
        timer_obj->COUNTERREGS.CC_23[channel_index % 2] =
            (match_temp == period_temp) ? period_temp + 1 : match_temp;
    }
    if(duty)
    {
        timer_obj->COMMONREGS.ODIS &= ~(GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW << channel_index);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 初始化
// 参数说明     pin             选择 PWM 引脚
// 参数说明     freq            设置频率 同个模块只有最后一次设置生效
// 参数说明     duty            设置占空比
// 返回参数     void
// 使用示例     pwm_init(TIM5_PWMCH1_A0, 17000, PWM_DUTY_MAX / 2);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_init (pwm_channel_enum pin, const uint32 freq, const uint32 duty)
{
    uint8 tim_index = (pin >> PWM_INDEX_OFFSET) & PWM_INDEX_MASK;
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 就去查看你在什么地方调用这个函数 检查你的传入参数
    // 这里是检查是否有重复使用定时器
    // 比如初始化了 TIM1_PIT 然后又初始化成 TIM1_PWM 这种用法是不允许的
    zf_assert(timer_funciton_check((timer_index_enum)(tim_index), TIMER_FUNCTION_PWM));
    // 如果是这一行报错 那你得去看看最大占空比是限定的多少 占空比写入错误
    zf_assert(PWM_DUTY_MAX >= duty);

    timer_clock_enable((timer_index_enum)(tim_index));

    afio_init((gpio_pin_enum)(pin & PWM_PIN_INDEX_MASK), GPO, (gpio_af_enum)((pin >> PWM_PIN_AF_OFFSET) & PWM_PIN_AF_MASK), GPO_AF_PUSH_PULL);  // 初始化引脚

    GPTIMER_Regs *timer_obj;
    switch(tim_index)
    {
        case PWM_TIM_A0 :   timer_obj = TIMA0 ; break;
        case PWM_TIM_A1 :   timer_obj = TIMA1 ; break;
        case PWM_TIM_G0 :   timer_obj = TIMG0 ; break;
        case PWM_TIM_G6 :   timer_obj = TIMG6 ; break;
        case PWM_TIM_G7 :   timer_obj = TIMG7 ; break;
        case PWM_TIM_G8 :   timer_obj = TIMG8 ; break;
        case PWM_TIM_G12:   timer_obj = TIMG12; break;
        default:    break;
    }

    timer_obj->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_UP | GPTIMER_CTRCTL_REPEAT_REPEAT_1;

    uint32  clock_selcet = 0;
    uint8   div_temp = 1;
    uint16  psc_temp = 1;
    uint32  clock_temp = 0;
    if(PWM_TIM_G0 == tim_index || PWM_TIM_G8 == tim_index)
    {
        clock_selcet = 40000000;
    }
    else
    {
        clock_selcet = 80000000;
    }
    for(;;)
    {
        clock_temp = clock_selcet / freq / div_temp / psc_temp;
        if(0xFFF0 < clock_temp)
        {
            psc_temp ++;
            if(256 == psc_temp)
            {
                psc_temp = 1;
                div_temp += 1;
            }
            if(9 == div_temp)
            {
                clock_selcet = 4000000;
                psc_temp = 1;
                div_temp = 1;
            }
        }
        else
        {
            break;
        }
    }

    timer_obj->CLKSEL = (4000000 == clock_selcet) ? (GPTIMER_CLKSEL_MFCLK_SEL_ENABLE) : (GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE);
    timer_obj->CLKDIV = div_temp - 1;
    timer_obj->COMMONREGS.CPS = psc_temp - 1;
    timer_obj->COUNTERREGS.LOAD = clock_selcet / freq / div_temp / psc_temp - 1;

    uint32  channel_index = (pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK;
    timer_obj->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT << ((pin >> PWM_CHANNEL_OFFSET) & PWM_CHANNEL_MASK);
    if(2 > channel_index)
    {
        timer_obj->COUNTERREGS.CCCTL_01[channel_index % 2] = (channel_index << GPTIMER_CCCTL_01_CC2SELU_OFS) | (channel_index << GPTIMER_CCCTL_01_CC2SELD_OFS);
        timer_obj->COUNTERREGS.CCACT_01[channel_index % 2] = GPTIMER_CCACT_01_ZACT_CCP_HIGH | GPTIMER_CCACT_01_CUACT_CCP_LOW;
    }
    else
    {
        timer_obj->COUNTERREGS.CCCTL_01[channel_index % 2] = (channel_index << GPTIMER_CCCTL_23_CC2SELU_OFS) | (channel_index << GPTIMER_CCCTL_23_CC2SELD_OFS);
        timer_obj->COUNTERREGS.CCACT_23[channel_index % 2] = GPTIMER_CCACT_23_ZACT_CCP_HIGH | GPTIMER_CCACT_23_CUACT_CCP_LOW;
    }

    pwm_set_duty(pin, duty);
    timer_obj->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}
