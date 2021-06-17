#pragma once

#include "GraphEngine.h"

using namespace std;
//FILE: MapEngine.h


class CMapEngine : public CGraphEngine
{
	friend class CMapLayer;
public:
	CMapEngine();
	~CMapEngine();

	virtual void ClearMapLayers();
	bool loadDwgMap(std::string strImagePath);

	virtual void DoDraw();
	std::list<DRW_Layer> * GetDwgLayers() {	return &(m_dxData.layers); };
protected:
	//Raw Image Data here
	dx_data     m_dxData;
	dx_ifaceBlock* FindBlock(std::string strBlock);

	void        ExtractDwgEntityData(DRW_Entity *pDwgEntity, CMapLayer* pLayer, CPointAlteration *pAlteration, DRW_Block* pDwgBlk = NULL);
	CEntity*    DwgLineToLineEntity(DRW_Line *pDwgLine, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_LWPolyline * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_Arc * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_Circle * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgHatchToPolyLine(DRW_Hatch * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgInsertToPolyLine(DRW_Insert * pDwgEntity, CPointAlteration *pAlteration, CMapLayer* pLayer);
	void        GetArpRelativeCoordinates(CPointF *ptSrc, CPointF *ptDst, CPointAlteration *pAlteration);

	//Color Settings

	//Font Related	

	//Map Layers
	CMapLayer* FindOrNewMapLayer(std::string  strName);
	std::vector<CMapLayer*> m_lstMapLayers;
	bool ParseDwgToLayers();
	bool ParseDwgToLayers(dx_ifaceBlock* pBlockEntities, CMapLayer* pLayer, CPointAlteration *pAlteration);
};