#pragma once

//FILE: MapLayer.h
#include "BasicLayer.h"

/*Layer used for dwg loaded Map*/
class CMapLayer : public CBasicLayer
{
public:
	CMapLayer(CGraphEngine* pEngine = NULL);
	CMapLayer(CGraphEngine* pEngine, std::string strLayerName, std::wstring displayName);
	~CMapLayer();

	//options from dwg
	void GetLayerInfo(std::list<DRW_Layer> *players);
protected:
	bool m_bAlreadyGetInfo;

public:
	void PrintEntities();
};