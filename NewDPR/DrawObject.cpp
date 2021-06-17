#include "stdafx.h"
#include "DrawObject.h"


CDrawObject::CDrawObject()
{}


CDrawObject::~CDrawObject()
{}


CDrawObjectLine::CDrawObjectLine()
{
	m_Type = eDrawLine;
}

CDrawObjectLine::CDrawObjectLine(CPointF stPt, CPointF endPt) :
	CLineGeometry(stPt, endPt)
{

}

CDrawObjectLine::~CDrawObjectLine()
{
}
