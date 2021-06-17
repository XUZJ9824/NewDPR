#pragma once
#include "GraphEngine.h"

class CDrawEngine :	public CGraphEngine
{
public:
	CDrawEngine();
	~CDrawEngine();

public:
	virtual void DoDraw();

	//Drawing Layers
	std::vector<CBasicLayer*> m_lstDrawLayers;
	bool InitDrawLayers();
	void ClearLayers();
};

