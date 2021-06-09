#pragma once

//FILE: EntityDefs.h
#include "commonDefs.h"

#include <vector>

using namespace std;

//
// Summary:
//     Specifies style information applied to text.
typedef enum
{
	//
	// Summary:
	//     Normal text.
	Regular = 0,
	//
	// Summary:
	//     Bold text.
	Bold = 1,
	//
	// Summary:
	//     Italic text.
	Italic = 2,
	//
	// Summary:
	//     Underlined text.
	Underline = 4,
	//
	// Summary:
	//     Text with a line through the middle.
	Strikeout = 8
}FontStyle;

typedef struct 
{
	int				Size;
	std::wstring	FamilyName;
	FontStyle		Style;
}FontData;

typedef enum 
{
	eLineGeometry = 1,
	eLineEntity,
	ePolylineEntity,
	ePolygonEntity,
	eTextEntity,
	eUnknow
}E_EntityType;

class CPointF 
{
public:
	CPointF();
	CPointF(double fx, double fy);	
	~CPointF();

	double X, Y, Z;
	bool m_bIsEmpty;
};

class CBasicLayer;
class CEntity 
{
public:
	CEntity();
	~CEntity();

	CBasicLayer * m_pParentLayer;
	D3DCOLOR m_Color;
	E_EntityType m_Type;

	void SetParent(CBasicLayer * pParentLayer) { 
		m_pParentLayer = pParentLayer;  
	}

	void SetColor(D3DCOLOR color) { m_Color = color; }
	virtual inline int PointsCount() { return 0; }
};


/// <summary>
/// Represents a basic line between 2 points.
/// </summary>
class CLineGeometry : public CEntity
{
public:
	CLineGeometry();
	CLineGeometry(CPointF stPt, CPointF endPt);

	~CLineGeometry();

	CPointF m_startPoint;
	CPointF m_endPoint;

	virtual inline int PointsCount() { return 2; }
};

/// <summary>
/// This class is the graphic representation of a segment
/// of line defined by its start point, end point,
/// stroke width and stroke colour.
/// </summary>
class CLineEntity : public CEntity
{
public: CLineEntity();
		~CLineEntity();

		std::vector<CLineGeometry*> m_lstLineGeometries;

		void SetDwgColor(int iDwgColor);

		virtual inline int PointsCount() 
		{
			int cnt = 0;
			for (std::vector<CLineGeometry*>::const_iterator it = m_lstLineGeometries.begin(); it != m_lstLineGeometries.end(); it++)
			{
				cnt += (*it)->PointsCount();
			}

			return cnt;
		}
};

/// <summary>
/// This class represents a set of contiguous
/// line segments.
/// </summary>
class CPolylineEntity : public CEntity {
public: CPolylineEntity();
		~CPolylineEntity();

		void AddSegment(CLineGeometry *pSegment);
		std::vector<CLineGeometry*> m_lstLineGeometries;

		virtual inline int PointsCount()
		{
			int cnt = 0;
			for (std::vector<CLineGeometry*>::const_iterator it = m_lstLineGeometries.begin(); it != m_lstLineGeometries.end(); it++)
			{
				cnt += (*it)->PointsCount();
			}

			return cnt;
		}

};

/// <summary>
/// This class represents a Polygon defined by
/// a list of vertices, stroke width and stroke colour.
/// It also offers the capability to tesselate the
/// polygon into a set of triangles.
/// </summary>
class CPolygonEntity : public CEntity {
public: CPolygonEntity();
		~CPolygonEntity();

		/// <summary>
		/// Returns true if this Polygon is a valid one,
		/// that means it contains at least 3 vertices.
		/// </summary>
		bool IsTriangulated() 
		{
			if (PointsCount() >= 3) return true;
			else return false;
		}

		void AddVertex(CPointF *point) 
		{
			m_lstVertices.push_back(point);
		}

		std::vector<CPointF*> m_lstVertices;

		virtual inline int PointsCount()
		{
			return m_lstVertices.size();
		}
};

class CTextEntity : public CEntity {
public: CTextEntity();
		~CTextEntity();

		std::wstring m_str;
		CPointF      m_stPoint;
		float        m_Angle;
		float        m_width;
		float        m_height;
		FontData	 m_font;
};

/// <summary>
/// The graphic entities to display the airport map.
/// Each layer of the map has its own instance of AirportMapEntities.
/// </summary>
class CAirportMapEntities 
{
public: 
	CAirportMapEntities();
	~CAirportMapEntities();

	bool AddEntity(CEntity *pEntity);
	void ClearEntity();

	std::vector<CLineEntity*>		m_lstLines;
	std::vector<CPolygonEntity*>	m_lstPolygons;
	std::vector<CPolylineEntity*> m_lstPolyLines;
	std::vector<CTextEntity*>     m_lstTexts;

	inline int TotalLinePoints() 
	{
		int cnt = 0;
		for (std::vector<CLineEntity*>::const_iterator it = m_lstLines.begin(); it != m_lstLines.end(); it++) 
		{
			cnt += (*it)->PointsCount();
		}
		return cnt;
	}

	inline int TotalPolygonPoints()
	{
		int cnt = 0;
		for (std::vector<CPolygonEntity*>::const_iterator it = m_lstPolygons.begin(); it != m_lstPolygons.end(); it++)
		{
			cnt += (*it)->PointsCount();
		}
		return cnt;
	}

	inline int TotalPolylinePoints()
	{
		int cnt = 0;
		for (std::vector<CPolylineEntity*>::const_iterator it = m_lstPolyLines.begin(); it != m_lstPolyLines.end(); it++)
		{
			cnt += (*it)->PointsCount();
		}
		return cnt;
	}

	inline int PointsCountTexts()
	{
		int cnt = 0;
		for (std::vector<CTextEntity*>::const_iterator it = m_lstTexts.begin(); it != m_lstTexts.end(); it++)
		{
			cnt += (*it)->PointsCount();
		}
		return cnt;
	}
};

class CPointAlteration
{
public:
	CPointAlteration();
	CPointAlteration(CPointF offset, float angle, CPointF center, float scaleX, float scaleY);
	CPointAlteration(float angle, CPointF center);

	~CPointAlteration();

	void Alterate(CPointF *ptSrc, CPointF *ptDst);	

	CPointF m_Offset;
	float   m_Angle;
	float   m_ScaleX;
	float   m_ScaleY;
	CPointF m_Center;
};


