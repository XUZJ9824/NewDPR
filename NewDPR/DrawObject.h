#pragma once
#include "EntityDefs.h"

class CDrawObject
{
public:
	CDrawObject();
	~CDrawObject();
};

class CDrawObjectLine : public CDrawObject, CLineGeometry
{
public:
	CDrawObjectLine();
	CDrawObjectLine(CPointF stPt, CPointF endPt);
	~CDrawObjectLine();

};

