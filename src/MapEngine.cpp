﻿//FILE: MapEngine.cpp
#include "stdafx.h"
#include "MapEngine.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <math.h>

CMapEngine::CMapEngine(HWND hwnd):
m_hwnd(hwnd),
m_UsedAdapter(D3DADAPTER_DEFAULT),
m_deviceType(D3DDEVTYPE_HAL),
m_ViewCount(0),
m_AdaptersCount(0),
m_IsDirectXAvailable(true),
m_pD3D(NULL),
m_pD3DEx(NULL),
m_pd3dDevice(NULL),
m_pd3dDeviceEx(NULL)
{
	this->m_ArpX = OFFSET_X;
	this->m_ArpY = OFFSET_Y;

	InitD3Devices();

	m_cxOffset = 0;
	m_cyOffset = 0;
	m_scale = 2.0;
	m_rotate = 0;
	m_viewWidth = 1024;
	m_viewHeight = 768;

	m_FontSize = 72;
};

CMapEngine::~CMapEngine() {
	ClearMapLayers();

	SAFE_RELEASE(m_pVertexBuffer);

	DestroyResources();
};
void CMapEngine::DestroyResources() 
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pd3dDeviceEx);

	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DEx);
}
bool CMapEngine::InitD3Devices()
{
	EnsureD3DObjects();
	EnsureD3DDevices();

	m_pd3dDevice->CreateVertexBuffer(MAX_VERTEXS_ENGINE * sizeof(Vertex),
		0, CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer,
		NULL);

	UpdateMatrix();
	return true;
}
HRESULT CMapEngine::EnsureD3DObjects() 
{
	HRESULT hr = S_OK;
	if (m_pD3D == NULL)
	{
		if (!D3DXCheckVersion(D3D_SDK_VERSION, D3DX_SDK_VERSION))
		{
			hr = E_FAIL;
			Print_Error(_T("Wrong directX version !"));
		}
		else {
			HMODULE hD3D = LoadLibrary(TEXT("d3d9.dll"));
			if (hD3D != NULL)
			{
				DIRECT3DCREATE9EXFUNCTION pfnCreate9Ex = (DIRECT3DCREATE9EXFUNCTION)GetProcAddress(hD3D, "Direct3DCreate9Ex");
				if (pfnCreate9Ex != NULL)
				{
					hr = pfnCreate9Ex(D3D_SDK_VERSION, &m_pD3DEx);
					if (FAILED(hr))
					{
						Print_Error(_T("Unable to retrieve Direct3DCreate9Ex address !"));
					}
					else {
						hr = m_pD3DEx->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void**>(&m_pD3D));
						if (FAILED(hr))
						{
							Print_Error(_T("Unable to get IDirect3D9 interface with QueryInterface.\n"));
						}
					}
				}
				else
				{
					m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
					if (m_pD3D == NULL)
					{
						hr = E_FAIL;
						Print_Error(_T("Direct3DCreate9 failed !"));
					}
				}
			}
			else
			{
				hr = E_FAIL;
				Print_Error(_T("Loading Library d3d9.dll failed !"));
			}
			
			if (hD3D != NULL)
			{
				FreeLibrary(hD3D);
				hD3D = NULL;
			}
		}
	}
	return hr;
}
HRESULT CMapEngine::EnsureD3DDevices() 
{
	HRESULT hr = S_OK;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;

    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferHeight = 0;
	d3dpp.BackBufferWidth = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.hDeviceWindow = m_hwnd;

	// D3DMULTISAMPLE_2_SAMPLES only with DirectX9Ex interface
	d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


	DWORD dwVertexProcessing;

	// Try to get the hardware capabilities
	HRESULT getDevhr = m_pD3D->GetDeviceCaps(m_UsedAdapter, D3DDEVTYPE_HAL, &m_DeviceCaps);
	if (getDevhr == D3DERR_NOTAVAILABLE)
	{
		// Or the reference renderer
		getDevhr = m_pD3D->GetDeviceCaps(m_UsedAdapter, D3DDEVTYPE_REF, &m_DeviceCaps);
		m_deviceType = D3DDEVTYPE_REF;
	}
	hr = getDevhr;
	if (FAILED(hr))
	{
		m_IsDirectXAvailable = false;
		goto Cleanup;
	}

	if ((m_DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		dwVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		dwVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (m_pD3DEx)
	{
		IDirect3DDevice9 *pd3dDevice = NULL;
		HRESULT hrCreate =
			m_pD3DEx->CreateDeviceEx(
				m_UsedAdapter,  //D3DADAPTER_DEFAULT
				m_deviceType,   //D3DDEVTYPE_HAL
				m_hwnd,
				dwVertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
				&d3dpp,
				NULL,
				&m_pd3dDeviceEx
			);
		IFC(hrCreate);
		IFC(m_pd3dDeviceEx->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void**>(&m_pd3dDevice)));
	}
	else
	{
		assert(m_pD3D);
		HRESULT hrCreate =
			m_pD3D->CreateDevice(
				m_UsedAdapter,
				m_deviceType,
				m_hwnd,
				dwVertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
				&d3dpp,
				&m_pd3dDevice
			);
		IFC(hrCreate);
	}

	if (m_pd3dDevice)
	{
		D3DXCreateSprite(m_pd3dDevice, &m_LabelsSprite);
		D3DXCreateFont(m_pd3dDevice, m_FontSize, 0, 1, 1, FALSE, DEFAULT_CHARSET, OUT_STROKE_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_LabelsFont);

	}

	IFC(m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
	IFC(m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE));

Cleanup:
	return hr;
}
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
	HRESULT hr = 0;
	if(m_pd3dDevice)
	{
		//--clear the window to black 
		hr = m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		hr = m_pd3dDevice->BeginScene(); //--开始通知显卡要进行渲染 
#if 1
		DrawLine(m_pd3dDevice, -10, 0, 10, 0, D3DCOLOR_XRGB(255, 255, 255)); //X-Asile
		DrawLine(m_pd3dDevice, 0, -10, 0, 10, D3DCOLOR_XRGB(255, 255, 255)); //Y-Asile   
																																	  //hr = DrawRect(m_pd3dDevice, -5, -5, 10, 10, D3DCOLOR_XRGB(255, 0, 0)); //Center Rect
		DrawCircle(m_pd3dDevice, 0, 0, 10, 10, D3DCOLOR_XRGB(255, 0, 0));
#endif //0
		//draw each layers
		for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMayLayers.begin(); it != m_lstMayLayers.end(); it++) 
		{
			if (IsLayerVisible((*it)->m_strDisplayName)) {
				(*it)->DoDraw(); 
			}
		}

		m_pd3dDevice->EndScene();  //--结束图形的渲染 
		m_pd3dDevice->Present(NULL, NULL, NULL, NULL);//--翻页 
	}
}

void CMapEngine::SetViewSize(int w, int h) 
{
	m_viewWidth = w;
	m_viewHeight = h;

	UpdateMatrix();
}

void CMapEngine::SetZoomDelta(float zDelta) 
{
	m_scale += zDelta;

	if (m_scale < 0.01)
	{
		m_scale = 0.01;
	}
	else if (m_scale > 100.0)
	{
		m_scale = 30;
	}

	UpdateMatrix();
}

void CMapEngine::SetTransform(int cx, int cy) 
{
	m_cxOffset = cx;
	m_cyOffset = cy;
	UpdateMatrix();
}

void CMapEngine::UpdateMatrix() 
{
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTranslate);
	D3DXMatrixIdentity(&matRotation);

	D3DXMatrixScaling(&matScale, m_scale, m_scale, 1.0);        //Scale

																//Rotate    
	D3DXMatrixRotationZ(&matRotation, m_rotate * DEGTORAD);
	D3DXMatrixMultiply(&matWorld, &matScale, &matRotation);     //matWorld=matScale*matRotation 

	D3DXMatrixTranslation(&matTranslate, m_cxOffset, m_cyOffset, 0.0f);     //Translate
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTranslate);                // matWorld=matWorld *matTranslate  

																			//D3DXMatrixMultiply(&matWorld, &matTranslate, &matScale);
																			//resultm = scaling*mirror*rotm*transm;
																			//matWorld = matScale * matTranslate;

	D3DXMatrixIdentity(&matView);
#if 0
	/*matView._31 = -matView._31;
	matView._32 = -matView._32;
	matView._33 = -matView._33;
	matView._34 = -matView._34;*/
#else
	D3DXVECTOR3 vEye(0.0f, 0.0f, -1000.0f);         //摄像机的位置  
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);              //观察点的位置  
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);              //向上的向量  
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); //计算出取景变换矩阵  
#endif 

	D3DXMatrixIdentity(&matProjection);
#if 0
	//创建一个用户定制的左手坐标系的正交投影矩阵
	D3DXMatrixOrthoOffCenterRH(&matProjection,
		m_cxOffset - m_viewWidth*m_scale*0.5f,
		m_cxOffset + m_viewWidth*m_scale*0.5f,
		m_cyOffset - m_viewHeight*m_scale*0.5f,
		m_cyOffset + m_viewHeight*m_scale*0.5f,
		-1.0f,
		1.0f);
#else
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),                           // the horizontal field of view
		(FLOAT)m_viewWidth / (FLOAT)m_viewHeight,   // aspect ratio
		1.0f,    // the near view-plane
		100000.0f);    // the far view-plane
#endif 
					   /*
					   D3DXMatrixLookAtLH(&matView,
					   &D3DXVECTOR3(0.0f, 0.0f, 50.0f),    //vEye: the camera position
					   &D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //vAt: looking towards the origin
					   &D3DXVECTOR3(0.0f, 1.0f, 0.0f));    //vUp: The "up" direction is the positive direction on the y-axis

					   D3DXMatrixPerspectiveFovLH(&matProjection,
					   D3DXToRadian(45),                           // the horizontal field of view
					   (FLOAT)m_viewWidth / (FLOAT)m_viewHeight,   // aspect ratio
					   1.0f,    // the near view-plane
					   1000.0f);    // the far view-plane
					   */
}

void CMapEngine::DrawCircle(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT RadiusW, FLOAT RadiusH, D3DCOLOR dColor)
{
	int cnt = 0;

	CONST INT NUMPOINTS = 50;
	CONST FLOAT Angle = (2.0f * D3DX_PI) / NUMPOINTS;

	Vertex* pVertexList;

	m_pVertexBuffer->Lock(0, 0, (PVOID*)&pVertexList, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);

	for (INT i = 0; i <= NUMPOINTS; i++)
	{
		FLOAT CurAngle = (FLOAT)i * Angle;
		FLOAT DrawPosX = (X + RadiusW * cos(CurAngle));
		FLOAT DrawPosY = (Y - RadiusH * sin(CurAngle));

		pVertexList[i].X = DrawPosX;
		pVertexList[i].Y = DrawPosY;
		pVertexList[i].Z = 0.0f;
#if (CUSTOMVERTEX & D3DFVF_XYZRHW)
		pVertexList[i].RHW = 1.0f;
#endif
		pVertexList[i].dColor = dColor;
	}

	m_pVertexBuffer->Unlock();

	pDevice->SetTexture(0, NULL);
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	pDevice->SetFVF(CUSTOMVERTEX);

	pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, NUMPOINTS);
}

void CMapEngine::DrawLine(LPDIRECT3DDEVICE9 pDevice, FLOAT X, FLOAT Y, FLOAT X2, FLOAT Y2, D3DCOLOR dColor)
{
	Vertex* pVertexList;

	m_pVertexBuffer->Lock(0, 0, (PVOID*)&pVertexList, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);

	pVertexList[0].X = X;
	pVertexList[0].Y = Y;
	pVertexList[0].Z = 0.0f;
#if (CUSTOMVERTEX & D3DFVF_XYZRHW)
	pVertexList[0].RHW = 1.0f;
#endif
	pVertexList[0].dColor = dColor;

	pVertexList[1].X = X2;
	pVertexList[1].Y = Y2;
	pVertexList[1].Z = 0.0f;
#if (CUSTOMVERTEX & D3DFVF_XYZRHW)
	pVertexList[1].RHW = 1.0f;
#endif
	pVertexList[1].dColor = dColor;

	m_pVertexBuffer->Unlock();

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pDevice->SetTransform(D3DTS_VIEW, &matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProjection);

	pDevice->SetTexture(0, NULL);
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	pDevice->SetFVF(CUSTOMVERTEX);

	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}


CMapLayer* CMapEngine::FindOrNewMapLayer(std::string  strName)
{
	CMapLayer* pLayer = NULL;
	for (std::vector<CMapLayer*>::const_iterator it = m_lstMayLayers.begin(); it != m_lstMayLayers.end(); ++it) 
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
		this->m_lstMayLayers.push_back(pLayer);
	}

	return pLayer;
}

void CMapEngine::ClearMapLayers() 
{
	for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMayLayers.begin(); it != m_lstMayLayers.end(); it++)
	{
		CMapLayer* pitem = *it;
		delete pitem;
	}
	m_lstMayLayers.clear();
}

bool CMapEngine::ParseDwgToLayers(dx_ifaceBlock* pBlockEntities, CBasicLayer* pLayer, CPointAlteration *pAlteration)
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
	for (std::vector<CMapLayer*>::const_iterator it = this->m_lstMayLayers.begin(); it != this->m_lstMayLayers.end(); it++) 
	{
		(*it)->InitLineListsBuffer();
		(*it)->InitPolyLineBuffer();
		(*it)->InitPolygonBuffer();
	}
	return rt;
}

void  CMapEngine::ExtractDwgEntityData(DRW_Entity *pDwgEntity, CBasicLayer* pLayer, CPointAlteration *pAlteration, DRW_Block* pDwgBlk) {
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
CEntity* CMapEngine::DwgInsertToPolyLine(DRW_Insert * pDwgEntity, CPointAlteration *pAlteration, CBasicLayer* pLayer)
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