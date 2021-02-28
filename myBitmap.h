/*
	Module Name:	MyBitmap.h
	
	Version:		4.00

	Author:			Ghassem Shahdoost	
	
	Abstract:		Definition for Image Processing main Class.

*/

#ifndef _MYBITMAP_
#define _MYBITMAP_
#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#include <fstream>
#include <complex>

using namespace std;

#include "MySignal.h"

//Pi  
#define PI    3.141592654
#define PI2   6.283185308

//Gray-Scale Conversion Method 
#define GCM_NONE         0
#define GCM_AVERAGE      1
#define GCM_REALISTIC    2
#define GCM_REDCHANNEL   3
#define GCM_GREENCHANNEL 4
#define GCM_BLUECHANNEL  5

//Get Profile Directions
#define DIR_HORIZONTAL   0
#define DIR_VERTICAL     1

//Interpolations methods
#define INP_NEAR_NEIGHBOUR	0
#define INP_LINER	        1

//Convolutions types

//	1 2 1 
//	2 4 2	/16
//	1 2 1
#define CNV_GAUSSIAN1    0
//	0 1 0 
//	1 2 1	/6
//	0 1 0
#define CNV_GAUSSIAN2    1
#define CNV_LAPLACIAN	 2


//Text Alignments
#define TXT_ALIGNMENT_LEFT		0
#define TXT_ALIGNMENT_RIGHT		1
#define TXT_ALIGNMENT_CENTER	2

//RGB Color
#define CLR_TRANSPARENT	-1
#define CLR_BLACK		0x000000
#define CLR_RED			0x0000FF
#define CLR_GREEN		0x00FF00
#define CLR_BLUE		0xFF0000
#define CLR_YELLOW		0x00FFFF
#define CLR_BROWN		0x008FAF
#define CLR_ORANGE		0x008FFF
#define CLR_WHITE		0xFFFFFF

//DIPWindow Modes
#define WRM_MEAN		0
#define WRM_MINIMUM		1
#define WRM_MAXIMUM		2

//Macros
#define FreeIfAllocated(p)		{if (p) delete[] p; p = NULL;}
#define IsValidRGBColor(c)		(c >= 0 && c <= 0xffffff) 
#define SetPixel24(x,y,r,g,b)   {Rows[y][x + 0] = b; Rows[y][x + 1] = g; Rows[y][x + 2] = r;}	
#define SetPixel32(x,y,r,g,b,a) {Rows[y][x + 0] = b; Rows[y][x + 1] = g; Rows[y][x + 2] = r; Rows[y][x + 3] = a;}
#define SetPixel24c(x,y,c) {Rows[y][x + 0] = GetBValue(c); Rows[y][x + 1] = GetGValue(c); Rows[y][x + 2] = GetRValue(c);}	
#define SetPixel32c(x,y,c) {Rows[y][x + 0] = GetBValue(c); Rows[y][x + 1] = GetGValue(c); Rows[y][x + 2] = GetRValue(c); Rows[y][x + 3] = LOBYTE((rgb)>>24);}

// character sets
#define	CHARACTERS_WIDTH	7
#define	CHARACTERS_HEIGHT	9

const __int64 Chrs8CourierNew[] = 
{
	  0x0000000000000000 ,0x0000200081020408 ,0x0000000000050A14 ,0x001451F1428F8A28 ,0x0008107101808E08 ,0x000020A083C10504 ,0x0000F091A0810C00 ,0x0000000000020408
	 ,0x0020404081020810 ,0x0008104081020204 ,0x0000000002821F08 ,0x0000204087C20408 ,0x0418600000000000 ,0x0000000007C00000 ,0x0000300000000000 ,0x0004102081040820
	 ,0x000071122448911C ,0x0000F8408102040C ,0x0000F8104104111C ,0x000071120308111C ,0x0000E083E2450C10 ,0x000071120381023C ,0x0000711223C08238 ,0x000020410208113E
	 ,0x000071122388911C ,0x000038820788911C ,0x0000300000030000 ,0x000830C000060000 ,0x0000010180C61000 ,0x00000003E00F8000 ,0x00000010C6030100 ,0x000030008104090C
	 ,0x00780992A54C911C ,0x0001DD11C285040C ,0x00007D1223C8911F ,0x000071102040913C ,0x00003C922448890F ,0x0000F9214385123E ,0x000038214385123E ,0x000071172040913C
	 ,0x0001DD1227C89177 ,0x0000F8408102043E ,0x000030912204083C ,0x00019D1121C49177 ,0x0000F9224081020E ,0x0001DD12254D9B77 ,0x0000DD92A5499377 ,0x000071122448911C
	 ,0x00003821C489121E ,0x007071122448911C ,0x00013921C489121E ,0x000079120380992C ,0x000070408102153E ,0x0000711224489177 ,0x000030A142489177 ,0x00005152A54A9177
	 ,0x00018D1141051163 ,0x0000704081051177 ,0x0000F9104104113E ,0x0030204081020418 ,0x0020404081010202 ,0x001820408102040C ,0x0000000000088A08 ,0x0000000000000000
	 ,0x0000000000000404 ,0x0001F913C4470000 ,0x00007D1224C68103 ,0x0000F010244F0000 ,0x0001F112264B1030 ,0x0000F013E4470000 ,0x0000F040810F0430 ,0x1C40F112265B0000
	 ,0x0001DD1224C68103 ,0x0000F84081038008 ,0x0E20408102078008 ,0x0000EC90E24E8103 ,0x0000F8408102040C ,0x0001FD52A545C000 ,0x0001DD1224C6C000 ,0x0000711224470000
	 ,0x0704791224C6C000 ,0x7040F112265B0000 ,0x00007820418D8000 ,0x00007901C04F0000 ,0x000071102047C100 ,0x0001B192244CC000 ,0x000030A1245DC000 ,0x00005152A45DC000
	 ,0x0000D8A0828D8000 ,0x0E1020A1445DC000 ,0x0000F920824F8000 ,0x10102040C1020410 ,0x0010204081020408 ,0x0410204181020404 ,0x00000001A5800000 ,0x0000FD0A142850BF
};

class CMyBitmap 
{
private:

	int CMyBitmap::round(double x) 
	{
		x += x > 0.0 ? 0.5 : -0.5; 
		return int(x);
		
	}

	void CMyBitmap::UpdateRows()
	{
		FreeIfAllocated(Rows);
		Rows = new BYTE* [Height];
		for (int i = 0; i < Height; i++)
			Rows[i] = Bits + i * BytesPerRow;
	};

	bool CMyBitmap::WrongDimensions(CMyBitmap* bmp)
	{
		if (bmp->Width <= 0 || bmp->Height <=0 || !(bmp->BPP == 8 || bmp->BPP == 24 || bmp->BPP == 32))
			return true;
		else
			return false;
	}
	bool CMyBitmap::CorrectWindow(int& left, int& top, int& right, int& bottom)
	{
		left = min(Width - 1, max(0, left));
		top = min(Height - 1, max(0, top));
		right = min(Width - 1, max(0, right));
		bottom = min(Height - 1, max(0, bottom));
		return left < right && top < bottom;
	}
	void CMyBitmap::UpdateBitmapInfo()
	{
		if (BPP == 8)
		{
			// set pallete
			FreeIfAllocated(BitmapInfo);
			BitmapInfo = (BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
			Palette = (RGBQUAD*) ((char*)BitmapInfo + sizeof(BITMAPINFOHEADER));
			for (int i = 0; i < 256; i++)
			{
				RGBQUAD c;
				c.rgbBlue = i;
				c.rgbGreen = i;
				c.rgbRed = i;
				c.rgbReserved = 0;
				*(Palette + i) = c;
			}
		}
		else
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
			Palette = NULL;
		}

		// create header
		BitmapInfo->bmiHeader.biBitCount = BPP;
		BitmapInfo->bmiHeader.biClrImportant = 0;
		BitmapInfo->bmiHeader.biClrUsed = (BPP == 8) ? 256 : 0;
		BitmapInfo->bmiHeader.biCompression = 0;
		BitmapInfo->bmiHeader.biHeight = -Height;
		BitmapInfo->bmiHeader.biPlanes = 1;
		BitmapInfo->bmiHeader.biSize = 40;
		BitmapInfo->bmiHeader.biSizeImage = BytesPerRow * Height;
		BitmapInfo->bmiHeader.biWidth = Width;
		BitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		BitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	}
	void CMyBitmap::CleanAddress()
	{
		Bits = NULL;
		Rows = NULL;
		BitmapInfo = NULL; 
	}
	bool CMyBitmap::FloodFill(int x, int y, BYTE newColor,int* piArea = NULL)
	{
		const int STACK_SIZE = 1024;
		int iStackPointer = 0;
		int iArea = 0;
		int pStack[STACK_SIZE];

		BYTE iOldColor = Rows[y][x];
		int x1; 

		bool spanLeft, spanRight;
	    
		iStackPointer++; 
		pStack[iStackPointer] = (x << 16) + y; 

		while(iStackPointer > 0) 
		{   
			int p = pStack[iStackPointer]; 
			x = p >> 16;
			y = p & 0xffff;
			iStackPointer--; 

			x1 = x;
			while(x1 >= 0 && Rows[y][x1] == iOldColor)
				x1--;

			x1++;
			spanLeft = spanRight = 0;

			while(x1 < Width && Rows[y][x1] == iOldColor)
			{
				Rows[y][x1] = newColor;
				iArea++;
				if(!spanLeft && y > 0 && Rows[y - 1][x1] == iOldColor) 
				{
					if(iStackPointer < STACK_SIZE - 1) 
					{ 
						iStackPointer++; 
						pStack[iStackPointer] =  (x1 << 16) + (y - 1); 
					}
					else
						return 0;
					spanLeft = 1;
				}
				else if(spanLeft && y > 0 && Rows[y - 1][x1] != iOldColor)
				{
					spanLeft = 0;
				}
				if(!spanRight && y < Height - 1 && Rows[y + 1][x1] == iOldColor) 
				{
					if(iStackPointer < STACK_SIZE - 1) 
					{ 
						iStackPointer++; 
						pStack[iStackPointer] =  (x1 << 16) + y + 1;
					}
					else
						return 0;
					spanRight = 1;
				}
				else if(spanRight && y < Height - 1 && Rows[y + 1][x1] != iOldColor)
				{
					spanRight = 0;
				} 
				x1++;
			}
		}
		if (piArea)
			*piArea = iArea;
		return 1;

	}
public:
	int			Width, Height;	// Dimensions
	int			BPP;			// Bits Per Pixel.
	BYTE*		Bits;			// Bits of the Image.
	RGBQUAD*	Palette;		// RGB Palette for the image. (RGB Palette is a part of pbmi and 'Palette' is the pointer of start of Palette)
	int			BytesPerRow;	// Row Width (in bytes).
	BITMAPINFO* BitmapInfo;		// BITMAPINFO structure
	BYTE**		Rows;			// Address of each row on bitmap
	
	CMyBitmap::CMyBitmap()
	{
		Height = Width = BPP = BytesPerRow =  0;
		Bits = NULL;
		BitmapInfo = NULL;
		Rows = NULL;
	};

	CMyBitmap::CMyBitmap(char* szFile)
	{
		Height = Width = BPP = BytesPerRow =  0;
		Bits = NULL;
		BitmapInfo = NULL;
		Rows = NULL;
		this->BMPLoad(szFile);
	};

	CMyBitmap::CMyBitmap(int iWidth, int iHeight, int bpp)
	{
		Height = Width = BPP = BytesPerRow =  0;
		Bits = NULL;
		BitmapInfo = NULL;
		Rows = NULL;
		this->BMPNew(iWidth, iHeight, bpp);
	};

	CMyBitmap::CMyBitmap(CMyBitmap& bmpSource)

	{
		Height = Width = BPP = BytesPerRow =  0;
		Bits = NULL;
		BitmapInfo = NULL;
		Rows = NULL;
		this->BMPAssign(bmpSource);
	};
	CMyBitmap::~CMyBitmap()
	{
		FreeIfAllocated (Bits);
		FreeIfAllocated (Rows);
		FreeIfAllocated (BitmapInfo);
	};


	// ***Bitmap Accessories***
	int CMyBitmap::BMPLoad (char * szFile)
	{
		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER bmih;

		// Open file.
		ifstream bmpfile (szFile, ios::in | ios::binary);
		
		if (! bmpfile.is_open()) 
			return 0;		// Error opening file

		// Load bitmap fileheader & infoheader
		bmpfile.read ((char*)&bmfh, sizeof(BITMAPFILEHEADER));
		bmpfile.read ((char*)&bmih, sizeof(BITMAPINFOHEADER));

		// Check filetype signature
		if (bmfh.bfType != 'MB') 
			return 0;		// File is not BMP

		// Assign some short variables:
		BPP = bmih.biBitCount;
		Width = bmih.biWidth;
		Height = (bmih.biHeight > 0) ? bmih.biHeight : -bmih.biHeight; // absoulte value
		BytesPerRow = Width * BPP / 8;
		BytesPerRow += (4 - BytesPerRow % 4) % 4;	// int alignment

		// If BPP aren't 24, load Palette:
		if (BPP == 24) 
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo = (BITMAPINFO*)new char [sizeof(BITMAPINFO)];
		}
		else
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo = (BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER) + (1 << BPP) * sizeof(RGBQUAD)];
			Palette = (RGBQUAD*)((char*)BitmapInfo + sizeof(BITMAPINFOHEADER));
			bmpfile.read ((char*)Palette, sizeof(RGBQUAD) * (1 << BPP));
		}
		BitmapInfo->bmiHeader = bmih;

		// Load bitmap
		bmpfile.seekg (bmfh.bfOffBits, ios::beg);

		FreeIfAllocated(Bits);
		Bits = new BYTE[BytesPerRow * Height];

		UpdateRows();

		// (if iHeight is positive the bmp is bottom-up, read it reversed)
		if (bmih.biHeight > 0)
			for (int n = Height - 1; n >= 0; n--)
				bmpfile.read ((char*)(Bits + BytesPerRow * n), BytesPerRow);
		else
			bmpfile.read ((char*)Bits, BytesPerRow * Height);

		// so, we always have a up-bottom raster (that is negative iHeight for windows):
		UpdateBitmapInfo();
		bmpfile.close();
		
		return 1;
	};


	int CMyBitmap::BMPSave (char * szFile)
	{
		BITMAPFILEHEADER bmfh;

		bmfh.bfType = 'MB';
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = BitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);//  1078;//1078
		bmfh.bfSize = bmfh.bfOffBits + BitmapInfo->bmiHeader.biSizeImage;//  FileSize
		
		ofstream bmpfile (szFile, ios::out | ios::binary);

		//store file header
		bmpfile.write ((char*)&bmfh, sizeof (BITMAPFILEHEADER));
		//store bitmap header
		bmpfile.write ((char*)&BitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER));
		//store bitmap palette
		if (BitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD)) 
			bmpfile.write ((char*)Palette, BitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD));
		//store data bits
		bmpfile.write ((char*)Bits, BitmapInfo->bmiHeader.biSizeImage);
		
		bmpfile.close();						  
		
		return 1;
	};

	int CMyBitmap::BMPNew(int iWidth, int iHeight, int bpp)
	{
		if (iWidth <= 0 || iHeight <=0 || !(bpp == 8 || bpp == 24 || bpp == 32))
			return 0;

		Width = iWidth;
		Height = iHeight;
		BPP = bpp;
		BytesPerRow = Width * BPP / 8;
		BytesPerRow += (4 - BytesPerRow % 4) % 4;
		FreeIfAllocated(Bits);
		Bits = new BYTE[BytesPerRow * Height];
		UpdateRows();
		UpdateBitmapInfo();

		return 1;
	};

	int CMyBitmap::BMPFill(int iColor)
	{
		if (WrongDimensions(this))
			return 0;

		if (BPP == 8)
		{
			for (int y = 0; y < Height; y++)
				for (int x = 0; x < Width; x++)
					Rows[y][x] = (BYTE)iColor;
			return 1;
		}
		else if (BPP == 24)
		{
			for (int y = 0; y < Height; y++)
				for (int x = 0; x < Width * 3; x += 3)
				{
					Rows[y][x + 0] = GetBValue(iColor);
					Rows[y][x + 1] = GetGValue(iColor);
					Rows[y][x + 2] = GetRValue(iColor);
				}
		}
		else if (BPP == 32)
		{
			for (int y = 0; y < Height; y++)
				for (int x = 0; x < Width * 4 ;x += 4)
				{
					Rows[y][x + 0] = GetBValue(iColor);
					Rows[y][x + 1] = GetGValue(iColor);
					Rows[y][x + 2] = GetRValue(iColor);
					Rows[y][x + 3] = 0;
				}
		}
		return 1;
	}

	int CMyBitmap::BMPAssign (CMyBitmap& Source)
	{
		if (WrongDimensions(&Source))
			return 0;
		Width = Source.Width;
		Height = Source.Height;
		BPP = Source.BPP ;
		BytesPerRow = Source.BytesPerRow;

		int bitssize = BytesPerRow * Height;
		FreeIfAllocated(Bits);
		Bits = new BYTE[bitssize];
		UpdateRows();
		memcpy(Bits,Source.Bits,bitssize);

		if (Source.BPP == 8)
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo=(BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
			Palette=(RGBQUAD*) ((char*)BitmapInfo+sizeof(BITMAPINFOHEADER));
			memcpy(Palette,Source.Palette,256*sizeof(RGBQUAD));
		}
		else
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
		}
		// copy bitmap information
		memcpy(&(BitmapInfo->bmiHeader),  &(Source.BitmapInfo->bmiHeader),  sizeof(tagBITMAPINFOHEADER));

		return 1;
	};
	
	
	int CMyBitmap::BMPReadFromStream(int width, int height, int bpp, BYTE * bits, bool upside_down = false)
	{
		if (width <= 0 || height <= 0 || !(bpp == 8 || bpp == 24 || bpp == 32) || bits == NULL)
			return 0;
		if(Width != width || Height != height || BPP != bpp)
		{
			Width = width;
			Height = height;
			BPP = bpp;
			BytesPerRow = Width * BPP / 8;
			BytesPerRow += (4 - BytesPerRow % 4) % 4;	// int alignment
			int bitssize = BytesPerRow * Height;
			FreeIfAllocated(Bits);
			Bits = new BYTE[bitssize];
			UpdateRows();
		}
		if(upside_down)
		{
			for (int n = Height - 1; n >= 0; n--)
				memcpy_s((char*)(Bits + BytesPerRow * (Height - 1 - n)), width * bpp / 8, (char*)(bits + width * n * bpp / 8), width * bpp / 8);
		}
		else
		{
			for (int n = Height - 1; n >= 0; n--)
				memcpy_s((char*)(Bits + BytesPerRow * n), width * bpp / 8, (char*)(bits + width * n * bpp / 8), width * bpp / 8);
		}
		UpdateBitmapInfo();

		return 1;

	}

	int CMyBitmap::BMPWriteToStream(int& width, int& height, int& bpp, BYTE * bits, bool upside_down = false)
	{
		width = Width;
		height = Height;
		bpp = BPP;


		return BMPWriteToStream(bits, upside_down);
	}

	int CMyBitmap::BMPWriteToStream(BYTE * bits, bool upside_down = false)
	{
		if (bits == NULL)
			return 0;

		if(upside_down)
		{
			for (int n = Height - 1; n >= 0; n--)
				memcpy_s((char*)(bits + Width * n * BPP / 8), Width * BPP / 8, (char*)(Bits + BytesPerRow * (Height - 1 - n)), Width * BPP / 8);
		}
		else
		{
			for (int n = Height - 1; n >= 0; n--)
				memcpy_s((char*)(bits + Width * n * BPP / 8), Width * BPP / 8, (char*)(Bits + BytesPerRow * n), Width * BPP / 8);
		}

		return 1;
	}



/*
	int CMyBitmap::BMPAssign(int width, int height, int bpp, BYTE * bits)
	{
		if (width <= 0 || height <= 0 || !(bpp == 8 || bpp == 24 || bpp == 32) || bits == NULL)
			return 0;

		Width = width;
		Height = height;
		BPP = bpp;
		BytesPerRow = Width * BPP / 8;
		BytesPerRow += (4 - BytesPerRow % 4) % 4;	// int alignment
		int bitssize = BytesPerRow * Height;
		FreeIfAllocated(Bits);
		Bits = new BYTE[bitssize];
		UpdateRows();

		for (int n = Height - 1; n >= 0; n--)
			memcpy_s((char*)(Bits + BytesPerRow * n), width * bpp / 8, (char*)(bits + width * n * bpp / 8), width * bpp / 8);

		UpdateBitmapInfo();

		return 1;
	}

	int CMyBitmap::BMPCopyBitsTo(BYTE * bits)
	{

		for (int n = Height - 1; n >= 0; n--)
			memcpy_s((char*)(bits + Width * n * BPP / 8), Width * BPP / 8, (char*)(Bits + BytesPerRow * n), Width * BPP / 8);


		return 1;
	}
*/

	int CMyBitmap::BMPAdjustment(int contrast = 100, int Brightness = 0)
	{
		for(int i = 0; i < Height * BytesPerRow; i++)
		{
			int c = (Bits[i] * contrast) / 100 + Brightness;
			Bits[i] = max(0, min(255, c));
		}
		return 1;
	}

	int CMyBitmap::BMPConvertTo8Bit(UINT gcmMethod ,CMyBitmap* bmpResult = NULL)
	{
		if (WrongDimensions(this))
			return 0;

		if (BPP == 8)
			return 1;

		int iNewBPP = 8;
		int iNewBytesPerRow = Width * iNewBPP / 8;
		iNewBytesPerRow += (4 - iNewBytesPerRow % 4) % 4;
	
		BYTE* pNewBits= new BYTE[iNewBytesPerRow * Height];
		int iBytePP = BPP / 8;

		for (int y = 0; y < Height; y++)
		{
			int yy = y * iNewBytesPerRow;		
			int xrgb = 0;

			for (int x = 0; x < Width; x++)
			{
				BYTE b = Rows[y][xrgb + 0];
				BYTE g = Rows[y][xrgb + 1];
				BYTE r = Rows[y][xrgb + 2];
				int c = 0;
				switch (gcmMethod)
				{
					case GCM_NONE:
						c = r;
						break;
					case GCM_AVERAGE:
						c = (r + g + b) / 3 ;
						break;
					case GCM_REALISTIC:
						c = (r * 30 + g * 59 + b * 11) / 100;
						break;
					case GCM_REDCHANNEL:
						c = r;
						break;
					case GCM_GREENCHANNEL:
						c = g;
						break;
					case GCM_BLUECHANNEL:
						c = b;
						break;
					default:
						c = 0;
				}

				pNewBits[yy + x] = (BYTE)c;
				xrgb += iBytePP;

			}
		}

		//replace old image with new 24Bis
		if (bmpResult)
		{//result on bmpResult
			delete[] bmpResult->Bits;
			bmpResult->Height = Height;
			bmpResult->Width = Width;
			bmpResult->Bits = pNewBits;
			bmpResult->BytesPerRow = iNewBytesPerRow;
			bmpResult->BPP = iNewBPP;
			bmpResult->UpdateRows();
			bmpResult->UpdateBitmapInfo();			
		}
		else
		{// result on self
			delete[] Bits;
			Bits = pNewBits;
			BytesPerRow = iNewBytesPerRow;
			BPP = iNewBPP;
			UpdateRows();
			UpdateBitmapInfo();
		}
		return 1;
	};
	int CMyBitmap::BMPConvertTo24Bit(CMyBitmap* bmpResult = NULL)
	{
		if (WrongDimensions(this))
			return 0;

		if (BPP == 24)
			return 1;

		int iNewBPP = 24;
		int iNewBytesPerRow = Width * iNewBPP / 8;
		iNewBytesPerRow += (4 - iNewBytesPerRow % 4) % 4;	
		BYTE* pNewBits= new BYTE[iNewBytesPerRow * Height];
		
		if (BPP == 8)
			for (int y = 0; y < Height; y++)
			{
				int yy = y * iNewBytesPerRow;		
				int x24 = 0;
				for (int x = 0; x < Width; x++)
				{
					BYTE c = Rows[y][x];
					pNewBits[yy + x24 + 0] = c;
					pNewBits[yy + x24 + 1] = c;
					pNewBits[yy + x24 + 2] = c;
					x24 += 3;
				}
			}
		else // bpp == 32
			for (int y = 0; y < Height; y++)
			{
				int yy = y * iNewBytesPerRow;		
				int x24 = 0;
				int x32 = 0;
				for (int x = 0; x < Width; x++)
				{
					BYTE b = Rows[y][x32 + 0];
					BYTE g = Rows[y][x32 + 1];
					BYTE r = Rows[y][x32 + 2];
					pNewBits[yy + x24 + 0] = b;
					pNewBits[yy + x24 + 1] = g;
					pNewBits[yy + x24 + 2] = r;
					x24 += 3;
					x32 += 4;
				}
			}
		//replace old image with new 24Bis
		if (bmpResult)
		{//result on bmpResult
			delete[] bmpResult->Bits;
			bmpResult->Height = Height;
			bmpResult->Width = Width;
			bmpResult->Bits = pNewBits;
			bmpResult->BytesPerRow = iNewBytesPerRow;
			bmpResult->BPP = iNewBPP;
			bmpResult->UpdateRows();
			bmpResult->UpdateBitmapInfo();			
		}
		else
		{// result on self
			delete[] Bits;
			Bits = pNewBits;
			BytesPerRow = iNewBytesPerRow;
			BPP = iNewBPP;
			UpdateRows();
			UpdateBitmapInfo();
		}
		return 1;
	};
	int CMyBitmap::BMPConvertTo32Bit(CMyBitmap* bmpResult = NULL)
	{
		if (WrongDimensions(this))
			return 0;

		if (BPP == 32)
		{
			if (bmpResult)
				BMPAssign(*bmpResult);
			return 1;
		}

		int iNewBPP = 32;
		int iNewBytesPerRow = Width * iNewBPP / 8;
		iNewBytesPerRow += (4 - iNewBytesPerRow % 4) % 4;	
		BYTE* pNewBits= new BYTE[iNewBytesPerRow * Height];
		
		if (BPP == 8)
			for (int y = 0; y < Height; y++)
			{
				int yy = y * iNewBytesPerRow;		
				int x32 = 0;
				for (int x = 0; x < Width; x++)
				{
					BYTE c = Rows[y][x];
					pNewBits[yy + x32 + 0] = c;
					pNewBits[yy + x32 + 1] = c;
					pNewBits[yy + x32 + 2] = c;
					pNewBits[yy + x32 + 3] = 0;
					x32 += 4;
				}
			}
		else // bpp == 24
			for (int y = 0; y < Height; y++)
			{
				int yy = y * iNewBytesPerRow;		
				int x24 = 0;
				int x32 = 0;
				for (int x = 0; x < Width; x++)
				{
					BYTE b = Rows[y][x24 + 0];
					BYTE g = Rows[y][x24 + 1];
					BYTE r = Rows[y][x24 + 2];
					pNewBits[yy + x32 + 0] = b;
					pNewBits[yy + x32 + 1] = g;
					pNewBits[yy + x32 + 2] = r;
					pNewBits[yy + x32 + 3] = 0;
					x24 += 3;
					x32 += 4;
				}
			}
		//replace old image with new 24Bis
		if (bmpResult)
		{//result on bmpResult
			delete[] bmpResult->Bits;
			bmpResult->Height = Height;
			bmpResult->Width = Width;
			bmpResult->Bits = pNewBits;
			bmpResult->BytesPerRow = iNewBytesPerRow;
			bmpResult->BPP = iNewBPP;
			bmpResult->UpdateRows();
			bmpResult->UpdateBitmapInfo();			
		}
		else
		{// result on self
			delete[] Bits;
			Bits = pNewBits;
			BytesPerRow = iNewBytesPerRow;
			BPP = iNewBPP;
			UpdateRows();
			UpdateBitmapInfo();
		}
		return 1;
	};

	int CMyBitmap::BMPResize(int iNewWidth, int iNewHeight, UINT inpInterpolationMethod, CMyBitmap* bmpResult = NULL)
	{
		if (WrongDimensions(this) || iNewWidth <= 0 || iNewHeight <= 0 || !(inpInterpolationMethod == INP_NEAR_NEIGHBOUR || inpInterpolationMethod == INP_LINER)) 
			return 0;
		
		CMySignal piReplacementTable;
		
		CMySignal piCountTable;
		CMySignal piBaseTable;
		CMySignal piIntermedeiteRow;
		
		CMyBitmap* pSelBMP = this;

		int iBytePP = BPP / 8;
		int iNewBytesPerRow = iNewWidth * BPP / 8;
		iNewBytesPerRow += (4 - iNewBytesPerRow % 4) % 4;

		BYTE* pNewBits = new BYTE [iNewBytesPerRow * Height];	
		
		if(bmpResult)
			pSelBMP = bmpResult;
		//               *******  HORIZONTAL RESIZING 
		
		if (Width >= iNewWidth)
		{//shrink image horizontally
			piReplacementTable.SGNResize(Width);
			piCountTable.SGNResize(iNewWidth, true);
			piIntermedeiteRow.SGNResize(iNewBytesPerRow);
			// fill ReplacementTable
			for(int i = 0; i < Width; i++)
			{
				int c = (i * iNewWidth) / Width;
				piReplacementTable.Bits[i] = c;
				piCountTable.Bits[c]++;
			}
			
			for (int y = 0; y < Height; y++)
			{
				int yy = y * iNewBytesPerRow;
				piIntermedeiteRow.SGNFill(0);	
				if (BPP == 8)
				{
					for (int x = 0; x < Width; x++)
						piIntermedeiteRow.Bits[piReplacementTable.Bits[x]] += Rows[y][x]; 
					for (int x = 0; x < iNewWidth; x++)
						pNewBits[yy + x] = (BYTE)(piIntermedeiteRow.Bits[x] / piCountTable.Bits[x]);
				}
				else// 24Bit or 32Bit
				{
					int px = 0;
					for (int x = 0; x < Width; x++)
					{
						int sx = piReplacementTable.Bits[x] * iBytePP;
						piIntermedeiteRow.Bits[sx + 0] += Rows[y][px + 0]; 
						piIntermedeiteRow.Bits[sx + 1] += Rows[y][px + 1]; 
						piIntermedeiteRow.Bits[sx + 2] += Rows[y][px + 2];
						px += iBytePP;
					}
					px = 0;
					for (int x = 0; x < iNewWidth; x++)
					{
						pNewBits[yy + px + 0] = (BYTE)(piIntermedeiteRow.Bits[px + 0] / piCountTable.Bits[x]);
						pNewBits[yy + px + 1] = (BYTE)(piIntermedeiteRow.Bits[px + 1] / piCountTable.Bits[x]);
						pNewBits[yy + px + 2] = (BYTE)(piIntermedeiteRow.Bits[px + 2] / piCountTable.Bits[x]);
						px += iBytePP;
					}
				}
			}
		}
		else
		{//expand image horizontally
			
			if(inpInterpolationMethod == INP_NEAR_NEIGHBOUR)
			{
				piReplacementTable.SGNResize(iNewWidth);
				// fill ReplacementTable
				for(int i = 0; i < iNewWidth; i++)
				{
					int c = (i * Width) / iNewWidth;
					piReplacementTable.Bits[i] = c;
				}
				for (int y = 0; y < Height; y++)
				{
					int yy = y * iNewBytesPerRow;
					if (BPP == 8)
					{
						for (int x = 0; x < iNewWidth; x++)
							pNewBits[yy + x] = Rows[y][piReplacementTable.Bits[x]];
					} 
					else if(BPP == 24)
					{
						for (int x = 0; x < iNewWidth; x++)
						{
							pNewBits[yy + x * 3 + 0] = Rows[y][piReplacementTable.Bits[x] * 3 + 0];
							pNewBits[yy + x * 3 + 1] = Rows[y][piReplacementTable.Bits[x] * 3 + 1];
							pNewBits[yy + x * 3 + 2] = Rows[y][piReplacementTable.Bits[x] * 3 + 2];
						}

					}//TODO: image expand for color images
				}
			}
			else if(inpInterpolationMethod == INP_LINER)
			{
				int hw = iNewWidth / (Width * 2);

				piReplacementTable.SGNResize(iNewWidth);
				piCountTable.SGNResize(iNewWidth);
				piBaseTable.SGNResize(iNewWidth);
				// fill ReplacementTable by initial data 
				for(int i = 0, s = i + hw, l = 0; i < iNewWidth - hw - 1; i++)
				{
					int c = (i * Width) / iNewWidth;
					piReplacementTable.Bits[i + hw] = c;
					if(c != l)
					{
						l = c;
						s = i + hw;
					}
					piBaseTable.Bits[i + hw] = s;
				}			

				for(int i = hw; i < iNewWidth - hw - 1;)
				{
					int l = piReplacementTable.Bits[i];
					int j = 0;
					while(l == piReplacementTable.Bits[j + i])
						j++;
					for(int k = i; k < i + j; k++)
						piCountTable.Bits[k] = j;
					i += j;
				}

				for(int y = 0; y < Height; y++)
				{
					int yy = y * iNewBytesPerRow;
					for (int x = hw; x < iNewWidth - hw - 1; x++)
					{
						int v1 = Rows[y][piReplacementTable.Bits[x]];
						int v2 = Rows[y][piReplacementTable.Bits[x] + 1];
						pNewBits[yy + x] = v1 + (((x - piBaseTable.Bits[x] ) * (v2 - v1)) / piCountTable.Bits[x]) ;
					}
					for(int i = 0; i <= hw; i++)
					{
						pNewBits[yy + i] = Rows[y][0];
						pNewBits[yy + iNewWidth - i - 1] = Rows[y][Width - 1];
					}

				}
			}

		}

		FreeIfAllocated(pSelBMP->Bits);
		pSelBMP->Bits = new BYTE[iNewBytesPerRow * iNewHeight];    


		//	*******  VERTICALLY RESIZING 
		if (Height >= iNewHeight)
		{             // image shrink vertically
			piReplacementTable.SGNResize(Height);
			piCountTable.SGNResize(iNewHeight, true);
			piIntermedeiteRow.SGNResize(iNewHeight);
			
			for(int i = 0; i < Height; i++)
			{
				int c = (i * iNewHeight) / Height;
				piReplacementTable.Bits[i] = c;
				piCountTable.Bits[c]++;
			}

			for (int x = 0; x < iNewBytesPerRow; x++)
			{
				piIntermedeiteRow.SGNFill(0);
				int yy = 0;
				for (int y = 0; y < Height; y++)
				{
					piIntermedeiteRow.Bits[piReplacementTable.Bits[y]] += pNewBits[yy + x];
					yy += iNewBytesPerRow;
				}
				yy = 0;
				for (int y = 0; y < iNewHeight; y++)
				{
					pSelBMP->Bits[yy + x] = (BYTE)(piIntermedeiteRow.Bits[y] / piCountTable.Bits[y]);
					yy += iNewBytesPerRow;
				}
			} 		
		}
		else
		{//expand image vertically
			if(inpInterpolationMethod == INP_NEAR_NEIGHBOUR)
			{
				piReplacementTable.SGNResize(iNewHeight);
				for(int i = 0; i < iNewHeight; i++)
				{
					int c = (i * Height) / iNewHeight;
					piReplacementTable.Bits[i] = c;
				}
				
				for (int x = 0; x < iNewWidth; x++)
				{
					if (BPP == 8)
					{
						int yy = 0;
						for (int y = 0; y < iNewHeight; y++)
						{
							pSelBMP->Bits[yy + x] = pNewBits[piReplacementTable.Bits[y] * iNewBytesPerRow + x];
							yy += iNewBytesPerRow;
						}
					}
					else if(BPP == 24)
					{
						int yy = 0;
						for (int y = 0; y < iNewHeight; y++)
						{
							pSelBMP->Bits[yy + x * 3 + 0] = pNewBits[piReplacementTable.Bits[y] * iNewBytesPerRow + x * 3 + 0];
							pSelBMP->Bits[yy + x * 3 + 1] = pNewBits[piReplacementTable.Bits[y] * iNewBytesPerRow + x * 3 + 1];
							pSelBMP->Bits[yy + x * 3 + 2] = pNewBits[piReplacementTable.Bits[y] * iNewBytesPerRow + x * 3 + 2];
							yy += iNewBytesPerRow;
						}
					}

				}
			}
			else if(inpInterpolationMethod == INP_LINER)
			{
				int hw = iNewHeight / (Height * 2);

				piReplacementTable.SGNResize(iNewHeight);
				piCountTable.SGNResize(iNewHeight);
				piBaseTable.SGNResize(iNewHeight);
				// fill ReplacementTable by initial data 
				for(int i = 0, s = i + hw, l = 0; i < iNewHeight - hw - 1; i++)
				{
					int c = (i * Height) / iNewHeight;
					piReplacementTable.Bits[i + hw] = c;
					if(c != l)
					{
						l = c;
						s = i + hw;
					}
					piBaseTable.Bits[i + hw] = s;
				}			

				for(int i = hw; i < iNewHeight - hw - 1;)
				{
					int l = piReplacementTable.Bits[i];
					int j = 0;
					while(l == piReplacementTable.Bits[j + i])
						j++;
					for(int k = i; k < i + j; k++)
						piCountTable.Bits[k] = j;
					i += j;
				}

				for (int x = 0; x < iNewWidth; x++)
				{
					int yy = hw * iNewBytesPerRow;	
					for (int y = hw; y < iNewHeight - hw - 1; y++)
					{
						int v1 = pNewBits[piReplacementTable.Bits[y] * iNewBytesPerRow + x];
						int v2 = pNewBits[(piReplacementTable.Bits[y] + 1) * iNewBytesPerRow + x];
						pSelBMP->Bits[yy + x] = v1 + (((y - piBaseTable.Bits[y]) * (v2 - v1)) / piCountTable.Bits[y]);
						yy += iNewBytesPerRow;
					}
					for(int i = 0; i <= hw; i++)
					{
						pSelBMP->Bits[i * iNewBytesPerRow + x] = pNewBits[x];
						pSelBMP->Bits[(iNewHeight - i - 1) * iNewBytesPerRow + x] = pNewBits[(Height - 1) * iNewBytesPerRow + x];
					}
				}

			}
		}

		pSelBMP->Width = iNewWidth;
		pSelBMP->Height = iNewHeight;
		pSelBMP->BytesPerRow = pSelBMP->Width * pSelBMP->BPP / 8;
		pSelBMP->BytesPerRow += (4 - pSelBMP->BytesPerRow % 4) % 4;	
		pSelBMP->UpdateRows();
		pSelBMP->UpdateBitmapInfo();
		delete[] pNewBits;
		return 1;
	};

	int CMyBitmap::BMPCrop(RECT iWin, CMyBitmap& bmpResult)
	{
		return BMPCrop(iWin.left ,iWin.top ,iWin.right ,iWin.bottom ,bmpResult);
	}

	int CMyBitmap::BMPCrop(int iWinLeft, int iWinTop, int iWinRight, int iWinBottom, CMyBitmap& bmpResult)
	{
		if(!CorrectWindow(iWinLeft, iWinTop, iWinRight, iWinBottom) || WrongDimensions(this))
			return 0;
		int iNewWidth = iWinRight - iWinLeft;
		int iNewHeight = iWinBottom - iWinTop;
		int iBytePP = BPP / 8;
		if(iNewWidth != bmpResult.Width || iNewHeight != bmpResult.Height || BPP != bmpResult.BPP )
			bmpResult.BMPNew(iNewWidth, iNewHeight, BPP);

		for(int y = 0; y < iNewHeight; y++)
			for(int x = 0; x < iNewWidth * iBytePP; x++)
				bmpResult.Rows[y][x] = Rows[y + iWinTop][x + iWinLeft * iBytePP];
		return 1;
	}

	int CMyBitmap::BMPFlipVertically(CMyBitmap* bmpResult = NULL)
	{
		
		if(bmpResult)
		{
			bmpResult->BMPNew(Width, Height, BPP);
			for(int i = 0; i < Height; i++)
			{
				memcpy(bmpResult->Bits + (BytesPerRow * (Height - i - 1)), Bits + (BytesPerRow * i), BytesPerRow);
			}
		}
		else
		{
			void * swapBuf = malloc(BytesPerRow); 
			for(int i = 0; i < (Height / 2); i++)
			{
				memcpy(swapBuf, Bits + (BytesPerRow * i), BytesPerRow);
				memcpy(Bits + (BytesPerRow * i), Bits + (BytesPerRow * (Height - i - 1)), BytesPerRow);
				memcpy(Bits + (BytesPerRow * (Height - i - 1)), swapBuf, BytesPerRow);
			}
			free(swapBuf);
		}

		return 1;
	}


	// * Paints CMyBitmap to a Windows DC.
	int CMyBitmap::GDIPaint (HDC hdc,int x = 0,int y = 0, bool frame = false)
	{
		// Paint the image to the device.
		if(Width <= 0 || Height <= 0)
			return 0;
		if(frame)
			Rectangle(hdc, x - 1, y - 1, x + Width + 1,y + Height + 1);
		return !SetDIBitsToDevice (hdc, x, y, Width, Height, 0, 0, 0, Height, (LPVOID)Bits, BitmapInfo, 0);
	};

	int CMyBitmap::GDIPaint (HDC hdc, int x ,int y , int wx1, int wy1,int wx2, int wy2,  bool frame = false)
	{
		// Paint the image to the device.
		int w = wx2 - wx1, h = wy2 - wy1;
		if(w <= 0 || h <= 0 || wx1 < 0 || wy1 < 0)
			return 0;
		if(frame)
			Rectangle(hdc, x - 1, y - 1, x + w + 1,y + h + 1);
		h = min(h, Height);
		w = min(w, Width);
		return !SetDIBitsToDevice (hdc, x, y, w, h, wx1, wy1, 0, Height, (LPVOID)Bits, BitmapInfo, 0);
	};

	int CMyBitmap::GDIReadFromHandle(HBITMAP handle)
	{
		if(handle)
		{
			BITMAP bmp;
			GetObject(handle, sizeof(BITMAP),(BITMAP*)&bmp);
			this->BMPNew(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);
			GetBitmapBits(handle,Height * BytesPerRow, Bits);
			if((Width * BPP / 8) % 4)	// alinement
			{
				BYTE * NewBits = new BYTE[BytesPerRow * Height];
				int w = Width * BPP / 8;
				int wi = w + (Width % 2); //note: in practice this alinement is necessary but i dont know the reason 
				for(int i = 0; i < Height; i++)
					memcpy_s(NewBits + (i * BytesPerRow), w, Bits + (i * wi) , w);
				delete Bits;
				Bits = NewBits;
				UpdateRows();

			}
		}
		else
			  return 0;
		return 1;
	};


	
	// ***DIGITAL IMAGE PROCESSING METHODS***
	int CMyBitmap::DIPGetProfile(int pos, UINT direction, CMySignal& sgnResult)	
	{
		if (WrongDimensions(this))
			return 0;

		if (direction == DIR_HORIZONTAL)
		{
			sgnResult.SGNResize(Width);
			for (int x = 0; x < Width; x++)
				sgnResult.Bits[x] = (int)Rows[pos][x];
		}
		else if (direction == DIR_VERTICAL)
		{
			sgnResult.SGNResize(Height);
			for (int y = 0; y < Height; y++)
				sgnResult.Bits[y] = (int)Rows[y][pos];
		}
		else
			return 0;
		return 1;
	}

	int CMyBitmap::DIPGetProjection(UINT direction, CMySignal& sgnResult)	
	{
		if (WrongDimensions(this))
			return 0;

		if (direction == DIR_HORIZONTAL)
		{
			sgnResult.SGNResize(Width);
			for (int x = 0; x < Width; x++)
			{
				int c = 0;
				for (int y = 0; y < Height; y++)
					c += (int)Rows[y][x];

				sgnResult.Bits[x] = c;
			}
		}
		else if (direction == DIR_VERTICAL)
		{
			sgnResult.SGNResize(Height);
			for (int y = 0; y < Height; y++)
			{
				int c = 0;
				for (int x = 0; x < Width; x++)		
					c += (int)Rows[y][x];

			sgnResult.Bits[y]  = c;
			}
		}
		else
			return 0;
		return 1;
	}

	int CMyBitmap::DIPSetProfile(int pos, UINT direction, CMySignal& sgnResult)	
	{
		if (WrongDimensions(this))
			return 0;

		if (direction == DIR_HORIZONTAL)
		{
			for (int x = 0; x < Width && x < sgnResult.Length; x++)
				Rows[pos][x] = (BYTE)sgnResult.Bits[x];
		}
		else if (direction == DIR_VERTICAL)
		{
			for (int y = 0; y < Height && y < sgnResult.Length; y++)
				Rows[y][pos] = (BYTE)sgnResult.Bits[y];
		}
		else
			return 0;
		return 1;
	}



	int CMyBitmap::DIPContrastNormalization(int iLClipLevel = 10000,  int iRClipLevel = 10000,  CMyBitmap* pbmpResult = NULL, int iEfLeft = 0, int iEfTop = 0, int iEfRight = -1, int iEfBottom = -1,float MaxContrastFactor = -1)
	{
		if (BPP != 8 || WrongDimensions(this))
			return 0;

		int RColorClipLevel = (Height * Width) / iRClipLevel;
		int LColorClipLevel = (Height * Width) / iLClipLevel;
		int ihistogram[256] = {0};
		int ibmpsize = Height * Width;
		BYTE LeftLevel = 0,  RightLevel = 0;

		if(iEfRight == -1)
			iEfRight = Width - 1;  
		if(iEfBottom == -1)
			iEfBottom = Height - 1;
		
		if(!CorrectWindow(iEfLeft, iEfTop, iEfRight, iEfBottom))
			return 0;

		// Create Histogram
		if (iEfLeft == 0 && iEfTop == 0 && iEfRight == (Width - 1) && iEfBottom == (Height - 1))
		{
			for(int i = 0; i < ibmpsize; i++)
				ihistogram[Bits[i]]++;
		}
		else
		{
			RColorClipLevel = ((iEfBottom - iEfTop) * (iEfRight - iEfLeft)) / iRClipLevel;
			LColorClipLevel = ((iEfBottom - iEfTop) * (iEfRight - iEfLeft)) / iLClipLevel;
			for(int y = iEfTop; y <= iEfBottom; y++)
				for(int x = iEfLeft; x <= iEfRight; x++)
					ihistogram[Rows[y][x]]++;
		}

		//find Left Clip Level
		for (int i = 0; i < 256; i++)
			if (ihistogram[i] > (LColorClipLevel))
			{
				LeftLevel = i;
				break;
			}
		// Find Right Clip Level
		for (int i = 255; i >= 0; i--)
			if (ihistogram[i] > (RColorClipLevel))
			{
				RightLevel = i;
				break;
			}
				
		//Check for empty bitmap
		if (RightLevel <= LeftLevel)
			return 0;

		float CFactor = (float)255 / (RightLevel - LeftLevel);

		if (MaxContrastFactor > 0)
		   return 0;

		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;

		for (int i = 0; i < ibmpsize; i++)
		{
			int c = round(double(*(Bits + i) - LeftLevel) * CFactor);
			res[i] = BYTE((c < 0) ? 0 : ((c > 255) ? 255 : c));	
		}

	  
		return 1;
	};

	int CMyBitmap::DIPShrinkAssign (CMyBitmap& bmpSource,int iShrunkFactor)
	{
		if ((bmpSource.Width <= 0) || (bmpSource.Height <= 0) || (bmpSource.BPP != 8) || (iShrunkFactor < 1)|| (iShrunkFactor > 8) )
			return 0;
		int sf = 1 << iShrunkFactor;
		Width = bmpSource.Width / sf;
		Height = bmpSource.Height / sf;
		BPP = bmpSource.BPP;
		BytesPerRow = Width * BPP / 8;
		BytesPerRow += (4 - BytesPerRow%4) % 4;	// int alignment

		int bitssize = BytesPerRow * Height;
		FreeIfAllocated(Bits);
		Bits = new BYTE[bitssize];
		UpdateRows();
		// copy mean of a block for each bit bites
		for (int y = 0; y < Height; y++)
			for(int x = 0; x < Width; x++)
			{
				int c = 0;

				int xp = x << iShrunkFactor;
				int yp = y << iShrunkFactor;

				for (int yy = 0; yy < sf; yy++)
					for (int xx = 0; xx <  sf; xx++)
						c += bmpSource.Rows[yp + yy][xp + xx];
				
				c = c >> sf;
				
				Rows[y][x] = (BYTE) c;
			}

		if (bmpSource.BPP == 8)
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo=(BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
			Palette=(RGBQUAD*)((char*)BitmapInfo+sizeof(BITMAPINFOHEADER));
			memcpy(Palette,bmpSource.Palette,256*sizeof(RGBQUAD));
		}
		else
		{
			FreeIfAllocated(BitmapInfo);
			BitmapInfo=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
		}

		memcpy(&(BitmapInfo->bmiHeader),  &(bmpSource.BitmapInfo->bmiHeader),  sizeof(tagBITMAPINFOHEADER));

		BitmapInfo->bmiHeader.biHeight = -Height;
		BitmapInfo->bmiHeader.biWidth = Width;
		BitmapInfo->bmiHeader.biSizeImage = Height * BytesPerRow;
		return 1;
	};

	int CMyBitmap::DIPBoxBlur(int iHND, int iVND, CMyBitmap* pbmpResult = NULL)
	{
		if(WrongDimensions(this))
			return 0;

		BYTE** piSelectedDest = (pbmpResult == NULL) ? Rows : pbmpResult->Rows;

		BYTE** buffer = new BYTE*[Height];
			for (int i = 0; i < Height; i++)
				buffer[i] = new BYTE[Width];

		// blur all rows
			   
		int rows = 0;
		int* rowbuffer = new int[max(Width, Height)];
		for(int x = 0; x < Width; x++)
			rowbuffer[x] = 0;
		for(int y = -iVND; y < Height + iVND; y++)
		{
			int yl = y - iVND;
			int yr = y + iVND;
			if(yr < Height)
			{
				for(int x = 0; x < Width; x++)
					rowbuffer[x] += Rows[yr][x];
				rows++;
			}
			if(y >= 0 && y < Height)
				for(int x = 0; x < Width; x++)
					buffer[y][x] = rowbuffer[x] / rows;

			if(yl >= 0)
			{
				rows--;
				for(int x = 0; x < Width; x++)
					rowbuffer[x] -= Rows[yl][x];
			}
		}
		rows = 0;
		for(int y = 0; y < Height; y++)
			rowbuffer[y] = 0;
		for(int x = -iHND; x < Width + iHND; x++)
		{
			int xl = x - iHND;
			int xr = x + iHND;
			if(xr < Width)
			{
				for(int y = 0; y < Height; y++)
					rowbuffer[y] += buffer[y][xr];
				rows++;
			}
			if(x >= 0 && x < Width)
				for(int y = 0; y < Height; y++)
					piSelectedDest[y][x] = rowbuffer[y] / rows;
			if(xl >= 0)
			{
				rows--;
				for(int y = 0; y < Height; y++)
					rowbuffer[y] -= buffer[y][xl];
			}
		}

		delete []rowbuffer;

		 
			  
		   /*
		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Width; x++)
			{

				int i1 = -iHND;
				int i2 = iHND;
				if (x < iHND)
					i1 = -x;
				if (x > Width - 1 - iHND)
					i2 = Width - 1 - x;
				int j = 0,  k = 0;
				for (int i = i1; i <= i2; i++)
				{
					j += Rows[y][x + i];
					k++;
				}
				buffer[x][y] = round(double(j) / double(k));
			}

		 

		//blur all columns into selected destination
		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Width; x++)
			{
				int i1 = -iVND;
				int i2 = iVND;
				if (y < iVND)
					i1 = -y;
				if (y > Height - 1 - iVND)
					i2 = Height - 1 - y;
				int j = 0,  k = 0;
				for (int i = i1; i <= i2; i++)
				{
					j += buffer[x][y + i];
					k++;
				}
				piSelectedDest[y][x] = round(double(j) / double(k));
			}
	
			   */
		  
		for (int i = 0; i < Height; i++)
			delete[] buffer[i];
		delete []buffer;

		return 1;
	}

	int CMyBitmap::DIPWindow(int iWinWidth, int iWinHeight, int wrmRepresentMode, CMyBitmap &bmpResult)
	{
		//if(WrongDimensions(this) || BPP != 8)
		//	return 0;
   
		int iResW = Width / iWinWidth;
		int iResH = Height / iWinHeight;
		int iBlockSize = iWinWidth * iWinHeight;
		bmpResult.BMPNew(iResW, iResH, 8);

		for(int by = 0; by < iResH; by ++)
			for(int bx = 0; bx < iResW; bx++)
			{
				int c = 0;
				int bby = by * iWinHeight;
				int bbx = bx * iWinWidth;

				switch(wrmRepresentMode)
				{
				case WRM_MEAN:
					for(int y = 0; y < iWinWidth; y++)
						for(int x = 0; x < iWinWidth; x++)
							c += Rows[bby + y][bbx + x];
					c /= iBlockSize; 
					break;
				case WRM_MINIMUM:
					c = 255;
					for(int y = 0; y < iWinWidth; y++)
						for(int x = 0; x < iWinWidth; x++)
							if(c > Rows[bby + y][bbx + x])
								c = Rows[bby + y][bbx + x];
					break;
				case WRM_MAXIMUM:
					
					for(int y = 0; y < iWinWidth; y++)
						for(int x = 0; x < iWinWidth; x++)
							if(c < Rows[bby + y][bbx + x])
								c = Rows[bby + y][bbx + x];
					break;
				}
				bmpResult.Rows[by][bx] = (BYTE)c;
			};
	
			   
		return 1;
	}


	int CMyBitmap::DIPEdgeDetction(int iHND, int iVND,bool bBidirectional = false, int iOffset = 0, CMyBitmap* pbmpResult = NULL)
	{
		if(WrongDimensions(this))
			return 0;

		BYTE** piSelectedDest = (pbmpResult == NULL) ? Rows : pbmpResult->Rows;

		BYTE** buffer = new BYTE*[Height];
			for (int i = 0; i < Height; i++)
				buffer[i] = new BYTE[Width];

		int rows = 0;
		int* rowbuffer = new int[max(Width, Height)];
		for(int x = 0; x < Width; x++)
			rowbuffer[x] = 0;
		for(int y = -iVND; y < Height + iVND; y++)
		{
			int yl = y - iVND;
			int yr = y + iVND;
			if(yr < Height)
			{
				for(int x = 0; x < Width; x++)
					rowbuffer[x] += Rows[yr][x];
				rows++;
			}
			if(y >= 0 && y < Height)
				for(int x = 0; x < Width; x++)
					buffer[y][x] = rowbuffer[x] / rows;

			if(yl >= 0)
			{
				rows--;
				for(int x = 0; x < Width; x++)
					rowbuffer[x] -= Rows[yl][x];
			}
		}
		rows = 0;
		for(int y = 0; y < Height; y++)
			rowbuffer[y] = 0;

		if(bBidirectional)
		{
			for(int x = -iHND; x < Width + iHND; x++)
			{
				int xl = x - iHND;
				int xr = x + iHND;
				if(xr < Width)
				{
					for(int y = 0; y < Height; y++)
						rowbuffer[y] += buffer[y][xr];
					rows++;
				}
				if(x >= 0 && x < Width)
					for(int y = 0; y < Height; y++)
						piSelectedDest[y][x] = min(255, max(0, Rows[y][x] - rowbuffer[y] / rows + iOffset));
				if(xl >= 0)
				{
					rows--;
					for(int y = 0; y < Height; y++)
						rowbuffer[y] -= buffer[y][xl];
				}
			}
		}
		else
		{
			for(int x = -iHND; x < Width + iHND; x++)
			{
				int xl = x - iHND;
				int xr = x + iHND;
				if(xr < Width)
				{
					for(int y = 0; y < Height; y++)
						rowbuffer[y] += buffer[y][xr];
					rows++;
				}
				if(x >= 0 && x < Width)
					for(int y = 0; y < Height; y++)
						piSelectedDest[y][x] = min(255, abs(Rows[y][x] - rowbuffer[y] / rows));
				if(xl >= 0)
				{
					rows--;
					for(int y = 0; y < Height; y++)
						rowbuffer[y] -= buffer[y][xl];
				}
			}

		}



		delete []rowbuffer;
		for (int i = 0; i < Height; i++)
			delete[] buffer[i];
		delete []buffer;

		return 1;	
	}
	int CMyBitmap::DIPMosaic(int iBoxWidth, int iBoxHeight, CMyBitmap* pbmpResult = NULL)
	{
		if (WrongDimensions(this) || BPP != 8 || iBoxWidth <= 0 || iBoxHeight <= 0)
			return 0;
		int bw = Width / iBoxWidth;
		int bh = Height / iBoxHeight;
		int iBoxArea = iBoxWidth * iBoxHeight;
		int xof = Width - Width % iBoxWidth;
		int yof = Height - Height % iBoxHeight;

		BYTE * * resrows = pbmpResult ? pbmpResult->Rows : Rows;

		for(int y = 0; y < Height; y++)
		{
			for(int bx = 0; bx < bw; bx++)
			{
				int c = 0;
				int bxx = bx * iBoxWidth;
				for(int x = bxx; x < bxx + iBoxWidth; x++)
					c += Rows[y][x];
				c /= iBoxWidth;
				for(int x = bxx; x < bxx + iBoxWidth; x++)
					resrows[y][x] = c;
			}
		}

		for(int x = 0; x < Width; x++)
		{
			for(int by = 0; by < bh; by++)
			{
				int c = 0;
				int byy = by * iBoxHeight;
				for(int y = byy; y < byy + iBoxHeight; y++)
					c += resrows[y][x];
				c /= iBoxHeight;
				for(int y = byy; y < byy + iBoxHeight; y++)
					resrows[y][x] = c;
			}
		}

		for(int y = 0; y < Height; y++)
			for(int x = xof; x < Width; x++)	
				resrows[y][x] = resrows[y][xof - 1];

		for(int x = 0; x < Width; x++)
			for(int y = yof; y < Height; y++)	
				resrows[y][x] = resrows[yof - 1][x];

		return 1;
	}

	int CMyBitmap::DIPPredefinedConvolution(UINT convolution, int param1 = 0, int param2 = 0, CMyBitmap* pbmpResult = NULL)
	{
		if (WrongDimensions(this) || BPP != 8)
			return 0;

		CMyBitmap bmpTemp;
		bmpTemp.BMPNew(Width, Height, BPP); 

		switch (convolution)
		{	
			case CNV_GAUSSIAN1:	//	1 2 1 
			{					//	2 4 2	/16
								//	1 2 1
				for (int y = 0; y < Height ; y++)
					for (int x = 0; x < Width; x++)
					{
						int c = Rows[y][x] * 4;
						int k = 4;
						if (x < Width - 1)
						{
							c += Rows[y][x + 1] * 2;
							k += 2;
						}
						if (y < Height - 1)
						{
							c += Rows[y + 1][x] * 2;
							k += 2;
						}
						if (x > 0)
						{
							c += Rows[y][x - 1] * 2;
							k += 2;
						}
						if (y > 0)
						{
							c += Rows[y - 1][x] * 2;
							k += 2;
						}
						if (x < Width - 1 && y < Height - 1)
						{
							c += Rows[y + 1][x + 1];
							k ++;
						}
						if (x < Width - 1 && y > 0)
						{
							c += Rows[y - 1][x + 1];
							k ++;
						}
						if (x > 0 && y < Height - 1)
						{
							c += Rows[y + 1][x - 1];
							k ++;
						}
						if (x > 0 && y > 0)
						{
							c += Rows[y - 1][x - 1];
							k ++;
						}
						bmpTemp.Rows[y][x] = c / k;
					};				
				break;
			}
			case CNV_GAUSSIAN2:	//	0 1 0 
			{					//	1 2 1	/6
								//	0 1 0
				for (int y = 0; y < Height ; y++)
					for (int x = 0; x < Width; x++)
					{
						int c = Rows[y][x] * 2;
						int k = 2;
						if (x < Width - 1)
						{
							c += Rows[y][x + 1];
							k ++;
						}
						if (y < Height - 1)
						{
							c += Rows[y + 1][x];
							k ++;
						}
						if (x > 0)
						{
							c += Rows[y][x - 1];
							k ++;
						}
						if (y > 0)
						{
							c += Rows[y - 1][x];
							k ++;
						}
						bmpTemp.Rows[y][x] = c / k;
					};				
				break;
			}			
			case CNV_LAPLACIAN:	//	-1 -1 -1		 param1 => offset
			{					//	-1 +8 -1	/1
								//	-1 -1 -1
				for (int y = 1; y < Height - 1; y++)
					for (int x = 1; x < Width - 1; x++)
					{
						int c = Rows[y][x + 1] + Rows[y][x - 1] + Rows[y + 1][x] + Rows[y - 1][x]
						      + Rows[y + 1][x + 1] + Rows[y - 1][x - 1] + Rows[y + 1][x - 1] + Rows[y - 1][x + 1];
						bmpTemp.Rows[y][x] = min(255, max(0, Rows[y][x] * 8 - c + param1));
					};	
				bmpTemp.DRAWLine(0, 0, Width, 0, param1);
				bmpTemp.DRAWLine(0, Height - 1, Width, Height - 1, param1);
				bmpTemp.DRAWLine(0, 0, 0, Height, param1);
				bmpTemp.DRAWLine(Width - 1, 0, Width - 1, Height, param1);

				break;
			}
			default:
				return 0;
		}
		
		if(pbmpResult)
			pbmpResult->BMPAssign(bmpTemp);
		else
			BMPAssign(bmpTemp);
		

		return 1;
	}

	int CMyBitmap::DIPMedianBlur(int iHND, int iVND, CMyBitmap& bmpResult)
	{
		//histogram table
		int iLevelsCount[256];
		
		for (int y = 0; y < Height; y ++)
			for(int x = 0; x < Width; x++)
			{
				// horizontal limiters
				int i1 = -iHND;
				int i2 = iHND;
				if (x < iHND)
					i1 = -x;
				if (x > Width - 1 - iHND)
					i2 = Width - 1 - x;

				// vertical limiters
				int j1 = -iVND;
				int j2 = iVND;
				if (y < iVND)
					j1 = -y;
				if (y > Height - 1 - iVND)
					j2 = Height - 1 - y;

				int k = 0;

				// clear iLevelsCount
				for (int i = 0; i < 256; i++)
					iLevelsCount[i] = 0;

				//make histogram of selected block
				for (int j = j1; j <= j2; j++)
					for (int i = i1; i <= i2; i++)
					{
						iLevelsCount[Rows[y + j][x + i]]++;
						k++;
					}
				
				int m = k >> 1;

				int i = 0, j = 0;

				//find the middle color
				while ((j < m) && (i < 255))
				{
					j += iLevelsCount[i];
					i++;
				}

				bmpResult.Rows[y][x] = i;
			}
		return 1;
	};

	int CMyBitmap::DIPSubtraction(CMyBitmap& bmpSubtrahend, CMyBitmap* pbmpResult, BYTE offset)
	{
		int iMemSize = Height * BytesPerRow;
		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;
		//Subtraction
		for (int i = 0; i < iMemSize; i++)
		{
			int c = Bits[i] - bmpSubtrahend.Bits[i] + offset;
			c = (c < 0) ? 0 : ((c > 255) ? 255 : c);
			res[i] = (BYTE)c;
		}

		return 1;
	};

	int CMyBitmap::DIPLogicalAND(CMyBitmap& bmpSubtrahend, CMyBitmap* pbmpResult)
	{
		int iMemSize = Height * BytesPerRow;
		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;

		for (int i = 0; i < iMemSize; i++)
			res[i] = Bits[i] & bmpSubtrahend.Bits[i];

		return 1;
	};

	int CMyBitmap::DIPDifference(CMyBitmap& bmpSubtrahend, CMyBitmap* pbmpResult = NULL)
	{
		int iMemSize = Height * BytesPerRow;
		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;
		//Subtraction
		for (int i = 0; i < iMemSize; i++)
		{
			int c = abs(Bits[i] - bmpSubtrahend.Bits[i]);
			c = (c < 0) ? 0 : ((c > 255) ? 255 : c);
			res[i] = (BYTE)c;
		}

		return 1;
	};

	int CMyBitmap::DIPBinaryImage(int iThreshold = 128, CMyBitmap* pbmpResult = NULL)
	{
		int iMemSize = Height * BytesPerRow;
		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;
		//Binarization
		for (int i = 0; i < iMemSize; i++)
			res[i] = (Bits[i] >= iThreshold) ? 255 : 0;

		return 1;
	};

	int CMyBitmap::DIPNegativeImage(CMyBitmap* pbmpResult = NULL)
	{
		int iMemSize = Height * BytesPerRow;
		//select destination
		BYTE* res = (pbmpResult == NULL) ? Bits : pbmpResult->Bits;

		for (int i = 0; i < iMemSize; i++)
			res[i] = 255 - Bits[i];

		return 1;
	};


	int CMyBitmap::DIPDilation(int neighborDistance, CMyBitmap* pbmpResult = NULL)
	{
		if (WrongDimensions(this) || BPP != 8)
			return 0;

		int iNgbDst = neighborDistance;	
		int iWindowSize = iNgbDst * 2 + 1;
		CMyBitmap win;
		CMyBitmap bmpBuf;

		if (pbmpResult)
		{
			if (pbmpResult->Width != Width || pbmpResult->Height != Height || pbmpResult->BPP != BPP)
				pbmpResult->BMPNew(Width, Height, 8);
			bmpBuf = *pbmpResult;
		}
		else
			bmpBuf.BMPNew(Width, Height, 8);

		win.BMPNew(iWindowSize,iWindowSize,8);

		win.BMPFill(0);
		win.DRAWCircle(iNgbDst, iNgbDst, iNgbDst,255,true);
		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Width; x++)
			{
				int cx1 = -iNgbDst;
				int cx2 = iNgbDst;
				int cy1 = -iNgbDst;
				int cy2 = iNgbDst;	
				if (x < iNgbDst)
					cx1 = -x;
				if (x > Width - 1 - iNgbDst)
					cx2 = Width - 1 - x;
				if (y < iNgbDst)
					cy1 = -y;
				if (y > Height - 1 - iNgbDst)
					cy2 = Height - 1 - y;
				int c = 255;
				for (int cx = cx1; cx <= cx2; cx++)
					for (int cy = cy1; cy <= cy2; cy++)
						if (win.Rows[cy + iNgbDst][cx + iNgbDst])
							if (c > Rows[cy + y][cx + x])
								c = Rows[cy + y][cx + x];
				bmpBuf.Rows[y][x] = c;
			};
		
		if (pbmpResult)
			bmpBuf.CleanAddress();
		else
			BMPAssign(bmpBuf);
		
		return 1;
	}
	int CMyBitmap::DIPErosion(int neighborDistance, CMyBitmap* pbmpResult = NULL)
	{
		if (WrongDimensions(this) || BPP != 8)
			return 0;

		int iNgbDst = neighborDistance;	
		int iWindowSize = iNgbDst * 2 + 1;
		CMyBitmap win;
		CMyBitmap bmpBuf;

		if (pbmpResult)
		{
			if (pbmpResult->Width != Width || pbmpResult->Height != Height || pbmpResult->BPP != BPP)
				pbmpResult->BMPNew(Width, Height, 8);
			bmpBuf = *pbmpResult;
		}
		else
			bmpBuf.BMPNew(Width, Height, 8);

		win.BMPNew(iWindowSize,iWindowSize,8);

		win.BMPFill(0);
		win.DRAWCircle(iNgbDst, iNgbDst, iNgbDst,255,true);
		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Width; x++)
			{
				int cx1 = -iNgbDst;
				int cx2 = iNgbDst;
				int cy1 = -iNgbDst;
				int cy2 = iNgbDst;	
				if (x < iNgbDst)
					cx1 = -x;
				if (x > Width - 1 - iNgbDst)
					cx2 = Width - 1 - x;
				if (y < iNgbDst)
					cy1 = -y;
				if (y > Height - 1 - iNgbDst)
					cy2 = Height - 1 - y;
				int c = 0;
				for (int cx = cx1; cx <= cx2; cx++)
					for (int cy = cy1; cy <= cy2; cy++)
						if (win.Rows[cy + iNgbDst][cx + iNgbDst])
							if (c < Rows[cy + y][cx + x])
								c = Rows[cy + y][cx + x];
				bmpBuf.Rows[y][x] = c;
			};
		
		if (pbmpResult)
			bmpBuf.CleanAddress();
		else
			BMPAssign(bmpBuf);
		
		return 1;
	}

	int CMyBitmap::DIPConvolution(CMyBitmap& bmpConv, CMyBitmap& bmpResult, BYTE iOffset = 0, BOOL bCircular = false)
	{	  
		int iBWOffset = bmpConv.Width / 2;
		int iBHOffset = bmpConv.Height / 2;
		int iConvWeight = 0;
	   
		for (int j = 0; j < bmpConv.Height; j++)
			for (int i = 0; i < bmpConv.Width; i++)
				iConvWeight += bmpConv.Rows[j][i];		
		
		if (iConvWeight == 0)
			return 0;

		if(bCircular)
		{
			for (int y = 0; y < Height; y++)
				for (int x = 0; x < Width; x++)
				{
					int c = 0;
					for(int j = 0; j < bmpConv.Height; j++)
						for(int i = 0; i < bmpConv.Width; i++)
							c += Rows[(Height + y + j - iBHOffset) % Height][(Width + x + i - iBWOffset) % Width] * bmpConv.Rows[j][i];
					bmpResult.Rows[y][x] = max(0, min(255, c / iConvWeight + iOffset));
				};
		}
		else
		{
			for (int y = iBHOffset; y < Height - iBHOffset; y++)
				for (int x = iBWOffset; x < Width - iBWOffset; x++)
				{
					int c = 0;
					for(int j = 0; j < bmpConv.Height; j++)
						for(int i = 0; i < bmpConv.Width; i++)
							c += Rows[y + j - iBHOffset][x + i - iBWOffset] * bmpConv.Rows[j][i];
					bmpResult.Rows[y][x] = max(0, min(255, c / iConvWeight + iOffset));
				};		
		}

		return 1;
	}

	int CMyBitmap::DIPCleanTinyObjects(int iMinimumArea)
	{
		
		for (int y = 0; y < Height; y++)
			for(int x = 0; x < Width; x++)
			{
				if (Rows[y][x] == 255)
				{	
					int iArea = 0;
					FloodFill(x, y, 1, &iArea);
					if (iArea < iMinimumArea)
						FloodFill(x, y, 0);
				}
				else if (Rows[y][x] > 1)
					return 0;
			};
		for (int y = 0; y < Height; y++)
			for(int x = 0; x < Width; x++)
				Rows[y][x] *= 255; 	

		return 1;
	}


	int CMyBitmap::DIPRotate(double dAngle, BYTE iBackColor, CMyBitmap& bmpResult)
	{
		if (WrongDimensions(this))
			return 0;

		if (bmpResult.Height != Height || bmpResult.Width != Width || bmpResult.BPP != 8)
			bmpResult.BMPNew(Width, Height, 8);

		dAngle = -dAngle;
		double a = +cos(dAngle);
		double b = +sin(dAngle);
		double c = -sin(dAngle);
		double d = +cos(dAngle);
		int xo = Width / 2;
		int yo = Height / 2;

		for (int y = 0; y < Height; y++)
			for(int x = 0; x < Width; x++)
			{
				int xx = xo + round((double)(x - xo) * a + (double)(y - yo) * b);
				int yy = yo + round((double)(x - xo) * c + (double)(y - yo) * d);
				if (xx >= 0 && yy >= 0 && xx < Width && yy < Height)
					bmpResult.Rows[y][x] = Rows[yy][xx];
				else
					bmpResult.Rows[y][x] = iBackColor;
			};
		return 1;
	}



	// ***DRAW METHODS***   
 	int CMyBitmap::DRAWCircle(int x, int y, int radius, int color, bool filled = false) 
	{
		if (WrongDimensions(this))
			return 0;

		int i = 0, j = radius;
		int iBytePP = BPP / 8;
		BYTE cRed   = GetRValue(color);
		BYTE cGreen = GetGValue(color);
		BYTE cBlue  = GetBValue(color);
		BYTE b_color = (BYTE)color;

		if (!filled )
		{
			// draw un filled circle
			if (BPP == 8)
				while (i <= j) 
				{
					if ((y - j >= 0) && (x + i >= 0) && (y - j < Height) && (x + i < Width))
						Rows[y - j][x + i] = b_color;
					if ((y - i >= 0) && (x + j >= 0) && (y - i < Height) && (x + j < Width))
						Rows[y - i][x + j] = b_color;
					if ((y + j >= 0) && (x + i >= 0) && (y + j < Height) && (x + i < Width))
						Rows[y + j][x + i] = b_color;
					if ((y + i >= 0) && (x + j >= 0) && (y + i < Height) && (x + j < Width))
						Rows[y + i][x + j] = b_color;
					if ((y - j >= 0) && (x - i >= 0) && (y - j < Height) && (x - i < Width))
						Rows[y - j][x - i] = b_color;
					if ((y - i >= 0) && (x - j >= 0) && (y - i < Height) && (x - j < Width))
						Rows[y - i][x - j] = b_color;
					if ((y + j >= 0) && (x - i >= 0) && (y + j < Height) && (x - i < Width))
						Rows[y + j][x - i] = b_color;
					if ((y + i >= 0) && (x - j >= 0) && (y + i < Height) && (x - j < Width))
						Rows[y + i][x - j] = b_color;
					i++ ;
					
					j = int(sqrt((double)radius * radius - i * i) + 0.5);
				}		
			else // BPP == 24 or 32
				while (i <= j) 
				{
					if ((y - j >= 0) && (x + i >= 0) && (y - j < Height) && (x + i < Width))
						SetPixel24((x + i) * iBytePP,y - j , cRed, cGreen, cBlue);
					if ((y - i >= 0) && (x + j >= 0) && (y - i < Height) && (x + j < Width))
						SetPixel24((x + j) * iBytePP,y - i , cRed, cGreen, cBlue);
					if ((y + j >= 0) && (x + i >= 0) && (y + j < Height) && (x + i < Width))
						SetPixel24((x + i) * iBytePP,y + j , cRed, cGreen, cBlue);
					if ((y + i >= 0) && (x + j >= 0) && (y + i < Height) && (x + j < Width))
						SetPixel24((x + j) * iBytePP,y + i , cRed, cGreen, cBlue);
					if ((y - j >= 0) && (x - i >= 0) && (y - j < Height) && (x - i < Width))
						SetPixel24((x - i) * iBytePP,y - j , cRed, cGreen, cBlue);
					if ((y - i >= 0) && (x - j >= 0) && (y - i < Height) && (x - j < Width))
						SetPixel24((x - j) * iBytePP,y - i , cRed, cGreen, cBlue);
					if ((y + j >= 0) && (x - i >= 0) && (y + j < Height) && (x - i < Width))
						SetPixel24((x - i) * iBytePP,y + j , cRed, cGreen, cBlue);
					if ((y + i >= 0) && (x - j >= 0) && (y + i < Height) && (x - j < Width))
						SetPixel24((x - j) * iBytePP,y + i , cRed, cGreen, cBlue);
					i++ ;
					
					j = int(sqrt((double)radius * radius - i * i) + 0.5);
				}		

			return 1;
		}
		
		// leftside stores left boarder of the circle
		int* leftside = new int[radius * 2 + 1]; 

		for (int i = 0; i < radius * 2 + 1; i++)
		{
			leftside[i] = radius;
		}			  

		while (i <= j) 
		{
			if (leftside[-j + radius] > i) 
				leftside[-j + radius] = i; 

			if (leftside[-j + radius] > -i) 
				leftside[-j + radius] = -i; 
			
			if (leftside[j + radius] > i) 
				leftside[j + radius] = i; 
			
			if (leftside[j + radius] > -i) 
				leftside[j + radius] = -i; 
			
			if (leftside[-i + radius] > j) 
				leftside[-i + radius] = j; 

			if (leftside[-i + radius] > -j) 
				leftside[-i + radius] = -j; 
			
			if (leftside[i + radius] > j) 
				leftside[i + radius] = j; 
			
			if (leftside[i + radius] > -j) 
				leftside[i + radius] = -j; 
			
			i++;
			
			j = int(sqrt((double)radius * radius - i * i) + 0.5) ;
		}

		// cys and cye indicate start point of drawing
		int cys = 0;
		int cye = radius * 2;
		if (y - radius < 0)
			cys = -(y - radius);
		if (cye + (y - radius) >= Height)
			cye = Height - 1 - (y - radius);
		
		for (int cy = cys; cy <= cye; cy++)
		{
			// right side is symmetry of left side
			int rightside = -leftside[cy] + x;
			leftside[cy] += x;

			if (leftside[cy] < 0)
				leftside[cy] = 0;

			if (rightside >= Width)
				rightside = Width - 1;

			int cyy = cy + y - radius;
			if (BPP == 8)
				for (int cx = leftside[cy]; cx <= rightside; cx++)
					Rows[cyy][cx] = b_color;
			else if(BPP == 24)
				for (int cx = leftside[cy] * 3; cx <= rightside * 3; cx += 3)
					SetPixel24(cx,cyy,cRed,cGreen,cBlue)

			else if(BPP == 32)
				for (int cx = leftside[cy] * 4; cx <= rightside * 4; cx += 4)
					SetPixel32(cx,cyy,cRed,cGreen,cBlue,0) 

		}

		delete[] leftside;

		return 1;
	};



	int CMyBitmap::DRAWBitmap(int x, int y, CMyBitmap& bmpSource)
	{	// 'DRAWBitmap' draws a bitmap on a given position 
		if ( BPP != bmpSource.BPP || WrongDimensions(this) || WrongDimensions(&bmpSource))
			return 0;

		int iDrawWidth = x >= 0 ? min(bmpSource.Width, Width - x) : min(bmpSource.Width + x, Width );
		int iDrawHeight = y >= 0 ? min(bmpSource.Height, Height - y) : min(bmpSource.Height + y, Height);
		int iOffsetX = max(0, x);
		int iOffsetY = max(0, y);
		int iOffsetX2 = max(0, -x);
		int iOffsetY2 = max(0, -y);
		int iBytePP = BPP / 8;

		iDrawWidth *= iBytePP;
		iOffsetX *= iBytePP;
		iOffsetX2 *= iBytePP;

		for (int yi = 0; yi < iDrawHeight; yi++)
			for (int xi = 0; xi < iDrawWidth; xi++)
			{
				Rows[yi + iOffsetY][xi + iOffsetX] = bmpSource.Rows[yi + iOffsetY2][xi + iOffsetX2];
			}

		return 1;
	};
	int CMyBitmap::DRAWBitmap(int x, int y, int sx, int sy, CMyBitmap& bmpSource)
	{
		return DRAWBitmap(x - sx,y - sy,bmpSource);
	};

	int CMyBitmap::DRAWBitmap(int x, int y, int sx, int sy,int w, int h, double sr, CMyBitmap& bmpSource)
	{
		if ( BPP != bmpSource.BPP || WrongDimensions(this) || WrongDimensions(&bmpSource) || w <= 0 || h <= 0)
			return 0;

		int iOffsetX = max(0, x);
		int iOffsetY = max(0, y);
		int iOffsetX2 = max(0, sx);
		int iOffsetY2 = max(0, sy);
		int iDrawWidth = min(min(bmpSource.Width - sx, Width - x), w) ;
		int iDrawHeight = min(min(bmpSource.Height - sy, Height - y), h);
		int iBytePP = BPP / 8;

		if(sr == 0)
		{
			iDrawWidth *= iBytePP;
			iOffsetX *= iBytePP;
			iOffsetX2 *= iBytePP;

			for(int yi = 0; yi < iDrawHeight; yi++)
				for(int xi = 0; xi < iDrawWidth; xi++)
				{
					Rows[yi + iOffsetY][xi + iOffsetX] = bmpSource.Rows[yi + iOffsetY2][xi + iOffsetX2];
				};
		}
		else
		{
			sr = -sr;
			double sa = +cos(sr);
			double sb = +sin(sr);
			double sc = -sin(sr);
			double sd = +cos(sr);
			int xo = iDrawHeight / 2;
			int yo = iDrawHeight / 2;
												    
			if(BPP == 8)
			{
				for (int yi = 0; yi < iDrawHeight; yi++)
					for (int xi = 0; xi < iDrawWidth; xi++)
					{
						int sxx = xo + round((double)(xi - xo) * sa + (double)(yi - yo) * sb) + iOffsetX2;
						int syy = yo + round((double)(xi - xo) * sc + (double)(yi - yo) * sd) + iOffsetY2;
						int yy = yi + iOffsetY;
						int xx = xi + iOffsetX;
						if(sxx >= 0 && syy >= 0 && syy < bmpSource.Height && sxx < bmpSource.Width)
							Rows[yy][xx] = bmpSource.Rows[syy][sxx];
						else
							Rows[yy][xx] = 0;
					};
			}
			else if(BPP == 24)
			{
				// Not Developed!
			}
			else if(BPP == 32)
			{
				// Not Developed!
			}
			
		}		

		return 1;
	}

	int CMyBitmap::DRAWDot(int x, int y,int color)
	{
		if(x >= 0 && y >= 0 && x < Width && y < Height)
		{
			switch(BPP)
			{
			case 8:
				Rows[y][x] = (BYTE)color;
				break;
			case 24:
				Rows[y][x * 3 + 0] = GetBValue(color);
				Rows[y][x * 3 + 1] = GetGValue(color);
				Rows[y][x * 3 + 2] = GetRValue(color);
				break;
			case 32:
				Rows[y][x * 4 + 0] = GetBValue(color);
				Rows[y][x * 4 + 1] = GetGValue(color);
				Rows[y][x * 4 + 2] = GetRValue(color);
				//Rows[y][x * 4 + 3] = 0;
				break;
			default:
				return 0;
			}
		}
		else 
			return 0;
		return 1;
	}

	int CMyBitmap::DRAWLine(int x1, int y1, int x2, int y2, int color)
	{
		if (WrongDimensions(this))
			return 0;

		int mxw = max(abs(x1 - x2), abs(y1 - y2));
		int w = x2 - x1;
		int h = y2 - y1;
		BYTE cRed   = GetRValue(color);
		BYTE cGreen = GetGValue(color);
		BYTE cBlue  = GetBValue(color);
		BYTE b_color  = BYTE(color);
		if (BPP == 8)
			for (int i = 0; i <= mxw; i++)
			{
				int x = x1 + (int)(((double)w / mxw) * i);
				int y = y1 + (int)(((double)h / mxw) * i);
				if (x >= 0 && y >= 0 && y < Height && x < Width)
					Rows[y][x] = b_color;
			}
		else if (BPP == 24)
			for (int i = 0; i <= mxw; i++)
			{
				int x = x1 + (int)(((double)w / mxw) * i);
				int y = y1 + (int)(((double)h / mxw) * i);
				if (x >= 0 && y >= 0 && y < Height && x < Width)
				{
					SetPixel24(x * 3, y, cRed, cGreen, cBlue)
				}
			}
		else if (BPP == 32)
			for (int i = 0; i <= mxw; i++)
			{
				int x = x1 + (int)(((double)w / mxw) * i);
				int y = y1 + (int)(((double)h / mxw) * i);
				if (x >= 0 && y >= 0 && y < Height && x < Width)
					SetPixel32(x * 4, y, cRed, cGreen, cBlue, 0)
			}
		
		return 1;
	};

	int CMyBitmap::DRAWLine(int x, int y, int length, double angle, int color)
	{	//		 |-p/2
		//	  p -o- 0
		//		 |p/2
		int x2 = x + (int)(length * cos(angle));
		int y2 = y + (int)(length * sin(angle));
		return DRAWLine(x, y, x2, y2, color);
	}

	int CMyBitmap::DRAWRectangle(RECT rect, int color, bool filled = false)
	{
		return DRAWRectangle(rect.left, rect.top, rect.right, rect.bottom, color, filled);
	}

	int CMyBitmap::DRAWRectangle(int left, int top, int right, int bottom, int color, bool filled = false)
	{
		if (WrongDimensions(this))
			return 0;	

		if (!filled)
		{
			DRAWLine(left, top, right, top, color);  
			DRAWLine(left, top, left, bottom, color);  
			DRAWLine(right, top, right, bottom, color);  
			DRAWLine(left, bottom, right, bottom, color);  
			
		}
		else
		{
			BYTE cRed   = GetRValue(color);
			BYTE cGreen = GetGValue(color);
			BYTE cBlue  = GetBValue(color);
			BYTE b_color = (BYTE)color;
			int x1 = max(0, min(Width - 1, min(left, right)));
			int y1 = max(0, min(Height - 1, min(top, bottom)));
			int x2 = max(0, min(Width - 1, max(left, right)));
			int y2 = max(0, min(Height - 1, max(top, bottom)));
			int iBytePP = BPP / 8;
			if (BPP == 8)
			{
				for (int y = y1; y <= y2; y++)
					for (int x = x1; x <= x2; x++)
						Rows[y][x] = b_color;
			}
			else // BPP == 24 or 32
			{
				x1 *= iBytePP;
				x2 *= iBytePP;
				for (int y = y1; y <= y2; y++)
					for (int x = x1; x <= x2; x += iBytePP)
					{
						SetPixel24(x, y, cRed, cGreen, cBlue);
					};
			}
		}

		return 1;
	}

	int CMyBitmap::DRAWFrame(int left, int top, int right, int bottom,int sidelen, int color)
	{
		DRAWLine(left, top, left, top + sidelen, color);
		DRAWLine(left, top, left + sidelen, top, color);
		DRAWLine(left, bottom, left, bottom - sidelen, color);
		DRAWLine(left, bottom, left + sidelen, bottom, color);
		DRAWLine(right, bottom, right - sidelen, bottom, color);
		DRAWLine(right, bottom, right, bottom - sidelen, color);
		DRAWLine(right, top, right - sidelen, top, color);
		DRAWLine(right, top, right, top + sidelen, color);
		return 1;
	}

	int CMyBitmap::DRAWStar(int x, int y, int Size, int color,int sc = 6)
	{
		if (WrongDimensions(this))
			return 0;

		int res = 1;
		for (int t = 0; t < sc; t++)
			res &= DRAWLine(x, y, Size, (double)t * PI2 / sc, color); 
		return res;
	}

	int CMyBitmap::DRAWPolygon(int x, int y, int Size, int color,int Sides = 6)
	{
		if (WrongDimensions(this))
			return 0;

		int res = 1;
		double offa = PI / 2 + (Sides % 2 == 0 ? PI / Sides: 0);
		for (int t = 0; t < Sides; t++)
		{

			double a1 = (double)((Sides + t - 1) % Sides) * PI2 / Sides - offa;
			double a2 = (double)t * PI2 / Sides - offa;
			int x1 = x + (int)(Size * cos(a1));
			int y1 = y + (int)(Size * sin(a1));
			int x2 = x + (int)(Size * cos(a2));
			int y2 = y + (int)(Size * sin(a2));
			res &= DRAWLine(x1, y1, x2, y2, color); 
		}
		return res;
	}



	int CMyBitmap::DRAWFloodFill(int x, int y, int color, int* area = NULL)
	{
		if (WrongDimensions(this))
			return 0;	

		switch (BPP)
		{
		case 8:
			FloodFill(x,y,color,area);
			break;
		default:
			return 0;
		}
	return 1;
	}

	int CMyBitmap::DRAWCharacter(BYTE chr, int x, int y, int forecolor, int backcolor = CLR_TRANSPARENT)
	{
		if (WrongDimensions(this) || x < 0 || y < 0 || x + CHARACTERS_WIDTH > Width || y + CHARACTERS_HEIGHT > Height)
			return 0;

		if(chr >= 128 || chr < 32)
			chr = 127;
		chr -= 32;

		int i = 62;
		__int64 ch = Chrs8CourierNew[chr];

		if IsValidRGBColor(backcolor)
			DRAWRectangle(x ,y ,x + CHARACTERS_WIDTH, y + CHARACTERS_HEIGHT, backcolor, true);
		y++;
		if(BPP == 8)
		{
	    forecolor = (BYTE)forecolor;
		for(int yy = y; yy < y + CHARACTERS_HEIGHT; yy++)
			for(int xx = x; xx < x + CHARACTERS_WIDTH; xx++)
			{
				if(ch & 1)
					Rows[yy][xx] = (BYTE)forecolor;
				ch >>= 1;
			}
		}
		else if(BPP == 24)
		{
			BYTE r = GetRValue(forecolor);
			BYTE g = GetGValue(forecolor);
			BYTE b = GetBValue(forecolor);
			for(int yy = y; yy < y + CHARACTERS_HEIGHT; yy++)
				for(int xx = x; xx < x + CHARACTERS_WIDTH ; xx++)
				{
					int xx3 = xx * 3;
					if(ch & 1)
						SetPixel24(xx3, yy, r, g, b);
					ch >>= 1;
				};
		}
		else if(BPP == 32)
		{
			BYTE r = GetRValue(forecolor);
			BYTE g = GetGValue(forecolor);
			BYTE b = GetBValue(forecolor);
			for(int yy = y; yy < y + CHARACTERS_HEIGHT; yy++)
				for(int xx = x; xx < x + CHARACTERS_WIDTH; xx++)
				{
					int xx4 = xx * 4;
					if(ch & 1)
						SetPixel32(xx4, yy, r, g, b, 0);
					ch >>= 1;
				};
		}
		return 1;
		
	}

	int CMyBitmap::DRAWText(const char * str, int x, int y, int forecolor, int backcolor = CLR_TRANSPARENT , int alignment = TXT_ALIGNMENT_LEFT)
	{
		int strln = strlen(str);
		int j = 0;

		if(alignment == TXT_ALIGNMENT_CENTER)
			x -= (CHARACTERS_WIDTH * strln) / 2;
		if(alignment == TXT_ALIGNMENT_RIGHT)
			x -= CHARACTERS_WIDTH * strln;

		for(int i = 0 ; i < strln; i++)
		{
			if (str[i] == 9)
			{
				DRAWCharacter(32, x + j++ * CHARACTERS_WIDTH, y, forecolor, backcolor);
				while(j % 4 != 0)
					DRAWCharacter(32, x + j++ * CHARACTERS_WIDTH, y, forecolor, backcolor);
			}
			else if(str[i] == 10)
			{
				 y += CHARACTERS_HEIGHT;
				 j = 0;
			}
			else
			{
				DRAWCharacter(str[i], x + j++ * CHARACTERS_WIDTH, y, forecolor, backcolor);
			}
		}
		return 1;
	}

	int CMyBitmap::DRAWText(int number, int x, int y, int forecolor, int backcolor = -1, int alignment = TXT_ALIGNMENT_LEFT)
	{
		CHAR ss[50] = {0};
		_itoa_s(number, ss, 10);
		DRAWText(ss, x, y, forecolor, backcolor, alignment);
		return 1;
	}

};

#endif 

/*
	TODO: Liner interpolation for BMPResize	 -DONE-
	TODO: 24 and 32 bit mode for BMPResize 
	TODO: Add EXT functions:
					EXTProfile
					EXTProjection (toptodown or lefttoright)
					EXTAvrege & variance Of Window
					EXTHistogram of Window
	TODO: BMPFlip must be added (vertically & horizontally)
	TODO: DRAWPolygon must be added 
	TODO: Wavelet
	TODO: Color Synthesizer
	TODO: convertto8bit needs to be optimized
	TODO: boxblur needs to be optimized
*/

/*
	Actions:
	April 4,2010   method added:  	GDIPaint (HDC hdc,int x = 0,int y = 0,int wx1, int wy1,int wx2, int wy2,  bool frame = false)  
	April 4,2010   method updated:	DIPPredefinedConvolution    laplacian convelution  
	May  26,2010   method added:     BMPReadFromStream(int width, int height, int bpp, BYTE * bits, bool upside_down = false)
	May  26,2010   method added:     BMPWriteToStream(int& width, int& height, int& bpp, BYTE * bits, bool upside_down = false) 
	Aug 3, 2010 DRAWPolygon was added 
	Aug 3, 2010 mpPI2 chenged to PI2
	sep 14 2010 home version  and  office version was merger { assigen(bits) -> readfromstream ,  copytobits -> writetostream   }
	oct 07 2010 GDIReadFromHandle is added
	


*/
