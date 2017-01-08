#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <windows.h>
#include <cstdio>
#include <wingdi.h>

#include "Color.h"

inline 
void DEBUG_ASSERT(bool x){
	if (!x){
		OutputDebugStringA("ERROR");
	}
}

extern TCHAR* WINDOW_NAME;
extern HWND g_hwnd;

inline
UINT TO_MUL4(UINT a) { return (a & 0x3) ? ( (a & 0xfffffc) + 0x4) : a; }

template <typename ColorType>
class ImgBuffer;

namespace BitMap {
	template <typename ColorType>
	void Display(const ImgBuffer<ColorType>* buffer) {
		UINT BitMapChannel = sizeof(ColorType);

		HDC hdc = GetDC(g_hwnd);

		BITMAPINFO hBitMap;
		ZeroMemory(&hBitMap, sizeof(BITMAPINFO));
		hBitMap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		hBitMap.bmiHeader.biWidth = buffer->width;
		hBitMap.bmiHeader.biHeight = buffer->height;
		hBitMap.bmiHeader.biPlanes = 1;
		hBitMap.bmiHeader.biBitCount = 8 * BitMapChannel;
		hBitMap.bmiHeader.biCompression = BI_RGB;
		hBitMap.bmiHeader.biSizeImage = TO_MUL4(buffer->width) * buffer->height;

		HDC dc = CreateCompatibleDC(hdc);

		char* pImg = NULL;
		HBITMAP bmp = CreateDIBSection(dc, &hBitMap, DIB_RGB_COLORS, (void**)&pImg, NULL, 0);

		if (!pImg) {
			ReleaseDC(g_hwnd, dc);
			return;
		}

		for (int y = 0; y != buffer->height; ++y) {
			char* pLine = pImg + TO_MUL4(buffer->width * BitMapChannel) * y;
			for (int x = 0; x < buffer->width; x++)
			{
				auto dstPtr = buffer->pPixelAt(x, y);
				//pLine[0 + x * BitMapChannel] = 255;
				//pLine[1 + x * BitMapChannel] = 0;
				//pLine[2 + x * BitMapChannel] = 255;
				pLine[0 + x * BitMapChannel] = dstPtr->_z;
				pLine[1 + x * BitMapChannel] = dstPtr->_y;
				pLine[2 + x * BitMapChannel] = dstPtr->_x;
			}
		}

		auto oldTmp = SelectObject(dc, bmp);
		SetDIBits(hdc, bmp, 0, buffer->height, buffer, &hBitMap, DIB_RGB_COLORS);
		//SetDIBitsToDevice(hdc, 0, 0, buffer->width, buffer->height, 0, 0, 0, buffer->height, pImg, &hBitMap, SRCCOPY);
		BitBlt(hdc, 0, 0, buffer->width, buffer->height, dc, 0, 0, SRCCOPY);
		SelectObject(dc, oldTmp);

		DeleteDC(dc);
		ReleaseDC(g_hwnd, hdc);
		DeleteObject(bmp);
	}

	ImgBuffer<ShortColor4>* Load(const char* filename);
	ImgBuffer<ShortColor4>* LoadBMP(FILE* pf, const char* filename);
	ImgBuffer<ShortColor4>* LoadPNG(FILE* pf, const char* filename);
	ImgBuffer<ShortColor4>* LoadTGA(FILE* pf, const char* filename);
}
#endif