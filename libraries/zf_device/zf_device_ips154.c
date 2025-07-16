#include "zf_device_ips154.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_spi.h"

static uint16 ips154_pencolor = IPS154_DEFAULT_PENCOLOR;
static uint16 ips154_bgcolor = IPS154_DEFAULT_BGCOLOR;
static ips154_dir_enum ips154_display_dir = IPS154_DEFAULT_DISPLAY_DIR;
static ips154_font_size_enum ips154_display_font = IPS154_DEFAULT_DISPLAY_FONT;
static uint16 ips154_width_max = IPS154_WIDTH_MAX;
static uint16 ips154_height_max = IPS154_HEIGHT_MAX;

#define IPS154_CS(x) ((x) ? (gpio_high(IPS154_CS_PIN)) : (gpio_low(IPS154_CS_PIN)))
#define IPS154_DC(x) ((x) ? (gpio_high(IPS154_DC_PIN)) : (gpio_low(IPS154_DC_PIN)))
#define IPS154_RST(x) ((x) ? (gpio_high(IPS154_RST_PIN)) : (gpio_low(IPS154_RST_PIN)))
#define IPS154_BLK(x) ((x) ? (gpio_high(IPS154_BLK_PIN)) : (gpio_low(IPS154_BLK_PIN)))

#define ips154_write_8bit_data(data) (spi_write_8bit(IPS154_SPI, (data)))
#define ips154_write_16bit_data(data) (spi_write_16bit(IPS154_SPI, (data)))
#define ips154_write_16bit_data_array(data, len) (spi_write_16bit_array(IPS154_SPI, (data), (len)))

static void ips154_write_index(const uint8 dat) {
    IPS154_DC(0);
    ips154_write_8bit_data(dat);
    IPS154_DC(1);
}

static void ips154_set_region(uint16 x1, uint16 y1, uint16 x2, uint16 y2) {
    zf_assert(x1 < ips154_width_max);
    zf_assert(y1 < ips154_height_max);
    zf_assert(x2 < ips154_width_max);
    zf_assert(y2 < ips154_height_max);
    ips154_write_index(0x2a);
    ips154_write_16bit_data(x1);
    ips154_write_16bit_data(x2);
    ips154_write_index(0x2b);
    ips154_write_16bit_data(y1);
    ips154_write_16bit_data(y2);
    ips154_write_index(0x2c);
}

void ips154_clear(void) {
    uint16 color_buffer[IPS154_WIDTH_MAX];
    uint16 i = 0, j = 0;
    IPS154_CS(0);
    ips154_set_region(0, 0, ips154_width_max - 1, ips154_height_max - 1);
    for(i = 0; i < ips154_width_max; i++) color_buffer[i] = ips154_bgcolor;
    for(j = 0; j < ips154_height_max; j++) ips154_write_16bit_data_array(color_buffer, ips154_width_max);
    IPS154_CS(1);
}

void ips154_full(const uint16 color) {
    uint16 color_buffer[IPS154_WIDTH_MAX];
    uint16 i = 0, j = 0;
    IPS154_CS(0);
    ips154_set_region(0, 0, ips154_width_max - 1, ips154_height_max - 1);
    for(i = 0; i < ips154_width_max; i++) color_buffer[i] = color;
    for(j = 0; j < ips154_height_max; j++) ips154_write_16bit_data_array(color_buffer, ips154_width_max);
    IPS154_CS(1);
}

void ips154_set_dir(ips154_dir_enum dir) {
    ips154_display_dir = dir;
    switch(dir) {
        case IPS154_PORTAIT:
        case IPS154_PORTAIT_180:
            ips154_width_max = 240;
            ips154_height_max = 240;
            break;
        case IPS154_CROSSWISE:
        case IPS154_CROSSWISE_180:
            ips154_width_max = 240;
            ips154_height_max = 240;
            break;
    }
}

void ips154_set_font(ips154_font_size_enum font) {
    ips154_display_font = font;
}

void ips154_set_color(const uint16 pen, const uint16 bgcolor) {
    ips154_pencolor = pen;
    ips154_bgcolor = bgcolor;
}

void ips154_draw_point(uint16 x, uint16 y, const uint16 color) {
    zf_assert(x < ips154_width_max);
    zf_assert(y < ips154_height_max);
    IPS154_CS(0);
    ips154_set_region(x, y, x, y);
    ips154_write_16bit_data(color);
    IPS154_CS(1);
}

void ips154_draw_line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color) {
    zf_assert(x_start < ips154_width_max);
    zf_assert(y_start < ips154_height_max);
    zf_assert(x_end < ips154_width_max);
    zf_assert(y_end < ips154_height_max);
    int16 x_dir = (x_start < x_end ? 1 : -1);
    int16 y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;
    do {
        if(x_start != x_end) {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        } else {
            while(y_start != y_end) {
                ips154_draw_point(x_start, y_start, color);
                y_start += y_dir;
            }
            ips154_draw_point(x_start, y_start, color);
            break;
        }
        if(func_abs(y_start - y_end) > func_abs(x_start - x_end)) {
            while(y_start != y_end) {
                ips154_draw_point(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16)(((float)y_start - temp_b) / temp_rate);
            }
            ips154_draw_point(x_start, y_start, color);
        } else {
            while(x_start != x_end) {
                ips154_draw_point(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16)((float)x_start * temp_rate + temp_b);
            }
            ips154_draw_point(x_start, y_start, color);
        }
    } while(0);
}

// 其余显示函数（show_char, show_string, show_int, show_uint, show_float, show_binary_image, show_gray_image, show_rgb565_image, show_wave, show_chinese）
// 可直接参考ips114/ips200实现，调用方式和边界检查一致。
// ... 省略 ...

void ips154_show_char(uint16 x, uint16 y, const char dat)
{
    zf_assert(x < ips154_width_max);
    zf_assert(y < ips154_height_max);

    uint8 i = 0, j = 0;

    IPS154_CS(0);
    switch(ips154_display_font)
    {
        case IPS154_6X8_FONT:
            // 可参考ips200_show_char实现6x8字体
            break;
        case IPS154_8X16_FONT:
        {
            uint16 display_buffer[8*16];
            ips154_set_region(x, y, x + 7, y + 15);
            for(i = 0; i < 8; i++)
            {
                uint8 temp_top = ascii_font_8x16[dat - 32][i];
                uint8 temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; j < 8; j++)
                {
                    display_buffer[i + j * 8] = (temp_top & 0x01) ? ips154_pencolor : ips154_bgcolor;
                    temp_top >>= 1;
                }
                for(j = 0; j < 8; j++)
                {
                    display_buffer[i + j * 8 + 4 * 16] = (temp_bottom & 0x01) ? ips154_pencolor : ips154_bgcolor;
                    temp_bottom >>= 1;
                }
            }
            ips154_write_16bit_data_array(display_buffer, 8 * 16);
        }
        break;
        case IPS154_16X16_FONT:
            // 暂不支持
            break;
    }
    IPS154_CS(1);
}

void ips154_show_string(uint16 x, uint16 y, const char dat[])
{
    zf_assert(x < ips154_width_max);
    zf_assert(y < ips154_height_max);

    uint16 j = 0;
    while('\0' != dat[j])
    {
        switch(ips154_display_font)
        {
            case IPS154_6X8_FONT:   ips154_show_char(x + 6 * j, y, dat[j]); break;
            case IPS154_8X16_FONT:  ips154_show_char(x + 8 * j, y, dat[j]); break;
            case IPS154_16X16_FONT: break; // 暂不支持
        }
        j++;
    }
}

void ips154_init(void) {
    spi_init(IPS154_SPI, SPI_MODE0, IPS154_SPI_SPEED, IPS154_SCL_PIN, IPS154_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL);
    gpio_init(IPS154_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS154_RST_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS154_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IPS154_BLK_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    ips154_set_dir(ips154_display_dir);
    ips154_set_color(ips154_pencolor, ips154_bgcolor);
    IPS154_RST(0);
    system_delay_ms(5);
    IPS154_RST(1);
    system_delay_ms(120);
    IPS154_CS(0);
    ips154_write_index(0x11);
    system_delay_ms(120);
    ips154_write_index(0x36);
    switch(ips154_display_dir) {
        case IPS154_PORTAIT:        ips154_write_8bit_data(0x00);   break;
        case IPS154_PORTAIT_180:    ips154_write_8bit_data(0xC0);   break;
        case IPS154_CROSSWISE:      ips154_write_8bit_data(0x70);   break;
        case IPS154_CROSSWISE_180:  ips154_write_8bit_data(0xA0);   break;
    }
    ips154_write_index(0x3A);
    ips154_write_8bit_data(0x05);
    ips154_write_index(0xB2);
    ips154_write_8bit_data(0x0C);
    ips154_write_8bit_data(0x0C);
    ips154_write_8bit_data(0x00);
    ips154_write_8bit_data(0x33);
    ips154_write_8bit_data(0x33);
    ips154_write_index(0xB7);
    ips154_write_8bit_data(0x35);
    ips154_write_index(0xBB);
    ips154_write_8bit_data(0x29);
    ips154_write_index(0xC2);
    ips154_write_8bit_data(0x01);
    ips154_write_index(0xC3);
    ips154_write_8bit_data(0x19);
    ips154_write_index(0xC4);
    ips154_write_8bit_data(0x20);
    ips154_write_index(0xC5);
    ips154_write_8bit_data(0x1A);
    ips154_write_index(0xC6);
    ips154_write_8bit_data(0x0F);
    ips154_write_index(0xD0);
    ips154_write_8bit_data(0xA4);
    ips154_write_8bit_data(0xA1);
    ips154_write_index(0xE0);
    ips154_write_8bit_data(0xD0);
    ips154_write_8bit_data(0x08);
    ips154_write_8bit_data(0x0E);
    ips154_write_8bit_data(0x09);
    ips154_write_8bit_data(0x09);
    ips154_write_8bit_data(0x05);
    ips154_write_8bit_data(0x31);
    ips154_write_8bit_data(0x33);
    ips154_write_8bit_data(0x48);
    ips154_write_8bit_data(0x17);
    ips154_write_8bit_data(0x14);
    ips154_write_8bit_data(0x15);
    ips154_write_8bit_data(0x31);
    ips154_write_8bit_data(0x34);
    ips154_write_index(0xE1);
    ips154_write_8bit_data(0xD0);
    ips154_write_8bit_data(0x08);
    ips154_write_8bit_data(0x0E);
    ips154_write_8bit_data(0x09);
    ips154_write_8bit_data(0x09);
    ips154_write_8bit_data(0x15);
    ips154_write_8bit_data(0x31);
    ips154_write_8bit_data(0x33);
    ips154_write_8bit_data(0x48);
    ips154_write_8bit_data(0x17);
    ips154_write_8bit_data(0x14);
    ips154_write_8bit_data(0x15);
    ips154_write_8bit_data(0x31);
    ips154_write_8bit_data(0x34);
    ips154_write_index(0x21);
    ips154_write_index(0x29);
    IPS154_CS(1);
    ips154_clear();
} 