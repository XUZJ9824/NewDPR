#include "DrawEngine.h"
#include "DrawLayer.h"

CDrawEngine::CDrawEngine()
{
	OutputDebugString(LOCATION);
	InitDrawLayers();
}


CDrawEngine::~CDrawEngine()
{
	OutputDebugString(LOCATION);
	ClearLayers();
}

void CDrawEngine::DoDraw()
{
	if (m_pd3dDevice)
	{
		//draw each layers
		for (std::vector<CBasicLayer*>::const_iterator it = this->m_lstDrawLayers.begin(); it != m_lstDrawLayers.end(); it++)
		{
			if (IsLayerVisible((*it)->m_strDisplayName)) {
				(*it)->DoDraw();
			}
		}
	}
}

bool CDrawEngine::InitDrawLayers()
{
	OutputDebugString(LOCATION);

	bool rt = false;
	for (int i = 0; i < TOTAL_DRAW_LAYERS; i++)
	{
		//DrawLayer will not use the Dwg Layer name for here.
		CDrawLayer *pNewLayer = new CDrawLayer(this, "", sDrawLayers[i]);
		m_lstDrawLayers.push_back(pNewLayer);
	}

	return rt;
}

void CDrawEngine::ClearLayers()
{
	for (std::vector<CBasicLayer*>::const_iterator it = this->m_lstDrawLayers.begin(); it != m_lstDrawLayers.end(); it++)
	{
		delete (*it);
	}
	m_lstDrawLayers.clear();
}
