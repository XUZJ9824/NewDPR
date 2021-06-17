#pragma once
#include "BasicLayer.h"
#include "GraphEngine.h"

class CDrawLayer :	public CBasicLayer
{
public:
	CDrawLayer();
	CDrawLayer(CGraphEngine* pEngine, std::string strLayerName, std::wstring displayName);
	~CDrawLayer();
};

