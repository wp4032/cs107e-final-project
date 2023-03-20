#include "gl.h"
#include "font.h"
#include "printf.h"
#include "strings.h"

void gl_copy_buffer(void);

static unsigned int actual_width = 0;
static gl_mode_t gl_mode;

void gl_init(unsigned int width, unsigned int height, gl_mode_t mode)
{
    fb_init(width, height, 4, mode);                    // use 32-bit depth always for graphics library
    actual_width = fb_get_pitch() / fb_get_depth();     // width is pitch / depth and not fb.width
    gl_mode = mode;
}

void gl_swap_buffer(void)
{
    fb_swap_buffer();
}

void gl_copy_buffer(void)
{
    // Gets the current framebuffer that is hidden before the swap (will be shown after swap)
    unsigned int (*fb_show)[actual_width] = (unsigned int (*)[actual_width])fb_get_draw_buffer();
    fb_swap_buffer();

    // Gets the current framebuffer that is hidden after the swap
    unsigned int (*fb_draw)[actual_width] = (unsigned int (*)[actual_width])fb_get_draw_buffer();

    // Copies what is shown to what is hidden so framebuffers are updated
    int w = gl_get_width();
    int h = gl_get_height();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (fb_draw[y][x] != fb_show[y][x]) {
                fb_draw[y][x] = fb_show[y][x];
            }
        }  
    } 
}

unsigned int gl_get_width(void)
{
    return fb_get_width();
}

unsigned int gl_get_height(void)
{
    return fb_get_height();
}

color_t gl_color(unsigned char r, unsigned char g, unsigned char b)
{
    color_t c = 0xff000000;         // alpha in 1st most significant bit (always 0xff)
    c |= ((unsigned int) r) << 16;  // red in 2nd most significant bit
    c |= ((unsigned int) g) << 8;   // green in 3rd most siginficant bit
    c |= ((unsigned int) b);        // blue in 4th most significant bit
    return c;
}

void gl_clear(color_t c)
{
    unsigned int (*fb)[actual_width] = (unsigned int (*)[actual_width])fb_get_draw_buffer();
    // Loop to draw in all pixels in framebuffer
    for (int y = 0; y < gl_get_height(); y++) {
        for (int x = 0; x < gl_get_width(); x++) {
            fb[y][x] = c;
        }
    }
}

void gl_draw_pixel(int x, int y, color_t c)
{
    if ((x < 0 || x > gl_get_width() - 1) || (y < 0 || y > gl_get_height() - 1)) return;
    
    // Typecasts framebuffer to 2D array
    unsigned int (*fb)[actual_width] = (unsigned int (*)[actual_width])fb_get_draw_buffer();
    fb[y][x] = (unsigned int) c;
}

color_t gl_read_pixel(int x, int y)
{
    if ((x < 0 || x > gl_get_width() - 1) || (y < 0 || y > gl_get_height() - 1)) return 0;

    // Typecasts framebuffer to 2D array
    unsigned int (*fb)[actual_width] = (unsigned int (*)[actual_width])fb_get_draw_buffer();
    return fb[y][x];
}

void gl_draw_rect(int x, int y, int w, int h, color_t c)
{   
    // If statements crop the width and height if out of bounds (faster)
    if (x < 0) {
        w = w + x;
        x = 0;
    }
    if (y < 0) {
        h = h + y;
        y = 0;
    }
    if (x + w > gl_get_width()) w = gl_get_width() - x;
    if (y + h > gl_get_height()) h = gl_get_height() - y; 
    
    // Loop to draw rectangle starting at top left corner (x, y) with w and h
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            gl_draw_pixel(x + i, y + j, c);
        }
    }
}

void gl_draw_char(int x, int y, char ch, color_t c)
{
    // Rejects invalid characters that are not in the font set
    if (ch < 0x21 || ch > 0x7f) return;

    unsigned char buf[font_get_glyph_size()];
    font_get_glyph(ch, buf, sizeof(buf));           // Gets pixel of each glyph from font.h
    int w = font_get_glyph_width();
    int h = font_get_glyph_height();
    unsigned char (*img)[w] = &buf;                 // Casts buf into 2d array

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (img[j][i] != 0) { 
                gl_draw_pixel(x + i, y + j, c);     // Only draws pixel if buf coordinate not empty
            }
        }   
    }   

}

void gl_draw_string(int x, int y, const char* str, color_t c)
{
    int len = strlen(str);
    int w = font_get_glyph_width();
    
    // If the string is way too long, will cut be cut off
    if (x + (len * w) > gl_get_width()) {
        len = ((gl_get_width() - x) / w) + 1;
    }

    // Draws each char in the string
    for (int i = 0; i < len; i++) { 
        gl_draw_char(x + (i * w), y, str[i], c);
    }
}

unsigned int gl_get_char_height(void)
{
    return font_get_glyph_height();
}

unsigned int gl_get_char_width(void)
{
    return font_get_glyph_width();
}
