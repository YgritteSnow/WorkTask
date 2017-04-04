#include "BitMap.h"
#include "TexBuffer.h"
#include <string.h>
#include <atlimage.h>

#pragma pack(push)
#pragma pack(1)
typedef struct tagTGAFileHead
{
	unsigned char IDLength;        /* 00h  Size of Image ID field */
	unsigned char ColorMapType;    /* 01h  Color map type */
	unsigned char ImageType;       /* 02h  Image type code */
	unsigned short CMapStart;       /* 03h  Color map origin */
	unsigned short CMapLength;      /* 05h  Color map length */
	unsigned char CMapDepth;       /* 07h  Depth of color map entries */
	unsigned short XOffset;         /* 08h  X origin of image */
	unsigned short YOffset;         /* 0Ah  Y origin of image */
	unsigned short Width;           /* 0Ch  Width of image */
	unsigned short Height;          /* 0Eh  Height of image */
	unsigned char PixelDepth;      /* 10h  Image pixel size */
	unsigned char ImageDescriptor; /* 11h  Image descriptor byte */
} TGAFileHead;
#pragma pack(pop)

namespace BitMap{
	TexBuffer<ShortColor4>* Load(const char* filename) {
		FILE* pf = nullptr;
		fopen_s(&pf, filename, "r");
		if (!pf) {
			return nullptr;
		}

		/*unsigned char* dataPtr = new unsigned char[800 * 800 * 4];
		fread(dataPtr, 800 * 800 * 4, 1, pf);
		fseek(pf, 0, SEEK_SET);
		fread(dataPtr, 800 * 800 * 4, 1, pf);
		fseek(pf, 0, SEEK_SET);
		fread(dataPtr, 800 * 800 * 4, 1, pf);
		fseek(pf, 0, SEEK_SET);
		delete[] dataPtr;*/

		TexBuffer<ShortColor4>* res = nullptr;
		std::string s_fn(filename);
		if (s_fn.size() > std::string(".png").size() 
			&& s_fn.find(".png") == s_fn.size() - std::string(".png").size()) {
			res = LoadPNG(pf, filename);
		}
		else if (s_fn.size() > std::string(".bmp").size()
			&& s_fn.find(".bmp") == s_fn.size() - std::string(".bmp").size()) {
			res = LoadBMP(pf, filename);
		}
		else if (s_fn.size() > std::string(".tga").size()
			&& s_fn.find(".tga") == s_fn.size() - std::string(".tga").size()) {
			res = LoadTGA(pf, filename);
		}
		fclose(pf);
		return res;
	}
	TexBuffer<ShortColor4>* LoadBMP(FILE* pf, const char* filename){
		WORD headSize;
		fread(&headSize, 1, sizeof(WORD), pf);
		if (headSize != 0x4d42) {
			return nullptr;
		}

		//读取bmp文件的文件头和信息头  
		tagBITMAPFILEHEADER temp;
		BITMAPINFOHEADER strInfo;
		auto iii = sizeof(tagBITMAPFILEHEADER);
		auto ttt = sizeof(tagBITMAPINFOHEADER);
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

		TexBuffer<ShortColor4>* res = new TexBuffer<ShortColor4>(strInfo.biWidth, strInfo.biHeight);
		for (int y = 0; y < strInfo.biHeight; ++y) {
			BYTE* linedata = imagedata + (width_byte * y);
			for (int x = 0; x < strInfo.biWidth; ++x) {
				res->pPixelAt(x, y)->_z = *(linedata + x*byteCount + 0);
				res->pPixelAt(x, y)->_y = *(linedata + x*byteCount + 1);
				res->pPixelAt(x, y)->_x = *(linedata + x*byteCount + 2);
				res->pPixelAt(x, y)->_w = 255;
			}
		}
		fclose(pf);

		return res;
	}
	TexBuffer<ShortColor4>* LoadPNG(FILE* pf, const char* filename) {
		return nullptr;
	}
	TexBuffer<ShortColor4>* LoadTGA(FILE* pf, const char* filename) {
		TGAFileHead fileHead;
		fread(&fileHead, 1, sizeof(TGAFileHead), pf);
		if (fileHead.ImageType != 2 || // TRUECOLOR
			fileHead.ColorMapType != 0 ||
			fileHead.CMapLength != 0 ||
			fileHead.XOffset != 0 ||
			fileHead.YOffset != 0 ||
			(fileHead.PixelDepth != 24 && fileHead.PixelDepth != 32))
		{
			return nullptr;
		}

		TexBuffer<ShortColor4>* res = new TexBuffer<ShortColor4>(fileHead.Width, fileHead.Height);
		bool has_alpha = fileHead.PixelDepth == 32;
		auto channel = fileHead.PixelDepth / 8;
		auto pitch = TO_MUL4(fileHead.Width * channel);
		//unsigned char* dataPtr = new unsigned char[800 * 800 * 4];
		fseek(pf, 18, SEEK_SET);
		//fread(dataPtr, 800 * 800 * 4, 1, pf);
		unsigned char * dataPtr = new unsigned char[pitch * fileHead.Height];
		fread(dataPtr, sizeof(unsigned char), pitch * fileHead.Height, pf);
		if (has_alpha)
		{
			for (int y = 0; y < fileHead.Height; y++)
			{
				for (int x = 0; x < fileHead.Width; x++)
				{
					auto srcPtr = dataPtr + x * channel;
					auto dstPtr = res->pPixelAt(x, y);
					dstPtr->_z = srcPtr[0];
					dstPtr->_y = srcPtr[1];
					dstPtr->_x = srcPtr[2];
					dstPtr->_w = srcPtr[3];
				}
				dataPtr += pitch;
			}
		}
		else
		{
			for (int y = 0; y < fileHead.Height; y++)
			{
				for (int x = 0; x < fileHead.Width; x++)
				{
					auto srcPtr = dataPtr + y * pitch + x * channel;
					auto dstPtr = res->pPixelAt(x, y);
					dstPtr->_z = srcPtr[0];
					dstPtr->_y = srcPtr[1];
					dstPtr->_x = srcPtr[2];
					dstPtr->_w = 255;
				}
			}
		}
		return res;
	}
}