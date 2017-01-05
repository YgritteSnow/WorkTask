#ifndef __BITMAPDISPLAY_H__
#define __BITMAPDISPLAY_H__

#include <windows.h>
#include <cstdio>
#include <wingdi.h>
#include "Color.h"

extern TCHAR* WINDOW_NAME;
extern HWND g_hwnd;

inline
UINT TO_MUL4(UINT a) { return (a & 0x3) ? ( (a & 0xfffffc) + 0x4) : a; }

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
			char* pSrc = (char*)(buffer->pLineAt(y));
			memcpy(pLine, pSrc, buffer->width * BitMapChannel);
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

	template <typename ColorType>
	ImgBuffer<ColorType>* Load(const char* filename) {
		FILE* pf = nullptr;
		fopen_s(&pf, filename, "r");
		if (!pf) {
			return nullptr;
		}

		WORD headSize;
		fread(&headSize, 1, sizeof(WORD), pf);
		if (headSize != 0x4d42) {
			return nullptr;
		}

		//读取bmp文件的文件头和信息头  
		tagBITMAPFILEHEADER temp;
		BITMAPINFOHEADER strInfo;
		fread(&temp, 1, sizeof(tagBITMAPFILEHEADER) - sizeof(WORD), pf);
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), pf);

		// 读取数据
		LONG width = strInfo.biWidth;
		LONG height = strInfo.biHeight;
		//图像每一行的字节数必须是4的整数倍  
		int byteCount = (strInfo.biBitCount / 8);
		//width = (width * byteCount  + 3) / 4 * 4 / byteCount;
		BYTE *imagedata = new BYTE[width * height * byteCount];
		fread(imagedata, height, byteCount*width, pf);

		ImgBuffer<ColorType>* res = new ImgBuffer<ColorType>(strInfo.biWidth, strInfo.biHeight);
		for (int y = 0; y < strInfo.biHeight; ++y) {
			BYTE* linedata = imagedata + (strInfo.biWidth * y * byteCount);
				for (int x = 0; x < strInfo.biWidth; ++x) {
					res->pPixelAt(x, y)->_x = *(linedata + x*byteCount);
					res->pPixelAt(x, y)->_y = *(linedata + x*byteCount +1);
					res->pPixelAt(x, y)->_z = *(linedata + x*byteCount +2);
					res->pPixelAt(x, y)->_w = 1;
				}
		}
		fclose(pf);

		return res;
	}
}
#endif