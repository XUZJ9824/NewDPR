//FILE: MapEngine.cpp
#include "stdafx.h"
#include "MapEngine.h"

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <math.h>

CMapEngine::CMapEngine():
	CGraphEngine()
{	
};

CMapEngine::~CMapEngine() {
	ClearMapLayers();
};

bool CMapEngine::loadDwgMap(std::string strImagePath) 
{
	bool bResult = false;
	//Load DWG into data memory
	dx_iface *pDxInput = new dx_iface();
	bResult = pDxInput->fileImport(strImagePath, &m_dxData);
	
	if (bResult) {
		ClearMapLayers();
		ParseDwgToLayers();
	}

	delete pDxInput;

	return bResult;
}
void CMapEngine::DoDraw()
{
	OutputDebugString(LOCATION);
	if(m_pd3dDevice)
	{		
		//draw each layers
		for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMapLayers.begin(); it != m_lstMapLayers.end(); it++)
		{
			if (IsLayerVisible((*it)->m_strDisplayName)) {
				(*it)->DoDraw(); 
			}
		}		
	}
}

CMapLayer* CMapEngine::FindOrNewMapLayer(std::string  strName)
{
	CMapLayer* pLayer = NULL;
	for (std::vector<CMapLayer*>::const_iterator it = m_lstMapLayers.begin(); it != m_lstMapLayers.end(); ++it)
	{
		CMapLayer *pCurrent = *it;
		if (pCurrent->m_strLayerName == strName) 
		{
			pLayer = pCurrent;
			break;
		}
	}

	/*create layer if not exit*/
	if (pLayer == NULL) 
	{
		std::wstring strDisp = Ansi2WChar(strName);
		OutputDebugString(strDisp.c_str());

		pLayer = new CMapLayer(this, strName, strDisp);
		this->m_lstMapLayers.push_back(pLayer);
	}

	return pLayer;
}

void CMapEngine::ClearMapLayers() 
{
	for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMapLayers.begin(); it != m_lstMapLayers.end(); it++)
	{
		CMapLayer* pitem = *it;
		delete pitem;
	}
	m_lstMapLayers.clear();
}

bool CMapEngine::ParseDwgToLayers(dx_ifaceBlock* pBlockEntities, CMapLayer* pLayer, CPointAlteration *pAlteration)
{
	bool rt = false;

	//load dwg entities
	for (std::list<DRW_Entity*>::const_iterator it = pBlockEntities->ent.begin(); it != pBlockEntities->ent.end(); ++it)
	{
		DRW_Entity *entity = *it;			
		
		ExtractDwgEntityData(entity, pLayer, pAlteration, NULL);
	}

	return rt;
}

bool CMapEngine::ParseDwgToLayers()
{
	bool rt = false;

	//load dwg entities
	for (std::list<DRW_Entity*>::const_iterator it = m_dxData.mBlock->ent.begin(); it != m_dxData.mBlock->ent.end(); ++it)
	{
		DRW_Entity *entity = *it;	

		CMapLayer *pCurrent = this->FindOrNewMapLayer(entity->layer);
		pCurrent->GetLayerInfo(&(m_dxData.layers));
		
		CPointAlteration defaultAlteration;
		ExtractDwgEntityData(entity, pCurrent, &defaultAlteration, NULL);
	}

	//dwg entities to D3D vertex for rendering purpose
	for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMapLayers.begin(); it != this->m_lstMapLayers.end(); it++)
	{
		(*it)->InitLineListsBuffer();
		(*it)->InitPolyLineBuffer();
		(*it)->InitPolygonBuffer();
	}
	return rt;
}

void  CMapEngine::ExtractDwgEntityData(DRW_Entity *pDwgEntity, CMapLayer* pLayer, CPointAlteration *pAlteration, DRW_Block* pDwgBlk) {
	CEntity *pNewEntity = NULL;

	if (pDwgEntity) 
	{
		switch(pDwgEntity->eType){
		case DRW::LINE: 
			pNewEntity = DwgLineToLineEntity((DRW_Line *) pDwgEntity, pAlteration);
			if (pNewEntity) {
				pNewEntity->SetParent(pLayer);
				pNewEntity->SetDwgColor(((DRW_Line *)pDwgEntity)->color, pDwgBlk);
			}
			break;
		case DRW::LWPOLYLINE: 
			pNewEntity = DwgPolylineBaseToPolyLine((DRW_LWPolyline *)pDwgEntity, pAlteration);
			if (pNewEntity) {
				pNewEntity->SetParent(pLayer);
				pNewEntity->SetDwgColor(((DRW_LWPolyline *)pDwgEntity)->color, pDwgBlk);
			}
			break;
		case DRW::MTEXT: //CADTextToTextEntity
			break;
		case DRW::TEXT: //CADTextToTextEntity
			break;
		case DRW::CIRCLE:  //DwgPolylineBaseToPolyLine
			pNewEntity = DwgPolylineBaseToPolyLine((DRW_Circle *)pDwgEntity, pAlteration);
			if (pNewEntity) { pNewEntity->SetParent(pLayer); 
				pNewEntity->SetDwgColor(((DRW_Circle *)pDwgEntity)->color, pDwgBlk);
			}
			break;
		case DRW::ARC: //DwgPolylineBaseToPolyLine
			pNewEntity = DwgPolylineBaseToPolyLine((DRW_Arc *)pDwgEntity, pAlteration);
			if (pNewEntity) { pNewEntity->SetParent(pLayer); 
				pNewEntity->SetDwgColor(((DRW_Arc *)pDwgEntity)->color, pDwgBlk);
			}
			break;
		case DRW::HATCH: //ExtractDataFromCadInsert
			pNewEntity = DwgHatchToPolyLine((DRW_Hatch *)pDwgEntity, pAlteration);
			if (pNewEntity) pNewEntity->SetParent(pLayer);
			break;
		case DRW::INSERT: //ExtractDataFromCadInsert
			pNewEntity = DwgInsertToPolyLine((DRW_Insert *)pDwgEntity, pAlteration, pLayer);
			if (pNewEntity) { pNewEntity->SetParent(pLayer); 
				pNewEntity->SetDwgColor(((DRW_Insert *)pDwgEntity)->color, pDwgBlk);
			}
			break;
		case DRW::VIEWPORT: 
			break;
		default: 
			break;
		}
	}

	if (pNewEntity) 
	{
		((CMapLayer*)pLayer)->m_Entities.AddEntity(pNewEntity);		
	}
}
CEntity* CMapEngine::DwgLineToLineEntity(DRW_Line *pDwgLine, CPointAlteration *pAlteration) {
	Print_Debug(_T("DwgLineToLineEntity\r\n"));

	CLineEntity *pLine = new CLineEntity();
	

	CPointF ptStart(pDwgLine->basePoint.x, pDwgLine->basePoint.y);
	CPointF ptEnd(pDwgLine->secPoint.x, pDwgLine->secPoint.y);
	CPointF ptNewStart(0,0), ptNewEnd(0,0);

	GetArpRelativeCoordinates(&ptStart, &ptNewStart, pAlteration);
	GetArpRelativeCoordinates(&ptEnd, &ptNewEnd, pAlteration);

	CLineGeometry *pLineGeometry = new CLineGeometry(ptNewStart, ptNewEnd);
	//pLineGeometry->m_startPoint = ptNewStart;
	//pLineGeometry->m_endPoint = ptNewEnd;

	pLine->m_lstLineGeometries.push_back(pLineGeometry);
	return pLine;
};
CEntity* CMapEngine::DwgPolylineBaseToPolyLine(DRW_LWPolyline * pDwgEntity, CPointAlteration *pAlteration)
{	
	Print_Debug(_T("DwgPolylineBaseToPolyLine LWPolyline\r\n"));
	int iNumPt = pDwgEntity->vertlist.size();
	CPolylineEntity *pPolylineEntity = NULL;
	if (iNumPt >= 2) 
	{
		pPolylineEntity = new CPolylineEntity();	
		for (int i = 0; i < (iNumPt-1); i++) 
		{
			CPointF pointOne(pDwgEntity->vertlist[i]->x, pDwgEntity->vertlist[i]->y);
			CPointF pointTwo(pDwgEntity->vertlist[i+1]->x, pDwgEntity->vertlist[i+1]->y);
			double distance = sqrt(pow(pointTwo.X - pointOne.X, 2) + pow(pointTwo.Y - pointOne.Y, 2));
			while (i< iNumPt && distance < 3.0) //skip very closed points
			{
				if (++i < (iNumPt - 1)) {
					pointTwo.X = pDwgEntity->vertlist[i + 1]->x;
					pointTwo.Y = pDwgEntity->vertlist[i + 1]->y;
				}
				distance = sqrt(pow(pointTwo.X - pointOne.X, 2) + pow(pointTwo.Y - pointOne.Y, 2));
			}

			CPointF pointStart, pointEnd;
			GetArpRelativeCoordinates(&pointOne, &pointStart, pAlteration);
			GetArpRelativeCoordinates(&pointTwo, &pointEnd, pAlteration);

			CLineGeometry *pOneLine = new CLineGeometry(pointStart, pointEnd);
			pPolylineEntity->AddSegment(pOneLine);
		}

		if(pDwgEntity->flags == 1) //closed polygon, add line from last point to the first one
		{
			CPointF pointOne(pDwgEntity->vertlist[iNumPt-1]->x, pDwgEntity->vertlist[iNumPt -1]->y);
			CPointF pointTwo(pDwgEntity->vertlist[0]->x, pDwgEntity->vertlist[0]->y);

			CPointF pointStart, pointEnd;
			GetArpRelativeCoordinates(&pointOne, &pointStart, pAlteration);
			GetArpRelativeCoordinates(&pointTwo, &pointEnd, pAlteration);

			CLineGeometry *pOneLine = new CLineGeometry(pointStart, pointEnd);
			pPolylineEntity->AddSegment(pOneLine);
		}
	}
	else 
	{ 
		//Ignore this kind of PolyLine
		assert(0);
	}

	return pPolylineEntity;
}
CEntity* CMapEngine::DwgPolylineBaseToPolyLine(DRW_Arc * pDwgEntity, CPointAlteration *pAlteration)
{
	Print_Debug(_T("DwgPolylineBaseToPolyLine Arc\r\n"));
	CEntity* pEntity = NULL;

	bool bNewLineAdded = false;

	if (pDwgEntity) 
	{
		if (pDwgEntity->startAngle() > pDwgEntity->endAngle()) 
		{
			pDwgEntity->endangle += (D3DX_PI*2);
		}

		pEntity = new CPolylineEntity();

		double curAngle = pDwgEntity->startAngle();
		//first point
		double x1 = pDwgEntity->center().x + pDwgEntity->radious * cos(curAngle);
		double y1 = pDwgEntity->center().y + pDwgEntity->radious * sin(curAngle);

		//to make sure at least one Line added in case the startAngle and endAngle is very close or even equal each other.
		for (curAngle = pDwgEntity->startAngle() + Step_Degree_Arc; curAngle <= pDwgEntity->endAngle(); curAngle += Step_Degree_Arc)
		{
			//second point
			double x2 = pDwgEntity->center().x + pDwgEntity->radious * cos(curAngle);
			double y2 = pDwgEntity->center().y + pDwgEntity->radious * sin(curAngle);

			CPointF pt1(x1, y1);
			CPointF pt2(x2, y2);

			CPointF pointStart, pointEnd;
			GetArpRelativeCoordinates(&pt1, &pointStart, pAlteration);
			GetArpRelativeCoordinates(&pt2, &pointEnd, pAlteration);

			CLineGeometry *pOneLine = new CLineGeometry(pointStart, pointEnd);
			((CPolylineEntity*)pEntity)->AddSegment(pOneLine);

			//move first point to second point
			x1 = x2;
			y1 = y2;

			bNewLineAdded = true;
		}

		
		if (!bNewLineAdded) 
		{
			//simply add a new cross the radius in case no line added already
		}
	}

	return pEntity;
}

CEntity* CMapEngine::DwgPolylineBaseToPolyLine(DRW_Circle * pDwgEntity, CPointAlteration *pAlteration)
{
	Print_Debug(_T("DwgPolylineBaseToPolyLine Circle\r\n"));
	CEntity* pEntity = NULL;

	if (pDwgEntity)
	{
		pEntity = new CPolylineEntity();

		double curAngle = 0;
		//first point
		double x1 = pDwgEntity->basePoint.x + pDwgEntity->radious * cos(curAngle);
		double y1 = pDwgEntity->basePoint.y + pDwgEntity->radious * sin(curAngle);
		for (curAngle = Step_Degree_Arc; curAngle <= (2*D3DX_PI); curAngle += Step_Degree_Arc)
		{
			//second point
			double x2 = pDwgEntity->basePoint.x + pDwgEntity->radious * cos(curAngle);
			double y2 = pDwgEntity->basePoint.y + pDwgEntity->radious * sin(curAngle);

			CPointF pt1(x1, y1);
			CPointF pt2(x2, y2);

			CPointF pointStart, pointEnd;
			GetArpRelativeCoordinates(&pt1, &pointStart, pAlteration);
			GetArpRelativeCoordinates(&pt2, &pointEnd, pAlteration);

			CLineGeometry *pOneLine = new CLineGeometry(pointStart, pointEnd);
			((CPolylineEntity*)pEntity)->AddSegment(pOneLine);

			//move first point to second point
			x1 = x2;
			y1 = y2;
		}
	}

	return pEntity;
}

/*
Hatch: angle, rotation anale for all objects inside the current hatch
Hatch: scale, scale factor for all objects inside the current hatch

*/
CEntity* CMapEngine::DwgHatchToPolyLine(DRW_Hatch * pDwgEntity, CPointAlteration *pAlteration)
{
	Print_Debug(_T("DwgPolylineBaseToPolyLine Hatch\r\n"));
	CEntity* pEntity = NULL;

	return pEntity;
}

/*Insert: (block reference)
cadInsert.Point -             Reference Point from Image (0,0), used to place the Block in final drawing
cadInsert.Box.Center -        Block Actual Center Offset from Image, combined with internal scale and rotation, then outside offset;
cadInsert.Block.Box  -        relative pos from inside the block

from real CAD operation practise, CAD firstly make scale inside block per the scale factor, then apply offset and rotate in parent image.

*Real Example from QD 20190612 dwg
APRX, APRY:             Image Center:                               (9411, 9091)
cadInsert.Box.Center:   Block Actual Offset from Image Center:      (9888, 7201)
cadInsert.Point:        Block Ref Offset from Image Center:         (10182, 9154)
cadInsert.Block.Box.Center: Block Internal Center:                  (3674, -1814)
cadInsert.Block.Box.Width: Block width :                            129
cadInsert.Block.Box.Height: Block height:                           49
*/
CEntity* CMapEngine::DwgInsertToPolyLine(DRW_Insert * pDwgEntity, CPointAlteration *pAlteration, CMapLayer* pLayer)
{
	Print_Debug(_T("DwgPolylineBaseToPolyLine Insert\r\n"));
	CEntity* pEntity = NULL;

	CPointF ptOffset(pDwgEntity->basePoint.x, pDwgEntity->basePoint.y);
	CPointF ptRotateCenter(pDwgEntity->basePoint.x, pDwgEntity->basePoint.y);

	CPointAlteration TempAleration(ptOffset, pDwgEntity->angle, ptRotateCenter, pDwgEntity->xscale, pDwgEntity->yscale);

	//find block based on insert.name
	dx_ifaceBlock * pBlockEntities = FindBlock(pDwgEntity->name);
	if (pBlockEntities) 
	{
		ParseDwgToLayers(pBlockEntities, pLayer, &TempAleration);
	}
	else 
	{
		ASSERT(0);
	}

	//for each entity inside the Insert
	//ExtractDwgEntityData(pSubEntity, pLayer, &TempAleration);
	return pEntity;
}

void CMapEngine::GetArpRelativeCoordinates(CPointF *ptSrc, CPointF *ptDst, CPointAlteration *pAlteration)
{
	CPointF ptTemp;
	pAlteration->Alterate(ptSrc, &ptTemp);

	ptDst->X = ptTemp.X - m_ArpX;
	ptDst->Y = ptTemp.Y - m_ArpY;
}

dx_ifaceBlock* CMapEngine::FindBlock(std::string strBlock) 
{
	dx_ifaceBlock* rt = NULL;

	for (std::list<dx_ifaceBlock*>::const_iterator it = m_dxData.blocks.begin(); it != m_dxData.blocks.end(); it++) 
	{
		dx_ifaceBlock* tmpblk = *it;
		if (tmpblk->name == strBlock) 
		{
			rt = tmpblk;
			break;
		}
	}

	return rt;
}