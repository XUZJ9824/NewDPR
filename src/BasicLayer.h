#pragma once
//FIlE: BasicLayer.h

#include "commonDefs.h"
#include "EntityDefs.h"
#include "dx_data.h"

#include "DxErr.h"
#include <vector>
#include <list>
#include <map>

typedef enum
{
	E_LAYER_MAP,
	E_LAYER_DRAWING,
	E_LAYER_LIGHT,
	E_LAYER_UNKNOWN
}eLayerType;

class CMapEngine;
/*basic common Layer*/
class CBasicLayer
{
public:
	CBasicLayer() {
		m_layerType = E_LAYER_UNKNOWN;
	};
	~CBasicLayer() {};

public:
	enum DRW_LW_Conv::lineWidth       lWeight;
	int                               color; /*!< layer color, code 62 , default BYLAYER (256)*/
	eLayerType                        m_layerType;
};