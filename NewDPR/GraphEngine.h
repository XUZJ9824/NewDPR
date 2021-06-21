#pragma once
#include "dx_data.h"
#include "dx_iface.h"
#include <iostream>
#include <string>
#include "EntityDefs.h"
#include "MapLayer.h"

#define MAX_VERTEXS_ENGINE     100

class CGraphEngine
{
public:
	CGraphEngine();
	~CGraphEngine();
	
	static void SetHWND(HWND hwnd);
	
	virtual void ClearMapLayers(){};
	
	static bool InitD3Devices();
	static void DestroyResources();
	static HRESULT EnsureD3DObjects();
	static HRESULT EnsureD3DDevices();
	static void UpdateMatrix();		
	static void SetViewSize(int w, int h);
	static void SetZoomDelta(float zDelta);
	static void SetTransform(int x, int y);
	
	//Zoom, Transition, Rotation, Coordinate
	static int m_cxOffset, m_cyOffset;
	static int m_viewWidth, m_viewHeight;
	static double m_scale;
	static double m_rotate;
	static D3DXMATRIX   matWorld;
	static D3DXMATRIX   matTranslate, matRotation, matScale;     //先缩放、再旋转、最后平移
	static D3DXMATRIX   matView;    // the view transform matrix
	static D3DXMATRIX   matProjection;     // the projection transform matrix
	
	
	//Window
	static HWND     m_hwnd;
	static int      m_ViewCount;
	static UINT     m_AdaptersCount;
	static UINT     m_UsedAdapter;
	static bool     m_IsDirectXAvailable;
	
	//D3D
	static D3DDEVTYPE           m_deviceType;
	static D3DFORMAT            m_DisplayFormat;
	static D3DCAPS9             m_DeviceCaps;
	static ID3DXSprite*         m_LabelsSprite;
	static LPD3DXFONT           m_LabelsFont;
	static int                  m_FontSize;

	static IDirect3D9          *m_pD3D;
	static IDirect3D9Ex        *m_pD3DEx;
	static IDirect3DDevice9    *m_pd3dDevice;
	static IDirect3DDevice9Ex  *m_pd3dDeviceEx;

	//Buffer for Engine specific display. others are covered by buffer from each Layer
	static IDirect3DVertexBuffer9* m_pVertexBuffer;

	//Draw
	static void StartDraw();
	static void EndDraw();

	static void DrawLine(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR dColor);
	static void DrawCircle(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT RadiusW, FLOAT RadiusH, D3DCOLOR dColor);

};

