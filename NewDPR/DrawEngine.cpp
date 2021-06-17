#include "DrawEngine.h"
#include "DrawLayer.h"

CDrawEngine::CDrawEngine()
{
	OutputDebugString(LOCATION);
}


CDrawEngine::~CDrawEngine()
{
	OutputDebugString(LOCATION);
	ClearLayers();
}

void CDrawEngine::DoDraw()
{
	
}

bool CDrawEngine::InitDrawLayers()
{
	OutputDebugString(LOCATION);

	bool rt = false;
	for (int i = 0; i < TOTAL_DRAW_LAYERS; i++)
	{
		CDrawLayer *pNewLayer = new CDrawLayer();
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
