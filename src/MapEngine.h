#pragma once
#include "dx_data.h"
#include "dx_iface.h"
#include <iostream>
#include <string>
#include "EntityDefs.h"
#include "MapLayer.h"

using namespace std;
//FILE: MapEngine.h

#define MAX_VERTEXS_ENGINE     100
class CMapEngine {
	friend class CMapLayer;
public:
	CMapEngine(HWND hwnd);
	~CMapEngine();

	void ClearMapLayers();
	bool loadDwgMap(std::string strImagePath);
	bool InitD3Devices();
	void DestroyResources();
	HRESULT EnsureD3DObjects();
	HRESULT EnsureD3DDevices();

	void DoDraw();
	void DrawLine(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR dColor);
	void DrawCircle(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT RadiusW, FLOAT RadiusH, D3DCOLOR dColor);
	void UpdateMatrix();
	void SetViewSize(int w, int h);
	void SetZoomDelta(float zDelta);
	void SetTransform(int x, int y);

	std::list<DRW_Layer> * GetDwgLayers() {	return &(m_dxData.layers); };
protected:
	//Raw Image Data here
	dx_data     m_dxData;
	void        ExtractDwgEntityData(DRW_Entity *pDwgEntity, CBasicLayer* pLayer, CPointAlteration *pAlteration, DRW_Block* pDwgBlk = NULL);
	CEntity*    DwgLineToLineEntity(DRW_Line *pDwgLine, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_LWPolyline * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_Arc * pDwgEntity, CPointAlteration *pAlteration);
	void        GetArpRelativeCoordinates(CPointF *ptSrc, CPointF *ptDst, CPointAlteration *pAlteration);

	//Color Settings

	//Font Related

	//Zoom, Transition, Rotation, Coordinate
	float m_ArpX;
	float m_ArpY;
	int m_cxOffset, m_cyOffset;
	int m_viewWidth, m_viewHeight;
	double m_scale;
	double m_rotate;
	D3DXMATRIX                      matWorld;
	D3DXMATRIX                      matTranslate, matRotation, matScale;     //先缩放、再旋转、最后平移
	D3DXMATRIX                      matView;    // the view transform matrix
	D3DXMATRIX                      matProjection;     // the projection transform matrix

	//Layers
	CMapLayer* FindOrNewMapLayer(std::wstring  strName);
	std::vector<CMapLayer*> m_lstMayLayers;
	bool ParseDwgToLayers();

	//Window
	HWND                            m_hwnd;
	int                             m_ViewCount;
	UINT                            m_AdaptersCount;
	UINT                            m_UsedAdapter;
	bool                            m_IsDirectXAvailable;

	//D3D
	D3DDEVTYPE           m_deviceType;
	D3DFORMAT            m_DisplayFormat;
	D3DCAPS9             m_DeviceCaps;
	ID3DXSprite*         m_LabelsSprite;
	LPD3DXFONT           m_LabelsFont;
	int                  m_FontSize;

	IDirect3D9          *m_pD3D;
	IDirect3D9Ex        *m_pD3DEx;
	IDirect3DDevice9    *m_pd3dDevice;
	IDirect3DDevice9Ex  *m_pd3dDeviceEx;

	IDirect3DVertexBuffer9* m_pVertexBuffer;
};