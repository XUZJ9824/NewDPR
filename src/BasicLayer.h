#pragma once
//FIlE: BasicLayer.h

#include "commonDefs.h"
#include "EntityDefs.h"
#include "dx_data.h"

#include "DxErr.h"
#include <vector>
#include <list>
#include <map>

typedef enum
{
	E_LAYER_MAP,
	E_LAYER_DRAWING,
	E_LAYER_LIGHT,
	E_LAYER_UNKNOWN
}eLayerType;

class CGraphEngine;
class CMapEngine;
class CDrawEngine;
/*basic common Layer*/
class CBasicLayer
{
public:
	CBasicLayer() {
		m_layerType = E_LAYER_UNKNOWN;
	};
	~CBasicLayer() {};
public:
    /*分散线段*/
	virtual bool InitLineListsBuffer();
	
	/*连接线段*/
	virtual bool InitPolyLineBuffer();
	
	/*多点折现区域*/
	virtual bool InitPolygonBuffer();

	virtual void DoDraw();
	
public:
	enum DRW_LW_Conv::lineWidth       lWeight;
	int                               color; /*!< layer color, code 62 , default BYLAYER (256)*/
	eLayerType                        m_layerType;
	
public:
	//Layer Options
	bool m_bVisible;
	bool m_bInitialed;
	//Original layer name from DWG
	std::string m_strLayerName;	

	//Unicode layer name for display purpose
	std::wstring m_strDisplayName;
	CAirportMapEntities m_Entities;
	
public:
	CGraphEngine *m_pEngine;

protected:	
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