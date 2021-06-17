#include "MapLayer.h"
#include "GraphEngine.h"

/*assuming 10000 objects with 64 bytes each*/
char g_MapLayerPrintBuffer[10000 * 64] = { 0 };

CMapLayer::CMapLayer(CGraphEngine *pEngine):
	m_bAlreadyGetInfo(false)	
{	
	m_layerType = E_LAYER_MAP;
	if (pEngine) 
	{
		m_pEngine = pEngine;
		m_pD3D = pEngine->m_pD3D;
		m_pD3DEx = pEngine->m_pD3DEx;
		m_pd3dDevice = pEngine->m_pd3dDevice;
		m_pd3dDeviceEx = pEngine->m_pd3dDeviceEx;
	}
	else 
	{
		m_pEngine = NULL;
		m_pD3D = NULL;
		m_pD3DEx = NULL;
		m_pd3dDevice = NULL;
		m_pd3dDeviceEx = NULL;
	}
}

CMapLayer::CMapLayer(CGraphEngine *pEngine, std::string strLayerName, std::wstring displayName)
{
	Print_Debug(_T("New CMapLayer %s\r\n"), strLayerName);

	m_bInitialed = false;
	m_LineStripsBuffersInitialized = false;
	m_PolyLinePosBufferInitialized = false;
	m_PolygonPosBufferInitialized = false;
	m_bAlreadyGetInfo = false;

	this->m_strLayerName = strLayerName;
	this->m_strDisplayName = displayName;

	if (pEngine)
	{
		m_pEngine = pEngine;
		m_pD3D = pEngine->m_pD3D;
		m_pD3DEx = pEngine->m_pD3DEx;
		m_pd3dDevice = pEngine->m_pd3dDevice;
		m_pd3dDeviceEx = pEngine->m_pd3dDeviceEx;
	}
	else
	{
		m_pEngine = NULL;
		m_pD3D = NULL;
		m_pD3DEx = NULL;
		m_pd3dDevice = NULL;
		m_pd3dDeviceEx = NULL;
	}
}

CMapLayer::~CMapLayer() 
{
	SAFE_RELEASE(m_LineListPosBuffer);
	SAFE_RELEASE(m_PolyLinePosBuffer);
	SAFE_RELEASE(m_PolygonPosBuffer);
}

void CMapLayer::GetLayerInfo(std::list<DRW_Layer> *players)
{
	if (!m_bAlreadyGetInfo) 
	{
		//Find dwg layer 		
		for (std::list<DRW_Layer>::const_iterator it = players->begin(); it != players->end(); it++) 
		{
			if (this->m_strLayerName == (*it).name) 
			{
				//Get dwg layer information
				this->lWeight = (*it).lWeight;
				this->color = (*it).color;

				m_bAlreadyGetInfo = true;
			}
		}		
	}
	
}

void CMapLayer::PrintEntities() 
{
	int itotalLen = 0;
	//TBD
}