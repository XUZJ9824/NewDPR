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
	void DrawLine(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR dColor);
	void DrawCircle(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT RadiusW, FLOAT RadiusH, D3DCOLOR dColor);

	std::list<DRW_Layer> * GetDwgLayers() {	return &(m_dxData.layers); };
protected:
	//Raw Image Data here
	dx_data     m_dxData;
	dx_ifaceBlock* FindBlock(std::string strBlock);

	void        ExtractDwgEntityData(DRW_Entity *pDwgEntity, CBasicLayer* pLayer, CPointAlteration *pAlteration, DRW_Block* pDwgBlk = NULL);
	CEntity*    DwgLineToLineEntity(DRW_Line *pDwgLine, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_LWPolyline * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_Arc * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgPolylineBaseToPolyLine(DRW_Circle * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgHatchToPolyLine(DRW_Hatch * pDwgEntity, CPointAlteration *pAlteration);
	CEntity*    DwgInsertToPolyLine(DRW_Insert * pDwgEntity, CPointAlteration *pAlteration, CBasicLayer* pLayer);
	void        GetArpRelativeCoordinates(CPointF *ptSrc, CPointF *ptDst, CPointAlteration *pAlteration);

	//Color Settings

	//Font Related	

	//Map Layers
	CMapLayer* FindOrNewMapLayer(std::string  strName);
	std::vector<CMapLayer*> m_lstMapLayers;
	bool ParseDwgToLayers();
	bool ParseDwgToLayers(dx_ifaceBlock* pBlockEntities, CBasicLayer* pLayer, CPointAlteration *pAlteration);
};