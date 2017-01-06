#include "BitMap.h"
#include "Texture.h"

namespace BitMap{
	ImgBuffer<ShortColor4>* Load(const char* filename){
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
		fread(&temp, 1, sizeof(tagBITMAPFILEHEADER)-sizeof(WORD), pf);
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), pf);

		// 读取数据
		LONG width = strInfo.biWidth;
		LONG height = strInfo.biHeight;
		//图像每一行的字节数必须是4的整数倍  
		int byteCount = (strInfo.biBitCount / 8);
		int width_byte = ((width * byteCount + 3) / 4) * 4;
		BYTE *imagedata = new BYTE[width_byte * height];
		fread(imagedata, height, width_byte, pf);

		ImgBuffer<ShortColor4>* res = new ImgBuffer<ShortColor4>(strInfo.biWidth, strInfo.biHeight);
		for (int y = 0; y < strInfo.biHeight; ++y) {
			BYTE* linedata = imagedata + (width_byte * y);
			for (int x = 0; x < strInfo.biWidth; ++x) {
				res->pPixelAt(x, y)->_x = *(linedata + x*byteCount);
				res->pPixelAt(x, y)->_y = *(linedata + x*byteCount + 1);
				res->pPixelAt(x, y)->_z = *(linedata + x*byteCount + 2);
				//res->pPixelAt(x, y)->_w = 255;
			}
		}
		fclose(pf);

		return res;
	}
}