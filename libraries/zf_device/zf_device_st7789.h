#ifndef _zf_device_st7789_h_
#define _zf_device_st7789_h_

#include "zf_common_typedef.h"

#define ST7789_DEFAULT_DISPLAY_DIR      ( ST7789_PORTAIT   )
#define ST7789_DEFAULT_PENCOLOR         ( RGB565_RED       )
#define ST7789_DEFAULT_BGCOLOR          ( RGB565_WHITE     )
#define ST7789_DEFAULT_DISPLAY_FONT     ( ST7789_8X16_FONT )

#define ST7789_RST_PIN                  ( A7 )
#define ST7789_DC_PIN                   ( A15 )
#define ST7789_CS_PIN                   ( A8 )
#define ST7789_BLK_PIN                  ( A13 )

#define ST7789_SPI                      ( SPI_0 )
#define ST7789_SPI_SPEED                ( 30 * 1000 * 1000 )
#define ST7789_SCL_PIN                  ( SPI0_SCK_A12 )
#define ST7789_SDA_PIN                  ( SPI0_MOSI_A9 )

#define ST7789_WIDTH_MAX                ( 240 )
#define ST7789_HEIGHT_MAX               ( 320 )

typedef enum {
    ST7789_PORTAIT = 0,
    ST7789_PORTAIT_180 = 1,
    ST7789_CROSSWISE = 2,
    ST7789_CROSSWISE_180 = 3,
} st7789_dir_enum;

typedef enum {
    ST7789_6X8_FONT = 0,
    ST7789_8X16_FONT = 1,
    ST7789_16X16_FONT = 2,
} st7789_font_size_enum;

void st7789_clear(void);
void st7789_full(const uint16 color);
void st7789_set_dir(st7789_dir_enum dir);
void st7789_set_font(st7789_font_size_enum font);
void st7789_set_color(const uint16 pen, const uint16 bgcolor);
void st7789_draw_point(uint16 x, uint16 y, const uint16 color);
void st7789_draw_line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);
void st7789_show_char(uint16 x, uint16 y, const char dat);
void st7789_show_string(uint16 x, uint16 y, const char dat[]);
void st7789_show_int(uint16 x, uint16 y, const int32 dat, uint8 num);
void st7789_show_uint(uint16 x, uint16 y, const uint32 dat, uint8 num);
void st7789_show_float(uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);
void st7789_show_binary_image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void st7789_show_gray_image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);
void st7789_show_rgb565_image(uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);
void st7789_show_wave(uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void st7789_show_chinese(uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);
void st7789_init(void);

#endif 