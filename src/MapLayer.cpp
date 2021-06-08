#include "MapLayer.h"
#include "MapEngine.h"

/*assuming 10000 objects with 64 bytes each*/
char g_MapLayerPrintBuffer[10000 * 64] = { 0 };

CMapLayer::CMapLayer(CMapEngine *pEngine):
	m_bAlreadyGetInfo(false)
{	
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

CMapLayer::CMapLayer(CMapEngine *pEngine, std::wstring strLayerName)
{
	m_bInitialed = false;
	m_GeometryBuffersInitialized = false;
	m_PolygonBuffersInitialized = false;
	m_bAlreadyGetInfo = false;

	this->m_strLayerName = strLayerName;
	this->m_strDisplayName = strLayerName;

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
	SAFE_RELEASE(m_LineStripsPosBuffer);
	SAFE_RELEASE(m_pPolygonPositionVertexBuffer);

	int ii = 0;
}

void CMapLayer::GetLayerInfo(std::list<DRW_Layer> *players)
{
	if (!m_bAlreadyGetInfo) 
	{
		//Find dwg layer 
		std::string layerName(m_strLayerName.begin(), m_strLayerName.end());
		for (std::list<DRW_Layer>::const_iterator it = players->begin(); it != players->end(); it++) 
		{
			if (layerName == (*it).name) 
			{
				//Get dwg layer information
				this->lWeight = (*it).lWeight;
				this->color = (*it).color;

				m_bAlreadyGetInfo = true;
			}
		}		
	}
	
}

bool CMapLayer::InitPolygonBuffer()
{
	return true;
}

/*handle single lines*/
bool CMapLayer::InitLineStripsBuffer() 
{	
	int itemPoints;
	int pointsCount = this->m_Entities.TotalLinePoints();

	if (m_pd3dDevice)
	{
		// Release old buffers if needed
		SAFE_RELEASE(m_LineStripsPosBuffer);

		m_LineStripsPosBufferSize = pointsCount * sizeof(Vertex);

		//Create Vertex Buffer
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_LineStripsPosBufferSize, 
			D3DUSAGE_WRITEONLY, 
			CUSTOMVERTEX,
			D3DPOOL_DEFAULT, 
			&m_LineStripsPosBuffer, NULL)))
		{
			//_ASSERT(0); TBD
			return false;
		}

		// Lock them
		Vertex*  pInternalPositionVertexBuffer;		
		WORD     index = 0;
		PWORD    pIndex;

		if (FAILED(m_LineStripsPosBuffer->Lock(0, m_LineStripsPosBufferSize, (void**)&pInternalPositionVertexBuffer, 0)))
		{
			_ASSERT(0); //TBD
			return false;
		}
		
		for (std::vector<CLineEntity*>::const_iterator it = this->m_Entities.m_lstLines.begin(); it != m_Entities.m_lstLines.end(); it++) 
		{
			CLineEntity* pLine = *it;
			for (std::vector<CLineGeometry*>::const_iterator it2 = pLine->m_lstLineGeometries.begin(); it2 != pLine->m_lstLineGeometries.end(); it2++)
			{
				CLineGeometry* pLineGeometry = *it2;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_startPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_startPoint.Y;
				pInternalPositionVertexBuffer->dColor = pLine->m_Color;
				pInternalPositionVertexBuffer++;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_endPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_endPoint.Y;
				pInternalPositionVertexBuffer->dColor = pLine->m_Color;
				pInternalPositionVertexBuffer++;
			}
		}		
	}

	m_LineStripsPosBuffer->Unlock();
	m_GeometryBuffersInitialized = true;
	return true;
}

void CMapLayer::DoDraw() 
{
	//TBD draw Line strips here
	if (m_LineStripsPosBuffer && m_pd3dDevice) {
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pEngine->matWorld));
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &(m_pEngine->matView));
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &(m_pEngine->matProjection));

		m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetStreamSource(0, m_LineStripsPosBuffer, 0, sizeof(Vertex));
		m_pd3dDevice->SetFVF(CUSTOMVERTEX);

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_LineStripsPosBufferSize/sizeof(Vertex));
	}
}

void CMapLayer::PrintEntities() 
{
	int itotalLen = 0;
	//TBD
}