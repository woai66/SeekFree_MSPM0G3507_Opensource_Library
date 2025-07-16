#include "zf_device_st7789.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_spi.h"

static uint16 st7789_pencolor = ST7789_DEFAULT_PENCOLOR;
static uint16 st7789_bgcolor = ST7789_DEFAULT_BGCOLOR;
static st7789_dir_enum st7789_display_dir = ST7789_DEFAULT_DISPLAY_DIR;
static st7789_font_size_enum st7789_display_font = ST7789_DEFAULT_DISPLAY_FONT;
static uint16 st7789_width_max = ST7789_WIDTH_MAX;
static uint16 st7789_height_max = ST7789_HEIGHT_MAX;

#define ST7789_CS(x) ((x) ? (gpio_high(ST7789_CS_PIN)) : (gpio_low(ST7789_CS_PIN)))
#define ST7789_DC(x) ((x) ? (gpio_high(ST7789_DC_PIN)) : (gpio_low(ST7789_DC_PIN)))
#define ST7789_RST(x) ((x) ? (gpio_high(ST7789_RST_PIN)) : (gpio_low(ST7789_RST_PIN)))
#define ST7789_BLK(x) ((x) ? (gpio_high(ST7789_BLK_PIN)) : (gpio_low(ST7789_BLK_PIN)))

#define st7789_write_8bit_data(data) (spi_write_8bit(ST7789_SPI, (data)))
#define st7789_write_16bit_data(data) (spi_write_16bit(ST7789_SPI, (data)))
#define st7789_write_16bit_data_array(data, len) (spi_write_16bit_array(ST7789_SPI, (data), (len)))

static void st7789_write_index(const uint8 dat) {
    ST7789_DC(0);
    st7789_write_8bit_data(dat);
    ST7789_DC(1);
}

static void st7789_set_region(uint16 x1, uint16 y1, uint16 x2, uint16 y2) {
    zf_assert(x1 < st7789_width_max);
    zf_assert(y1 < st7789_height_max);
    zf_assert(x2 < st7789_width_max);
    zf_assert(y2 < st7789_height_max);
    st7789_write_index(0x2A);
    st7789_write_8bit_data(x1 >> 8);
    st7789_write_8bit_data(x1 & 0xFF);
    st7789_write_8bit_data(x2 >> 8);
    st7789_write_8bit_data(x2 & 0xFF);
    st7789_write_index(0x2B);
    st7789_write_8bit_data(y1 >> 8);
    st7789_write_8bit_data(y1 & 0xFF);
    st7789_write_8bit_data(y2 >> 8);
    st7789_write_8bit_data(y2 & 0xFF);
    st7789_write_index(0x2C);
}

void st7789_clear(void) {
    uint16 color_buffer[ST7789_WIDTH_MAX];
    uint16 i = 0, j = 0;
    ST7789_CS(0);
    st7789_set_region(0, 0, st7789_width_max - 1, st7789_height_max - 1);
    for(i = 0; i < st7789_width_max; i++) color_buffer[i] = st7789_bgcolor;
    for(j = 0; j < st7789_height_max; j++) st7789_write_16bit_data_array(color_buffer, st7789_width_max);
    ST7789_CS(1);
}

void st7789_full(const uint16 color) {
    uint16 color_buffer[ST7789_WIDTH_MAX];
    uint16 i = 0, j = 0;
    ST7789_CS(0);
    st7789_set_region(0, 0, st7789_width_max - 1, st7789_height_max - 1);
    for(i = 0; i < st7789_width_max; i++) color_buffer[i] = color;
    for(j = 0; j < st7789_height_max; j++) st7789_write_16bit_data_array(color_buffer, st7789_width_max);
    ST7789_CS(1);
}

void st7789_set_dir(st7789_dir_enum dir) {
    st7789_display_dir = dir;
    switch(dir) {
        case ST7789_PORTAIT:
        case ST7789_PORTAIT_180:
            st7789_width_max = 240;
            st7789_height_max = 320;
            break;
        case ST7789_CROSSWISE:
        case ST7789_CROSSWISE_180:
            st7789_width_max = 320;
            st7789_height_max = 240;
            break;
    }
}

void st7789_set_font(st7789_font_size_enum font) {
    st7789_display_font = font;
}

void st7789_set_color(const uint16 pen, const uint16 bgcolor) {
    st7789_pencolor = pen;
    st7789_bgcolor = bgcolor;
}

void st7789_draw_point(uint16 x, uint16 y, const uint16 color) {
    zf_assert(x < st7789_width_max);
    zf_assert(y < st7789_height_max);
    ST7789_CS(0);
    st7789_set_region(x, y, x, y);
    st7789_write_16bit_data(color);
    ST7789_CS(1);
}

void st7789_show_char(uint16 x, uint16 y, const char dat)
{
    zf_assert(x < st7789_width_max);
    zf_assert(y < st7789_height_max);
    uint8 i = 0, j = 0;
    ST7789_CS(0);
    switch(st7789_display_font)
    {
        case ST7789_6X8_FONT:
            // 可参考ips200_show_char实现6x8字体
            break;
        case ST7789_8X16_FONT:
        {
            uint16 display_buffer[8*16];
            st7789_set_region(x, y, x + 7, y + 15);
            for(i = 0; i < 8; i++)
            {
                uint8 temp_top = ascii_font_8x16[dat - 32][i];
                uint8 temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; j < 8; j++)
                {
                    display_buffer[i + j * 8] = (temp_top & 0x01) ? st7789_pencolor : st7789_bgcolor;
                    temp_top >>= 1;
                }
                for(j = 0; j < 8; j++)
                {
                    display_buffer[i + j * 8 + 4 * 16] = (temp_bottom & 0x01) ? st7789_pencolor : st7789_bgcolor;
                    temp_bottom >>= 1;
                }
            }
            st7789_write_16bit_data_array(display_buffer, 8 * 16);
        }
        break;
        case ST7789_16X16_FONT:
            // 暂不支持
            break;
    }
    ST7789_CS(1);
}

void st7789_show_string(uint16 x, uint16 y, const char dat[])
{
    zf_assert(x < st7789_width_max);
    zf_assert(y < st7789_height_max);
    uint16 j = 0;
    while('\0' != dat[j])
    {
        switch(st7789_display_font)
        {
            case ST7789_6X8_FONT:   st7789_show_char(x + 6 * j, y, dat[j]); break;
            case ST7789_8X16_FONT:  st7789_show_char(x + 8 * j, y, dat[j]); break;
            case ST7789_16X16_FONT: break; // 暂不支持
        }
        j++;
    }
}

void st7789_init(void) {
    spi_init(ST7789_SPI, SPI_MODE0, ST7789_SPI_SPEED, ST7789_SCL_PIN, ST7789_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL);
    gpio_init(ST7789_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(ST7789_RST_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(ST7789_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(ST7789_BLK_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    st7789_set_dir(st7789_display_dir);
    st7789_set_color(st7789_pencolor, st7789_bgcolor);
    ST7789_RST(0);
    system_delay_ms(5);
    ST7789_RST(1);
    system_delay_ms(120);
    ST7789_CS(0);
    st7789_write_index(0x11); // Sleep Out
    system_delay_ms(120);
    st7789_write_index(0x36); // Memory Data Access Control
    switch(st7789_display_dir) {
        case ST7789_PORTAIT:        st7789_write_8bit_data(0x00);   break;
        case ST7789_PORTAIT_180:    st7789_write_8bit_data(0xC0);   break;
        case ST7789_CROSSWISE:      st7789_write_8bit_data(0x70);   break;
        case ST7789_CROSSWISE_180:  st7789_write_8bit_data(0xA0);   break;
    }
    st7789_write_index(0x3A); // Interface Pixel Format
    st7789_write_8bit_data(0x05); // 16bit/pixel
    st7789_write_index(0xB2); // Porch Setting
    st7789_write_8bit_data(0x0C);
    st7789_write_8bit_data(0x0C);
    st7789_write_8bit_data(0x00);
    st7789_write_8bit_data(0x33);
    st7789_write_8bit_data(0x33);
    st7789_write_index(0xB7); // Gate Control
    st7789_write_8bit_data(0x35);
    st7789_write_index(0xBB); // VCOM Setting
    st7789_write_8bit_data(0x19);
    st7789_write_index(0xC0); // LCM Control
    st7789_write_8bit_data(0x2C);
    st7789_write_index(0xC2); // VDV and VRH Command Enable
    st7789_write_8bit_data(0x01);
    st7789_write_index(0xC3); // VRH Set
    st7789_write_8bit_data(0x12);
    st7789_write_index(0xC4); // VDV Set
    st7789_write_8bit_data(0x20);
    st7789_write_index(0xC6); // Frame Rate Control in Normal Mode
    st7789_write_8bit_data(0x0F);
    st7789_write_index(0xD0); // Power Control 1
    st7789_write_8bit_data(0xA4);
    st7789_write_8bit_data(0xA1);
    st7789_write_index(0xE0); // Positive Voltage Gamma Control
    st7789_write_8bit_data(0xD0);
    st7789_write_8bit_data(0x04);
    st7789_write_8bit_data(0x0D);
    st7789_write_8bit_data(0x11);
    st7789_write_8bit_data(0x13);
    st7789_write_8bit_data(0x2B);
    st7789_write_8bit_data(0x3F);
    st7789_write_8bit_data(0x54);
    st7789_write_8bit_data(0x4C);
    st7789_write_8bit_data(0x18);
    st7789_write_8bit_data(0x0D);
    st7789_write_8bit_data(0x0B);
    st7789_write_8bit_data(0x1F);
    st7789_write_8bit_data(0x23);
    st7789_write_index(0xE1); // Negative Voltage Gamma Control
    st7789_write_8bit_data(0xD0);
    st7789_write_8bit_data(0x04);
    st7789_write_8bit_data(0x0C);
    st7789_write_8bit_data(0x11);
    st7789_write_8bit_data(0x13);
    st7789_write_8bit_data(0x2C);
    st7789_write_8bit_data(0x3F);
    st7789_write_8bit_data(0x44);
    st7789_write_8bit_data(0x51);
    st7789_write_8bit_data(0x2F);
    st7789_write_8bit_data(0x1F);
    st7789_write_8bit_data(0x1F);
    st7789_write_8bit_data(0x20);
    st7789_write_8bit_data(0x23);
    st7789_write_index(0x21); // Display Inversion On
    st7789_write_index(0x29); // Display On
    ST7789_CS(1);
    st7789_clear();
} 