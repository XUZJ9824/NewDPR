#pragma once
//FILE: commonDefs.h
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif 
#include "D3d9.h"
#include "D3dx9core.h"

#include "tchar.h"
#include <vector>

#define USE_DWG

#ifdef USE_DWG
#define OFFSET_X 0 //(9411.5)
#define OFFSET_Y 0 //(9091.25)
//#define FILE_DWG "D:\\Home\\WK\\TTW_SRC\\3rdpartysw\\libdxfrw\\bin\\dwgs\\QD_MAP_20210321.dwg"
//#define FILE_DWG "D:\\Home\\WK\\TTW_SRC\\APPLICATIONS\\Data\\dpr_maps\\QD_MAP_20210329.dwg"
#define FILE_DWG "D:\\Home\\WK\\TTW_SRC\\APPLICATIONS\\Data\\dpr_maps\\UMQ_20200508_v5.dwg"
#endif //USE_DWG

#define IFC(x)			{ hr = (x); if (FAILED(hr)) goto Cleanup; }
#define IFCOOM(x)		{ if ((x) == NULL) { hr = E_OUTOFMEMORY; IFC(hr); } }
#define SAFE_RELEASE(x)	{ if (x) { x->Release(); x = NULL; } }
#define DEGTORAD        ((2.0f*D3DX_PI)/360.0f)
#define Step_Degree_Arc (D3DX_PI / 180) //average one line for 1 Angle

typedef HRESULT(WINAPI *DIRECT3DCREATE9EXFUNCTION)(UINT SDKVersion, IDirect3D9Ex**);

#if 0 //TRANSFORMED
#define CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
/*
Flag	    Description	Types Included
D3DFVF_XYZ	Indicates that the vertex format includes the X, Y and Z coordinates of an untransformed vertex. Untransformed means that the vertex has not yet been translated into screen coordinates.	float, float, float
D3DFVF_XYZRHW	Indicates that the vertex format includes the X, Y and Z coordinates as well as an additional RHW value of a transformed vertex. This means that the vertex is already in screen coordinates. The Z and the RHW are used when building software engines, which we will not get into.	float, float, float, float
D3DFVF_DIFFUSE	Indicates that the vertex format contains a 32-bit color code for a vertex, used for the color of diffuse lighting.	DWORD
D3DFVF_SPECULAR	Indicates that the vertex format contains a 32-bit color code for a vertex, used for the color of specular highlighting.	DWORD
D3DFVF_TEX0
- through
D3DFVF_TEX8	Indicates that the vertex format contains the coordinates for any textures that will be applied to a model.
*/
struct Vertex
{
	float X, Y, Z, RHW;   // from the D3DFVF_XYZRHW flag
	unsigned long dColor; // from the D3DFVF_DIFFUSE flag
};
#else //3D untranformed
#define CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct Vertex
{
	float X, Y, Z;          // from the D3DFVF_XYZ flag
	unsigned long dColor;  // from the D3DFVF_DIFFUSE flag
};
#endif 

//Debug Trace Utilities
#ifdef UNICODE
int Print_Debug(const TCHAR* sfmt, ...);
int Print_Error(const TCHAR* str);
int Print(const TCHAR *str);
#else
//TBD
#endif // !UNICODE

//Color Mapping 
void GetColorByIndex(unsigned int iDwgColor, unsigned short &R, unsigned short &G, unsigned short &B);

//Debug File Location
#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION _T(__FUNCTION__ " : " S2(__LINE__) "\n")

typedef struct 
{
	std::wstring  sLayer;
	unsigned char bVisible;
}tDwgLayer;

typedef struct 
{
	//name from each layer, based from dwg map file
	std::vector<tDwgLayer>  arrDwgLayers;
}tDataCenter;

extern tDataCenter gDataCenter;


//Layer settings,
int IsLayerVisible(std::wstring sLayer);

//String utilities
inline char Ascii2Hex(const char ch);

std::wstring Ansi2WChar(std::string str);

std::wstring ReplaceWCSWithPattern(const std::wstring &message, const std::wstring &pattern, const std::wstring &replace);


//////////////////////////Drawing Settings////////////////////////////
#define TOTAL_DRAW_LAYERS 3
const WCHAR * sDrawLayers[];