#include "stdafx.h"
#include "commonDefs.h"
#include <string>
#include "stdio.h"
#include "stdlib.h"

using namespace std;

tDataCenter gDataCenter;

int Print(const TCHAR *strToPrint)
{
	int rt = wcslen(strToPrint);

	//std::string str(strToPrint);
	//std::wstring wstr(str.begin(), str.end());
	OutputDebugString(strToPrint);
	return rt;
}

int Print_Debug(const TCHAR *sfmt, ...) 
{
	int rt = 0;
	static TCHAR stemp[1024] = { 0 };

	va_list ap;
	va_start(ap, sfmt);
	vswprintf(stemp, 1024, sfmt, ap);

	return Print(stemp);
}

int Print_Error(const TCHAR* str) 
{
	int rt = 0;
	static TCHAR stemp[1024] = { 0 };

	swprintf_s(stemp, 1024, _T("ERR:%s\r\n"), str);
	return Print(stemp);
}

void GetColorByIndex(unsigned int iDwgColor, unsigned short &R, unsigned short &G, unsigned short &B) 
{
	static unsigned short acad_colors_static[256 + 1][3] =
	{
		{ 255, 255, 255 }, //0
		{ 255,   0,   0 }, //1 Red
		{ 255, 255,   0 }, //2 Yellow
		{ 0, 255,   0 },   //3 Green
		{ 0, 255, 255 },   //4 Cyan
		{ 0,   0, 255 },   //5 Blue
		{ 255,   0, 255 }, //6 Carmine
		{ 255, 255, 255 }, //7 white
		{ 0, 128, 128 },    //8 user defined 1
		{ 0, 0, 128 },      //9 user defined 2

		{ 255,   0,   0 },
		{ 255, 127, 127 },
		{ 204,   0,   0 },
		{ 204, 102, 102 },
		{ 153,   0,   0 },
		{ 153,  76,  76 },
		{ 127,   0,   0 },
		{ 127,  63,  63 },
		{ 76,   0,   0 },
		{ 76,  38,  38 },
		{ 255,  63,   0 },
		{ 255, 159, 127 },
		{ 204,  51,   0 },
		{ 204, 127, 102 },
		{ 153,  38,   0 },
		{ 153,  95,  76 },
		{ 127,  31,   0 },
		{ 127,  79,  63 },
		{ 76,  19,   0 },
		{ 76,  47,  38 },
		{ 255, 127,   0 },
		{ 255, 191, 127 },
		{ 204, 102,   0 },
		{ 204, 153, 102 },
		{ 153,  76,   0 },
		{ 153, 114,  76 },
		{ 127,  63,   0 },
		{ 127,  95,  63 },
		{ 76,  38,   0 },
		{ 76,  57,  38 },
		{ 255, 191,   0 },
		{ 255, 223, 127 },
		{ 204, 153,   0 },
		{ 204, 178, 102 },
		{ 153, 114,   0 },
		{ 153, 133,  76 },
		{ 127,  95,   0 },
		{ 127, 111,  63 },
		{ 76,  57,   0 },
		{ 76,  66,  38 },
		{ 255, 255,   0 },
		{ 255, 255, 127 },
		{ 204, 204,   0 },
		{ 204, 204, 102 },
		{ 153, 153,   0 },
		{ 153, 153,  76 },
		{ 127, 127,   0 },
		{ 127, 127,  63 },
		{ 76,  76,   0 },
		{ 76,  76,  38 },
		{ 191, 255,   0 },
		{ 223, 255, 127 },
		{ 153, 204,   0 },
		{ 178, 204, 102 },
		{ 114, 153,   0 },
		{ 133, 153,  76 },
		{ 95, 127,   0 },
		{ 111, 127,  63 },
		{ 57,  76,   0 },
		{ 66,  76,  38 },
		{ 127, 255,   0 },
		{ 191, 255, 127 },
		{ 102, 204,   0 },
		{ 153, 204, 102 },
		{ 76, 153,   0 },
		{ 114, 153,  76 },
		{ 63, 127,   0 },
		{ 95, 127,  63 },
		{ 38,  76,   0 },
		{ 57,  76,  38 },
		{ 63, 255,   0 },
		{ 159, 255, 127 },
		{ 51, 204,   0 },
		{ 127, 204, 102 },
		{ 38, 153,   0 },
		{ 95, 153,  76 },
		{ 31, 127,   0 },
		{ 79, 127,  63 },
		{ 19,  76,   0 },
		{ 47,  76,  38 },
		{ 0, 255,   0 },
		{ 127, 255, 127 },
		{ 0, 204,   0 },
		{ 102, 204, 102 },
		{ 0, 153,   0 },
		{ 76, 153,  76 },
		{ 0, 127,   0 },
		{ 63, 127,  63 },
		{ 0,  76,   0 },
		{ 38,  76,  38 },
		{ 0, 255,  63 },
		{ 127, 255, 159 },
		{ 0, 204,  51 },
		{ 102, 204, 127 },
		{ 0, 153,  38 },
		{ 76, 153,  95 },
		{ 0, 127,  31 },
		{ 63, 127,  79 },
		{ 0,  76,  19 },
		{ 38,  76,  47 },
		{ 0, 255, 127 },
		{ 127, 255, 191 },
		{ 0, 204, 102 },
		{ 102, 204, 153 },
		{ 0, 153,  76 },
		{ 76, 153, 114 },
		{ 0, 127,  63 },
		{ 63, 127,  95 },
		{ 0,  76,  38 },
		{ 38,  76,  57 },
		{ 0, 255, 191 },
		{ 127, 255, 223 },
		{ 0, 204, 153 },
		{ 102, 204, 178 },
		{ 0, 153, 114 },
		{ 76, 153, 133 },
		{ 0, 127,  95 },
		{ 63, 127, 111 },
		{ 0,  76,  57 },
		{ 38,  76,  66 },
		{ 0, 255, 255 },
		{ 127, 255, 255 },
		{ 0, 204, 204 },
		{ 102, 204, 204 },
		{ 0, 153, 153 },
		{ 76, 153, 153 },
		{ 0, 127, 127 },
		{ 63, 127, 127 },
		{ 0,  76,  76 },
		{ 38,  76,  76 },
		{ 0, 191, 255 },
		{ 127, 223, 255 },
		{ 0, 153, 204 },
		{ 102, 178, 204 },
		{ 0, 114, 153 },
		{ 76, 133, 153 },
		{ 0,  95, 127 },
		{ 63, 111, 127 },
		{ 0,  57,  76 },
		{ 38,  66,  76 },
		{ 0, 127, 255 },
		{ 127, 191, 255 },
		{ 0, 102, 204 },
		{ 102, 153, 204 },
		{ 0,  76, 153 },
		{ 76, 114, 153 },
		{ 0,  63, 127 },
		{ 63,  95, 127 },
		{ 0,  38,  76 },
		{ 38,  57,  76 },
		{ 0,  63, 255 },
		{ 127, 159, 255 },
		{ 0,  51, 204 },
		{ 102, 127, 204 },
		{ 0,  38, 153 },
		{ 76,  95, 153 },
		{ 0,  31, 127 },
		{ 63,  79, 127 },
		{ 0,  19,  76 },
		{ 38,  47,  76 },
		{ 0,   0, 255 },
		{ 127, 127, 255 },
		{ 0,   0, 204 },
		{ 102, 102, 204 },
		{ 0,   0, 153 },
		{ 76,  76, 153 },
		{ 0,   0, 127 },
		{ 63,  63, 127 },
		{ 0,   0,  76 },
		{ 38,  38,  76 },
		{ 63,   0, 255 },
		{ 159, 127, 255 },
		{ 51,   0, 204 },
		{ 127, 102, 204 },
		{ 38,   0, 153 },
		{ 95,  76, 153 },
		{ 31,   0, 127 },
		{ 79,  63, 127 },
		{ 19,   0,  76 },
		{ 47,  38,  76 },
		{ 127,   0, 255 },
		{ 191, 127, 255 },
		{ 102,   0, 204 },
		{ 153, 102, 204 },
		{ 76,   0, 153 },
		{ 114,  76, 153 },
		{ 63,   0, 127 },
		{ 95,  63, 127 },
		{ 38,   0,  76 },
		{ 57,  38,  76 },
		{ 191,   0, 255 },
		{ 223, 127, 255 },
		{ 153,   0, 204 },
		{ 178, 102, 204 },
		{ 114,   0, 153 },
		{ 133,  76, 153 },
		{ 95,   0, 127 },
		{ 111,  63, 127 },
		{ 57,   0,  76 },
		{ 66,  38,  76 },
		{ 255,   0, 255 },
		{ 255, 127, 255 },
		{ 204,   0, 204 },
		{ 204, 102, 204 },
		{ 153,   0, 153 },
		{ 153,  76, 153 },
		{ 127,   0, 127 },
		{ 127,  63, 127 },
		{ 76,   0,  76 },
		{ 76,  38,  76 },
		{ 255,   0, 191 },
		{ 255, 127, 223 },
		{ 204,   0, 153 },
		{ 204, 102, 178 },
		{ 153,   0, 114 },
		{ 153,  76, 133 },
		{ 127,   0,  95 },
		{ 127,  63, 111 },
		{ 76,   0,  57 },
		{ 76,  38,  66 },
		{ 255,   0, 127 },
		{ 255, 127, 191 },
		{ 204,   0, 102 },
		{ 204, 102, 153 },
		{ 153,   0,  76 },
		{ 153,  76, 114 },
		{ 127,   0,  63 },
		{ 127,  63,  95 },
		{ 76,   0,  38 },
		{ 76,  38,  57 },
		{ 255,   0,  63 },
		{ 255, 127, 159 },
		{ 204,   0,  51 },
		{ 204, 102, 127 },
		{ 153,   0,  38 },
		{ 153,  76,  95 },
		{ 127,   0,  31 },
		{ 127,  63,  79 },
		{ 76,   0,  19 },
		{ 76,  38,  47 },

		{ 51,  51,  51 },{ 91,  91,  91 },{ 132, 132, 132 },
		{ 173, 173, 173 },{ 214, 214, 214 },{ 255, 255, 255 },
		{ 255, 255, 255 }
	};

	if (iDwgColor <= 256) 
	{
		R = acad_colors_static[iDwgColor][0];
		G = acad_colors_static[iDwgColor][1];
		B = acad_colors_static[iDwgColor][2];
	}
}

int IsLayerVisible(std::wstring sLayer)
{
	int i = -1;
	int rt = 0;
	for (std::vector<tDwgLayer>::const_iterator it = gDataCenter.arrDwgLayers.begin(); it != gDataCenter.arrDwgLayers.end(); it++)
	{
		i++;
		if ( (*it).sLayer == sLayer) break;
	}

	if (i >= 0)
	{
		rt = gDataCenter.arrDwgLayers[i].bVisible;
	}

	return rt;
}