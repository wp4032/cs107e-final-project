#include "gl.h"
#include "font.h"
#include "printf.h"
#include "strings.h"
#include "math.h"

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


color_t brighten_color(color_t c, float alpha) {
    // If invalid alpha 
    if (alpha > 1.0 || alpha < 0.0) {
        return GL_BLACK;
    }
    
    unsigned char b = (c & 0xff); // red in 2nd most significant bit
    unsigned char g = (c & 0xff00) >> 8; // red in 2nd most significant bit
    unsigned char r = (c & 0xff0000) >> 16; // red in 2nd most significant bit

    b = ((0xff - b) * 0.25 * alpha) + b;
    g = ((0xff - g) * 0.25 * alpha) + g;
    r = ((0xff - r) * 0.25 * alpha) + r;

    return gl_color(r, g, b);
}


void gl_draw_line(int x1, int y1, int x2, int y2, color_t c) {
    int steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }
    if (x2 < x1) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    float delta_x = (float) (x2 - x1);
    float delta_y = (float) (y2 - y1);

    float slope = delta_y / delta_x; 
    if (delta_x == 0.0) {
        slope = 1.0;
    }

    float y_run = y1 + slope;

    if (steep) {
        for (int x = x1; x < x2; x++) {
            gl_draw_pixel(floor(y_run), x, brighten_color(c, decimals_flipped(y_run)));
            gl_draw_pixel(floor(y_run) + 1, x, brighten_color(c, decimals(y_run)));
            y_run += slope;
        }
    }
    else {
        for (int x = x1; x < x2; x++) {
            gl_draw_pixel(x, floor(y_run), brighten_color(c, decimals_flipped(y_run)));
            gl_draw_pixel(x, floor(y_run) + 1, brighten_color(c, decimals(y_run)));
            y_run += slope;
        } 
    }
}


// FUNCTION: gl_draw_circle_outline
// PARAMS: int x, int y, int r, color_t c
// RETURNS: draws an outlined circle at (x, y) with radius r and color c
void gl_draw_circle_outline(int x, int y, int r, color_t c) {
    gl_draw_circle(x, y, r, c, 0);
}


// FUNCTION: gl_draw_circle_filled
// PARAMS: int x, int y, int r, color_t c
// RETURNS: draws a filled in circle at (x, y) with radius r and color c
void gl_draw_circle_filled(int x, int y, int r, color_t c) {
    gl_draw_circle(x, y, r, c, 1);
}

// FUNCTION: gl_draw_circle_outline
// PARAMS: int x, int y, int r, color_t c, int filled
// RETURNS: draws an outline of a circle at (x, y) with radius r and color c
// if filled == 0 --> will draw outline; if filled == 1 --> will draw filled
// helper function for simpler functions (gl_draw_circle_filled & gl_draw_circle_outline)
// CITATION: https://www.gatevidyalay.com/bresenham-circle-drawing-algorithm/
static void gl_draw_circle(int x, int y, int r, color_t c, int filled) {
    if (filled > 1 || filled < 0) {
        return;
    }

    int x_0 = 0;
    int y_0 = r;
    int p = 3 - (2 * r);

    while (x_0 <= y_0) {
        if (filled == 0) {
            gl_draw_circle_octant(x_0, y_0, x, y, c);
        }
        if (filled == 1) {
            gl_draw_circle_octant_filled(x_0, y_0, x, y, c);
        }
        if (p < 0) {
            x_0++;
            p += (4 * x_0) + 6;
        }
        else if (p >= 0) {
            x_0++;
            y_0--;
            p += (4 * (x_0 - y_0)) + 10;
        }
    }
}


// FUNCTION: gl_draw_circle_octant
// PARAMS: int x, int y, int center_x, int center_y, color_t c
// RETURNS: when drawing a pixel around a center x and center y, it will then draw in other octants
// CITATION: https://www.cs.helsinki.fi/group/goa/mallinnus/ympyrat/ymp1.html
void gl_draw_circle_octant(int x, int y, int center_x, int center_y, color_t c) {
    gl_draw_pixel(center_x + x, center_y + y, c);
    gl_draw_pixel(center_x + y, center_y + x, c);
    gl_draw_pixel(center_x + y, center_y - x, c);
    gl_draw_pixel(center_x + x, center_y - y, c);
    gl_draw_pixel(center_x - y, center_y + x, c);
    gl_draw_pixel(center_x - x, center_y + y, c);
    gl_draw_pixel(center_x - x, center_y - y, c);
    gl_draw_pixel(center_x - y, center_y - x, c);
}

// FUNCTION: gl_draw_circle_octant_filled
// PARAMS: int x, int y, int center_x, int center_y, color_t c
// RETURNS: when drawing a pixel around a center x and center y, it will then draw in other octants
// CITATION: https://www.cs.helsinki.fi/group/goa/mallinnus/ympyrat/ymp1.html
void gl_draw_circle_octant_filled(int x, int y, int center_x, int center_y, color_t c) {
    gl_draw_circle_octant(x, y, center_x, center_y, c);
    gl_draw_line(center_x - x, center_y + y, center_x + x, center_y + y, c);
    gl_draw_line(center_x - x, center_y - y, center_x + x, center_y - y, c);
    gl_draw_line(center_x - y, center_y + x, center_x + y, center_y + x, c);
    gl_draw_line(center_x - y, center_y - x, center_x + y, center_y - x, c);
}