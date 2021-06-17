#include "stdafx.h"
#include "DrawLayer.h"


CDrawLayer::CDrawLayer()
{
}

CDrawLayer::CDrawLayer(CGraphEngine* pEngine, std::string strLayerName, std::wstring displayName) :
	CBasicLayer(pEngine, strLayerName, displayName)
{

}


CDrawLayer::~CDrawLayer()
{
}
