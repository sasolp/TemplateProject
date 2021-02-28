
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include "emmintrin.h"

//print into output window
#define DBOUT( s )                          \
{                                           \
   std::ostringstream os_;                  \
   os_ << s;                                \
   OutputDebugString( os_.str().c_str() );  \
}

namespace UTL
{
	char * GetPath(char * filename)
	{
		int l = strlen(filename);
		char * path = (char*)malloc(l); 
		int i = l;
		for(; i > 0 && filename[i] != '\\'; i--);
		memcpy(path, filename, i + 1);
		path[i+1] = 0;
		return path;
	}	
	
	char * GetFileName(char * filename)
	{
		int l = strlen(filename);
		char * path = (char*)malloc(l); 
		int i = l;
		for(; i > 0 && filename[i] != '\\'; i--);
		memcpy(path, &filename[i + 1], l - i);
		
		return path;
	}

	void gotoxy(int x, int y) 
	{ 
		COORD coord; 
		coord.X = x; coord.Y = y; 
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
	}

	void posxy(SHORT &x, SHORT &y) 
	{ 
		CONSOLE_SCREEN_BUFFER_INFO sbi; 
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sbi); 
		x = sbi.dwCursorPosition.X;
		y = sbi.dwCursorPosition.Y;
	}
		
	void SetConsoleTitle(const char * title)
	{
		HANDLE conH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitle(title);
		
	}

	void SetConsoleColor(int r, int g, int b, int w)
	{
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, w * 8 + b * 4 + g * 2 + r);
	}
	bool file_exists(const char * filename)
	{
		FILE * file;
		fopen_s(&file, filename, "r");
		if (file)
		{
			fclose(file);
			return true;
		}
    return false;
	}

	char * LoadStream(char * szFileName)
	{
		ifstream file (szFileName, ios::in | ios::binary);
		char * buf;	
		if (! file.is_open()) 
			return 0;		// Error opening file
		file.seekg(0, std::ios::end);
		int Size = file.tellg();
		file.seekg(0);
		buf = (char *)malloc(Size);
		file.read((char*)buf, Size);
		file.close();
		return buf;
	};

	void SaveStream(char * szFileName, char * stream, int size)
	{
		ofstream file (szFileName, ios::out | ios::binary);
		file.write(stream, size);
		file.close();
	};




};

class MyConsolePointer
{
public:
	short posX;
	short posY;
	MyConsolePointer()
	{
		posX = 0;
		posY = 0;
	}
	void Bookmark()
	{
		UTL::posxy(posX,posY);
	}
	void Reset()
	{
		UTL::gotoxy(posX,posY);
	}
	void SetColor(bool r, bool g, bool b, bool bright)
	{
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, bright * 8 + r * 4 + g * 2 + b);
	}

	void ProgressBar(int index, int maxIndex)
	{
		int persent = index * 100 / maxIndex;
		static int lp = -1;
		if(persent != lp)
		{
			Bookmark();
			char prog[51] = {};
			memset(prog,176,50); 
			memset(prog,219,persent / 2); 
			printf(" %s%d%%",prog,persent);
			Reset();
			lp = persent;
		}
	}
};

  
static MyConsolePointer ConsolePointer;

#ifdef _WINDOWS_
#ifndef UNICODE

static LRESULT CALLBACK winutlWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

namespace WinUTL
{
	void ShowBitmap(CMyBitmap & bmp, char * Caption, int Zoom)
	{
		LPCSTR szWindowClass = (LPCSTR)Caption;
		MSG msg;
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= winutlWindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= 0;
		wcex.hIcon			= LoadIcon(NULL, 0);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= szWindowClass;
		wcex.hIconSm		= LoadIcon(NULL, 0);
		RegisterClassEx(&wcex);


		HWND h = CreateWindow(szWindowClass, (LPCSTR)Caption , WS_OVERLAPPEDWINDOW,200, 200, bmp.Width, bmp.Height, NULL, NULL, NULL, NULL);
		ShowWindow(h, 1);
		UpdateWindow(h);
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);    
			DispatchMessage(&msg);
		}

	}
}	

#endif
#endif