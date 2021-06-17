#include "BasicLayer.h"
#include "GraphEngine.h"

/// Create a PolylineEntity object from a CADPolylineBase one.
/// The geometry of a CADPolylineBase is defined by a collection of points.
/// In order to improve the performance of display, the points are sampled
/// to limit the number of lines to be displayed. When the dx and dy between two
/// points is "small enough", the second point is discarded and the next point
/// of the list is evaluated, and so on. In any case the last point of the collection
/// is taken into account.
bool CBasicLayer::InitPolyLineBuffer()
{
	int itemPoints;
	int pointsCount = this->m_Entities.TotalPolylinePoints();

	if (m_pd3dDevice && (pointsCount>0))
	{
		// Release old buffers if needed
		SAFE_RELEASE(m_PolyLinePosBuffer);

		m_PolyLinePosBufferSize = pointsCount * sizeof(Vertex);

		//Create Vertex Buffer
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_PolyLinePosBufferSize,
			D3DUSAGE_WRITEONLY,
			CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&m_PolyLinePosBuffer, NULL)))
		{
			Print_Error(_T("InitPolyLineBuffer CreateVertexBuffer Failed."));
			return false;
		}

		// Lock them
		Vertex*  pInternalPositionVertexBuffer;
		WORD     index = 0;
		PWORD    pIndex;

		if (FAILED(m_PolyLinePosBuffer->Lock(0, m_PolyLinePosBufferSize, (void**)&pInternalPositionVertexBuffer, 0)))
		{
			Print_Error(_T("InitPolyLineBuffer Lock Failed."));
			return false;
		}

		for (std::vector<CPolylineEntity*>::const_iterator it = this->m_Entities.m_lstPolyLines.begin(); it != m_Entities.m_lstPolyLines.end(); it++)
		{
			CPolylineEntity* pPolyLine = *it; //std::vector<CLineGeometry*> m_lstLineGeometries
			for (std::vector<CLineGeometry*>::const_iterator it2 = pPolyLine->m_lstLineGeometries.begin(); it2 != pPolyLine->m_lstLineGeometries.end(); it2++)
			{
				CLineGeometry* pLineGeometry = *it2;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_startPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_startPoint.Y;
				pInternalPositionVertexBuffer->dColor = pPolyLine->m_Color;
				pInternalPositionVertexBuffer++;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_endPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_endPoint.Y;
				pInternalPositionVertexBuffer->dColor = pPolyLine->m_Color;
				pInternalPositionVertexBuffer++;
			}
		}

		m_PolyLinePosBuffer->Unlock();
		m_PolyLinePosBufferInitialized = true;
	}

	return true;
}

/// Creates an EllipseEntity object from a CADCircle one.
bool CBasicLayer::InitPolygonBuffer()
{
	int itemPoints;
	int pointsCount = this->m_Entities.TotalPolygonPoints();

	if (m_pd3dDevice && (pointsCount>0))
	{
		// Release old buffers if needed
		SAFE_RELEASE(m_PolygonPosBuffer);

		m_PolygonPosBufferSize = pointsCount * sizeof(Vertex);

		//Create Vertex Buffer
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_PolygonPosBufferSize,
			D3DUSAGE_WRITEONLY,
			CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&m_PolygonPosBuffer, NULL)))
		{
			Print_Error(_T("InitPolygonBuffer CreateVertexBuffer Failed."));
			return false;
		}

		// Lock them
		Vertex*  pInternalPositionVertexBuffer;
		WORD     index = 0;
		PWORD    pIndex;

		if (FAILED(m_PolygonPosBuffer->Lock(0, m_PolygonPosBufferSize, (void**)&pInternalPositionVertexBuffer, 0)))
		{
			Print_Error(_T("InitPolygonBuffer Lock Failed."));
			return false;
		}

		for (std::vector<CPolygonEntity*>::const_iterator it = this->m_Entities.m_lstPolygons.begin(); it != m_Entities.m_lstPolygons.end(); it++)
		{
			CPolygonEntity* pPolygon = *it;
			for (std::vector<CPointF*>::const_iterator it2 = pPolygon->m_lstVertices.begin(); it2 != pPolygon->m_lstVertices.end(); it2++)
			{
				CPointF* pPoint = *it2;

				pInternalPositionVertexBuffer->X = pPoint->X;
				pInternalPositionVertexBuffer->Y = pPoint->Y;
				pInternalPositionVertexBuffer->dColor = pPolygon->m_Color;
				pInternalPositionVertexBuffer++;
			}
		}

		m_PolygonPosBuffer->Unlock();
		m_PolygonPosBufferInitialized = true;
	}

	return true;
}

/*handle single lines*/
/// Create a LineEntity object from a CADLine one.
/// </summary>
bool CBasicLayer::InitLineListsBuffer()
{
	int itemPoints;
	int pointsCount = this->m_Entities.TotalLinePoints();

	if (m_pd3dDevice && (pointsCount>0))
	{
		// Release old buffers if needed
		SAFE_RELEASE(m_LineListPosBuffer);

		m_LineListPosBufferSize = pointsCount * sizeof(Vertex);

		//Create Vertex Buffer
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_LineListPosBufferSize,
			D3DUSAGE_WRITEONLY,
			CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&m_LineListPosBuffer, NULL)))
		{
			Print_Error(_T("InitLineListsBuffer CreateVertexBuffer Failed."));
			return false;
		}

		// Lock them
		Vertex*  pInternalPositionVertexBuffer;
		WORD     index = 0;
		PWORD    pIndex;

		if (FAILED(m_LineListPosBuffer->Lock(0, m_LineListPosBufferSize, (void**)&pInternalPositionVertexBuffer, 0)))
		{
			Print_Error(_T("InitLineListsBuffer Lock Failed."));
			return false;
		}

		for (std::vector<CLineEntity*>::const_iterator it = this->m_Entities.m_lstLines.begin(); it != m_Entities.m_lstLines.end(); it++)
		{
			CLineEntity* pLine = *it;
			for (std::vector<CLineGeometry*>::const_iterator it2 = pLine->m_lstLineGeometries.begin(); it2 != pLine->m_lstLineGeometries.end(); it2++)
			{
				CLineGeometry* pLineGeometry = *it2;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_startPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_startPoint.Y;
				pInternalPositionVertexBuffer->dColor = pLine->m_Color;
				pInternalPositionVertexBuffer++;

				pInternalPositionVertexBuffer->X = pLineGeometry->m_endPoint.X;
				pInternalPositionVertexBuffer->Y = pLineGeometry->m_endPoint.Y;
				pInternalPositionVertexBuffer->dColor = pLine->m_Color;
				pInternalPositionVertexBuffer++;
			}
		}

		m_LineListPosBuffer->Unlock();
		m_LineStripsBuffersInitialized = true;
	}

	return true;
}

void CBasicLayer::DoDraw()
{
	//TBD draw Polylines
	if (m_PolyLinePosBuffer && m_pd3dDevice) {
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pEngine->matWorld));
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &(m_pEngine->matView));
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &(m_pEngine->matProjection));

		m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetStreamSource(0, m_PolyLinePosBuffer, 0, sizeof(Vertex));
		m_pd3dDevice->SetFVF(CUSTOMVERTEX);

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_PolyLinePosBufferSize / (sizeof(Vertex) * 2));
	}

	//draw Line strips here
	if (m_LineListPosBuffer && m_pd3dDevice) {
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pEngine->matWorld));
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &(m_pEngine->matView));
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &(m_pEngine->matProjection));

		m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetStreamSource(0, m_LineListPosBuffer, 0, sizeof(Vertex));
		m_pd3dDevice->SetFVF(CUSTOMVERTEX);

		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_LineListPosBufferSize / (sizeof(Vertex) * 2));
	}

#if 0
	//Draw Polygon
	if (m_PolygonPosBuffer && m_pd3dDevice)
	{
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pEngine->matWorld));
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &(m_pEngine->matView));
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &(m_pEngine->matProjection));

		m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetStreamSource(0, m_PolygonPosBuffer, 0, sizeof(Vertex));
		m_pd3dDevice->SetFVF(CUSTOMVERTEX);

		//TBD: try the D3DPT_TRIANGLELIST as HMI
		m_pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, m_PolygonPosBufferSize / sizeof(Vertex));
	}


	//TBD draw text 
#endif //0

}