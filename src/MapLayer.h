#pragma once

//FILE: MapLayer.h
#include "commonDefs.h"
#include "EntityDefs.h"
#include "dx_data.h"

#include "DxErr.h"
#include <vector>
#include <list>
#include <map>

class CMapEngine;

/*basic common Layer*/
class CBasicLayer 
{
public:
	CBasicLayer() {};
	~CBasicLayer() {};

	enum DRW_LW_Conv::lineWidth       lWeight;
	int                               color; /*!< layer color, code 62 , default BYLAYER (256)*/
};

/*Layer used for drawing layers*/
class CDrawLayer : public CBasicLayer
{
public:
	CDrawLayer() {};
	~CDrawLayer() {};
};

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

	bool InitPolygonBuffer();
	
	bool InitLineStripsBuffer();

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
	UINT                            m_LineStripsPosBufferSize;
	LPDIRECT3DVERTEXBUFFER9         m_LineStripsPosBuffer;
	//LPDIRECT3DVERTEXBUFFER9         m_pGeometryColorVertexBuffer;
	//IDirect3DIndexBuffer9*          m_pGeometryIndexVertexBuffer;
	LPDIRECT3DVERTEXDECLARATION9    m_pVertexDeclaration;
	int                             m_MaxIndex;
	int                             m_SegmentCount;
	bool                            m_GeometryBuffersInitialized;

	LPDIRECT3DVERTEXBUFFER9         m_pPolygonPositionVertexBuffer;
	//LPDIRECT3DVERTEXBUFFER9         m_pPolygonColorVertexBuffer;
	//IDirect3DIndexBuffer9*          m_pPolygonIndexVertexBuffer;
	//IndicesList                     m_PolygonIndices;
	int                             m_PolygonVerticesCount;
	int                             m_TrianglesCount;
	int                             m_PolygonMaxIndex;
	bool                            m_PolygonBuffersInitialized;
};