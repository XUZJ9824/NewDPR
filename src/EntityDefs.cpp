#include "EntityDefs.h"
#include <math.h>
#include <iostream>
#include "MapLayer.h"

CPointF::CPointF() :
	X(0), Y(0), Z(0), m_bIsEmpty(true)
{
	this->m_bIsEmpty = true;
}

CPointF::CPointF(double fx, double fy):
	X(fx), Y(fy), Z(0), m_bIsEmpty(false)
{
}

CPointF::~CPointF() {};

CEntity::CEntity():
	m_Color(D3DCOLOR_XRGB(255,0,0)),
	m_pParentLayer(NULL)
{}
CEntity::~CEntity() {};

CLineGeometry::CLineGeometry() 
{
	Print_Debug(_T("New CLineGeometry\r\n"));
}
CLineGeometry::CLineGeometry(CPointF stPt, CPointF endPt)
{
	Print_Debug(_T("New CLineGeometry(%.3f,%.3f) to (%.3f,%.3f)\r\n"),
		stPt.X, stPt.Y,
		endPt.X, endPt.Y);

	m_startPoint = stPt;
	m_endPoint = endPt;
	m_Type = eLineGeometry;
};

CLineGeometry::~CLineGeometry() 
{	
	cout << "Delete CLineGeometry" << std::endl;
}

CLineEntity::CLineEntity()	
{
	m_Type = eLineEntity;
	Print_Debug(_T("New CLineEntity\r\n"));
};

CLineEntity::~CLineEntity() 
{
	cout << "Delete CLineEntity" << std::endl;

	for (std::vector<CLineGeometry*>::const_iterator it = m_lstLineGeometries.begin(); it != m_lstLineGeometries.end(); it++)
	{
		CLineGeometry *pitem = *it;
		delete pitem;		
	}
	m_lstLineGeometries.clear();
};

//Ref to HMI code GetColorCode
void CEntity::SetDwgColor(int iDwgColor, DRW_Block* pDwgBlk)
{
	unsigned short r = 255, g = 255, b = 255;
	if (m_pParentLayer) 
	{
		if (iDwgColor != 256) /*BYLAYER*/ 
		{
			if (iDwgColor != 0) /*BYBLOCK*/ 
			{				
				GetColorByIndex(iDwgColor, r, g, b);
				this->m_Color = D3DCOLOR_XRGB(r, g, b);
			}
			else 
			{
				if(pDwgBlk && (pDwgBlk->color!=0) && (pDwgBlk->color != 256))
				{
					GetColorByIndex(pDwgBlk->color, r, g, b);
				}
				else 
				{
					this->m_Color = D3DCOLOR_XRGB(255, 255, 255);
				}
				
			}
		}
		else 
		{
			iDwgColor = m_pParentLayer->color;
			GetColorByIndex(iDwgColor, r, g, b);
			this->m_Color = D3DCOLOR_XRGB(r, g, b);
		}
	}
}

CPolygonEntity::CPolygonEntity()	
{
	Print_Debug(_T("New CPolygonEntity\r\n"));
	m_Type = ePolygonEntity;
}

CPolygonEntity::~CPolygonEntity() 
{
	cout << "Delete CPolygonEntity" << std::endl;

	for (std::vector<CPointF*>::const_iterator it = m_lstVertices.begin(); it != m_lstVertices.end(); it++)
	{
		CPointF *pitem = *it;
		delete pitem;
	}
	m_lstVertices.clear();
}

CPolylineEntity::CPolylineEntity()	
{
	Print_Debug(_T("New CPolylineEntity\r\n"));
	m_Type = ePolylineEntity;
};
CPolylineEntity::~CPolylineEntity() 
{
	cout << "Delete CPolylineEntity" << std::endl;

	std::vector<CLineGeometry*> m_lstLines;

	for (std::vector<CLineGeometry*>::const_iterator it = m_lstLines.begin(); it != m_lstLines.end(); it++)
	{
		CLineGeometry *pitem = *it;
		delete pitem;
	}
	m_lstLines.clear();
};
void CPolylineEntity::AddSegment(CLineGeometry *pSegment)
{
	m_lstLineGeometries.push_back(pSegment);
}

CTextEntity::CTextEntity()	
{
	Print_Debug(_T("New CTextEntity\r\n"));
	m_Type = eTextEntity;
};
CTextEntity::~CTextEntity()
{
	cout << "Delete CTextEntity" << std::endl;
}

CAirportMapEntities::CAirportMapEntities() {};
CAirportMapEntities::~CAirportMapEntities() 
{
	for (std::vector<CLineEntity*>::const_iterator it = m_lstLines.begin(); it != m_lstLines.end(); it++)
	{
		CLineEntity *pitem = *it;
		delete pitem;
	}
	m_lstLines.clear();

	for (std::vector<CPolygonEntity*>::const_iterator it = m_lstPolygons.begin(); it != m_lstPolygons.end(); it++)
	{
		CPolygonEntity *pitem = *it;
		delete pitem;
	}
	m_lstPolygons.clear();

	for (std::vector<CPolylineEntity*>::const_iterator it = m_lstPolyLines.begin(); it != m_lstPolyLines.end(); it++)
	{
		CPolylineEntity *pitem = *it;
		delete pitem;
	}
	m_lstPolyLines.clear();

	for (std::vector<CTextEntity*>::const_iterator it = m_lstTexts.begin(); it != m_lstTexts.end(); it++)
	{
		CTextEntity *pitem = *it;
		delete pitem;
	}
	m_lstTexts.clear();
};

bool CAirportMapEntities::AddEntity(CEntity *pEntity) 
{
	bool rt = false;

	switch (pEntity->m_Type) 
	{
		case eLineGeometry: 
			this->m_lstLines.push_back((CLineEntity*)pEntity);
			break;

		case eLineEntity: 
			this->m_lstLines.push_back((CLineEntity*)pEntity);
			break;

		case ePolylineEntity: 
			this->m_lstPolyLines.push_back((CPolylineEntity*)pEntity);			
			break;

		case ePolygonEntity: 
			this->m_lstPolygons.push_back((CPolygonEntity*)pEntity);
			break;

		case eTextEntity: 
			this->m_lstTexts.push_back((CTextEntity*)pEntity);
			break;

		default: 
			delete pEntity;
			break;
	}
	return rt;
};

CPointAlteration::CPointAlteration() :
	m_Offset(0, 0), m_Center(0,0)
{
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_Angle = 0.0f;	
};
CPointAlteration::CPointAlteration(CPointF offset, float angle, CPointF center, float scaleX, float scaleY) 
{
	m_Offset = offset;
	m_ScaleX = scaleX;
	m_ScaleY = scaleY;
	m_Angle = angle;
	m_Center = center;
};
CPointAlteration::CPointAlteration(float angle, CPointF center):
	m_Offset(0,0)
{
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_Angle = angle;
	m_Center = center;
};

CPointAlteration::~CPointAlteration()
{
}

void CPointAlteration::Alterate(CPointF *ptSrc, CPointF *ptDst)
{
	CPointF outputPoint(ptSrc->X * m_ScaleX, ptSrc->Y * m_ScaleY);
	outputPoint.X += m_Offset.X;
	outputPoint.Y += m_Offset.Y;

	outputPoint.X -= m_Center.X;
	outputPoint.Y -= m_Center.Y;

	//save x temporarily because we need the original X to calculate Y aswell
	double tmpx = cos(m_Angle) * outputPoint.X + -sin(m_Angle) * outputPoint.Y;
	outputPoint.Y = sin(m_Angle) * outputPoint.X + cos(m_Angle) * outputPoint.Y;
	outputPoint.X = tmpx;

	outputPoint.X += m_Center.X;
	outputPoint.Y += m_Center.Y;

	*ptDst = outputPoint;
}