#ifndef _zf_device_ips154_h_
#define _zf_device_ips154_h_

#include "zf_common_typedef.h"

#define IPS154_DEFAULT_DISPLAY_DIR      ( IPS154_PORTAIT   )
#define IPS154_DEFAULT_PENCOLOR         ( RGB565_RED       )
#define IPS154_DEFAULT_BGCOLOR          ( RGB565_WHITE     )
#define IPS154_DEFAULT_DISPLAY_FONT     ( IPS154_8X16_FONT )

#define IPS154_RST_PIN                  ( A7 )
#define IPS154_DC_PIN                   ( A15 )
#define IPS154_CS_PIN                   ( A8 )
#define IPS154_BLK_PIN                  ( A13 )

#define IPS154_SPI                      ( SPI_0 )
#define IPS154_SPI_SPEED                ( 30 * 1000 * 1000 )
#define IPS154_SCL_PIN                  ( SPI0_SCK_A12 )
#define IPS154_SDA_PIN                  ( SPI0_MOSI_A9 )

#define IPS154_WIDTH_MAX                ( 240 )
#define IPS154_HEIGHT_MAX               ( 240 )

typedef enum {
    IPS154_PORTAIT = 0,
    IPS154_PORTAIT_180 = 1,
    IPS154_CROSSWISE = 2,
    IPS154_CROSSWISE_180 = 3,
} ips154_dir_enum;

typedef enum {
    IPS154_6X8_FONT = 0,
    IPS154_8X16_FONT = 1,
    IPS154_16X16_FONT = 2,
} ips154_font_size_enum;

void ips154_clear(void);
void ips154_full(const uint16 color);
void ips154_set_dir(ips154_dir_enum dir);
void ips154_set_font(ips154_font_size_enum font);
void ips154_set_color(const uint16 pen, const uint16 bgcolor);
void ips154_draw_point(uint16 x, uint16 y, const uint16 color);
void ips154_draw_line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);
void ips154_show_char(uint16 x, uint16 y, const char dat);
void ips154_show_string(uint16 x, uint16 y, const char dat[]);
void ips154_show_int(uint16 x, uint16 y, const int32 dat, uint8 num);
void ips154_show_uint(uint16 x, uint16 y, const uint32 dat, uint8 num);
void ips154_show_float(uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);
void ips154_show_binary_image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips154_show_gray_image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);
void ips154_show_rgb565_image(uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);
void ips154_show_wave(uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void ips154_show_chinese(uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);
void ips154_init(void);

#endif 