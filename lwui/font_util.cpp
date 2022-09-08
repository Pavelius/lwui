#include "crt.h"
#include "draw.h"
#include "win.h"
#include "font_util.h"
#include "sprite_util.h"

using namespace util;

static void wcpy(wchar_t* d, const char* s) {
	while(*s)
		*d++ = *s++;
	*d++ = 0;
}

font::font(const char* name, int size) {
	wchar_t name1[260];
	wcpy(name1, name);
	void* hf = CreateFontW(size, 0, 0, 0, FW_NORMAL, 0, 0,
		0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE,
		name1);
	hcnv = CreateCompatibleDC(0);
	hfnt = SelectObject(hcnv, hf);
}

font::~font() {
	DeleteObject(SelectObject(hcnv, hfnt));
	DeleteDC(hcnv);
}

// tool used to create font with SubPixel rendering from system font's
int font::glyphi(int glyph,
	int& width, int& height, int& dx, int& dy, int& ox, int& oy,
	unsigned char* buffer, int maxsize, int xscale, int yscale) {
	MAT2 scale3h;
	memset(&scale3h, 0, sizeof(MAT2));
	scale3h.eM11.value = xscale; // rgb color
	scale3h.eM22.value = yscale;
	GLYPHMETRICS gm;
	memset(&gm, 0, sizeof(gm));
	int size = GetGlyphOutlineW(hcnv,
		glyph,
		GGO_GRAY8_BITMAP,
		&gm,
		maxsize,
		buffer,
		&scale3h);
	width = gm.gmBlackBoxX;
	height = gm.gmBlackBoxY;
	dx = gm.gmCellIncX;
	dy = gm.gmCellIncY;
	ox = gm.gmptGlyphOrigin.x;
	oy = gm.gmptGlyphOrigin.y;
	return size;
}

void font::info(int& width, int& height, int& dy1, int& dy2) {
	TEXTMETRICA tm;
	GetTextMetricsA(hcnv, &tm);
	width = tm.tmAveCharWidth;
	height = tm.tmHeight;
	dy1 = tm.tmAscent;
	dy2 = tm.tmDescent;
}

static int primary[256];
static int secondary[256];
static int tertiary[256];
static unsigned char buf1[128 * 128];
static unsigned char buf2[128 * 128];
static short int widths[4096];
static int intervals[][2] = {{0x21, 0x7E},
	{0x410, 0x44F},
	{0x456, 0x457}, // ”краинские буквы маленькие
	{0x406, 0x407}, // ”краинские буквы большие
};

inline int floor(double f) {
	return ((int)(f * 10.00)) / 10;
}

const int num_levels = 65; // Constant GGO_GRAY8_BITMAP generate 65 levels of gray

static void lcd_init(double prim, double second, double tert) {
	double norm = (255.0 / ((double)num_levels)) / (prim + second + tert);
	prim *= norm;
	second *= norm;
	tert *= norm;
	for(int i = 0; i < num_levels; i++) {
		primary[i] = floor(prim * i);
		secondary[i] = floor(second * i);
		tertiary[i] = floor(tert * i);
	}
}

static void lcd_prepare_glyph(unsigned char* gbuf2, int& w2, const unsigned char* gbuf1, int w1, int h1) {
	int src_stride = (w1 + 3) / 4 * 4;
	int dst_width = src_stride + 4;
	for(int y = 0; y < h1; ++y) {
		const unsigned char* src_ptr = gbuf1 + src_stride * y;
		unsigned char* dst_ptr = gbuf2 + dst_width * y;
		for(int x = 0; x < w1; ++x) {
			unsigned v = *src_ptr++;
			dst_ptr[0] = imin((int)dst_ptr[0] + tertiary[v], 255);
			dst_ptr[1] = imin((int)dst_ptr[1] + secondary[v], 255);
			dst_ptr[2] = imin((int)dst_ptr[2] + primary[v], 255);
			dst_ptr[3] = imin((int)dst_ptr[3] + secondary[v], 255);
			dst_ptr[4] = imin((int)dst_ptr[4] + tertiary[v], 255);
			++dst_ptr;
		}
	}
	w2 = dst_width;
}

static void lcd_prepare_glyph2(unsigned char* gbuf2, int& w2, const unsigned char* gbuf1, int w1, int h1) {
	int src_stride = (w1 + 3) / 4 * 4;
	int dst_width = src_stride + 4;
	for(int y = 0; y < h1; ++y) {
		const unsigned char* src_ptr = gbuf1 + src_stride * y;
		unsigned char* dst_ptr = gbuf2 + dst_width * y;
		unsigned char v;
		for(int x = 0; x < w1; ++x) {
			v = *src_ptr++;
			*dst_ptr = (v>>1)<<2;
			++dst_ptr;
		}
	}
	w2 = dst_width;
}

static void glyph(util::font& font, int g, sprite& ei) {
	int w1 = 0, w2 = 0, dx, dy, ox, oy;
	int h1 = 0;
	memset(buf1, 0, sizeof(buf1));
	memset(buf2, 0, sizeof(buf2));
	int size = font.glyphi(g, w1, h1, dx, dy, ox, oy, buf1, sizeof(buf1), 3, 1);
	if(!size)
		return;
	lcd_prepare_glyph(buf2, w2, buf1, w1, h1);
	sprite_store(&ei, buf2, w2, ((w1 + 5) / 3) * 3, h1, -(ox + 1) / 3, oy, sprite::ALC);
	int id = ei.glyph(g);
	widths[id] = (dx + 2) / 3;
}

void font::write(const char* url, const char* name, int size) {
	int height, width;
	int ascend;
	int descend;
	lcd_init(2.0, 0.2, 0.05);
	font font(name, size);
	int glyph_count = 0;
	for(auto& e : intervals)
		glyph_count += e[1] - e[0] + 1;
	auto p = (sprite*)new char[256 * 256 * 8];
	if(!p)
		return;
	sprite_create(p, glyph_count);
	sprite_add(p, intervals, sizeof(intervals));
	for(auto& e : intervals) {
		for(int i = e[0]; i <= e[1]; i++)
			glyph(font, i, *p);
	}
	sprite_add(p, widths, sizeof(widths[0]) * glyph_count);
	font.info(width, height, ascend, descend);
	p->height = height;
	p->width = draw::textw(p);
	p->ascend = ascend;
	p->descend = descend;
	sprite_write(url, p);
	delete p;
}