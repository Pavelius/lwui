#pragma once

// Util namespace
namespace util {
class font {
	void*			hcnv;
	void*			hfnt;
public:
	font(const char* name, int size);
	~font();
	int				glyphi(int glyph, int& width, int& height, int& dx, int& dy, int& ox, int& oy, unsigned char* buffer, int maxsize, int xscale, int yscale);
	void			info(int& width, int& height, int& dy1, int& dy2);
	static void		write(const char* url, const char* name, int size);
};
}