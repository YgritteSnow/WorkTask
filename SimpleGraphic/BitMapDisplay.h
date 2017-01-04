#ifndef __BITMAPDISPLAY_H__
#define __BITMAPDISPLAY_H__

#include <windows.h>
#include <wingdi.h>

//#pragma comment(lib, "Gdi32.lib")

extern TCHAR* WINDOW_NAME;
extern HWND g_hwnd;
extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;

inline
UINT TO_MUL4(UINT a) { return (a & 0x3) ? ( (a & 0xfffffc) + 0x4) : a; }

namespace BitMapDisplay {
	template <typename ColorType>
	void Display(const ImgBuffer<ColorType>* buffer) {
		UINT BitMapChannel = sizeof(ColorType);

		HDC hdc = GetDC(g_hwnd);

		BITMAPINFO hBitMap;
		ZeroMemory(&hBitMap, sizeof(BITMAPINFO));
		hBitMap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		hBitMap.bmiHeader.biWidth = WINDOW_WIDTH;
		hBitMap.bmiHeader.biHeight = WINDOW_HEIGHT;
		hBitMap.bmiHeader.biPlanes = 1;
		hBitMap.bmiHeader.biBitCount = 8 * BitMapChannel;
		hBitMap.bmiHeader.biCompression = BI_RGB;
		hBitMap.bmiHeader.biSizeImage = TO_MUL4(WINDOW_WIDTH) * WINDOW_HEIGHT;

		HDC dc = CreateCompatibleDC(hdc);

		void* pImg = NULL;
		HBITMAP bmp = CreateDIBSection(dc, &hBitMap, DIB_RGB_COLORS, &pImg, NULL, 0);

		if (!pImg) {
			ReleaseDC(g_hwnd, dc);
			return;
		}

		for (int y = 0; y != WINDOW_HEIGHT; ++y) {
			char* pLine = static_cast<char*>(pImg) + TO_MUL4(WINDOW_WIDTH * BitMapChannel) * y;
			char* pSrc = (char*)(buffer->pLineAt(y));
			memcpy(pLine, pSrc, WINDOW_WIDTH * BitMapChannel);
		}

		auto oldTmp = SelectObject(dc, bmp);
		SetDIBits(hdc, bmp, 0, WINDOW_HEIGHT, buffer, &hBitMap, DIB_RGB_COLORS);
		//SetDIBitsToDevice(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, WINDOW_HEIGHT, pImg, &hBitMap, SRCCOPY);
		BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, dc, 0, 0, SRCCOPY);
		SelectObject(dc, oldTmp);

		DeleteDC(dc);
		ReleaseDC(g_hwnd, hdc);
		DeleteObject(bmp);
	}
}
#endif