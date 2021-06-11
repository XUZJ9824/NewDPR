#pragma once

//FILE: MapLayer.h
#include "BasicLayer.h"

/*Layer used for dwg loaded Map*/
class CMapLayer : public CBasicLayer
{
public:
	CMapLayer(CMapEngine* pEngine = NULL);
	CMapLayer(CMapEngine* pEngine, std::wstring strLayerName);
	~CMapLayer();

	//options from dwg
	void GetLayerInfo(std::list<DRW_Layer> *players);
protected:
	bool m_bAlreadyGetInfo;

public:
	//Layer Options
	bool m_bVisible;
	bool m_bInitialed;
	std::wstring m_strLayerName;
	std::wstring m_strDisplayName;
	CAirportMapEntities m_Entities;

	bool InitD3DDevices();

	/*分散线段*/
	bool InitLineListsBuffer();
	
	/*连接线段*/
	bool InitPolyLineBuffer();
	
	/*多点折现区域*/
	bool InitPolygonBuffer();

	void DoDraw();

	void PrintEntities();

protected:	
	CMapEngine *m_pEngine;
	//D3D Interfaces
	IDirect3D9          *m_pD3D;
	IDirect3D9Ex        *m_pD3DEx;
	IDirect3DDevice9    *m_pd3dDevice;
	IDirect3DDevice9Ex  *m_pd3dDeviceEx;

	//drawing Element buffer
	UINT                            m_LineListPosBufferSize;
	LPDIRECT3DVERTEXBUFFER9         m_LineListPosBuffer;
	bool                            m_LineStripsBuffersInitialized;

	UINT                            m_PolyLinePosBufferSize;
	LPDIRECT3DVERTEXBUFFER9         m_PolyLinePosBuffer;
	bool                            m_PolyLinePosBufferInitialized;

	UINT                            m_PolygonPosBufferSize;
	LPDIRECT3DVERTEXBUFFER9         m_PolygonPosBuffer;
	bool					        m_PolygonPosBufferInitialized;
};