/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <iostream>
#include <algorithm>
#include "dx_iface.h"
#include "libdwgr.h"
#include "libdxfrw.h"
#include "commonDefs.h"

bool dx_iface::fileImport(const std::string& fileI, dx_data *fData){
    unsigned int found = fileI.find_last_of(".");
    std::string fileExt = fileI.substr(found+1);
    std::transform(fileExt.begin(), fileExt.end(),fileExt.begin(), ::toupper);
    cData = fData;
    currentBlock = cData->mBlock;

    if (fileExt == "DXF"){
        //loads dxf
        dxfRW* dxf = new dxfRW(fileI.c_str());
        bool success = dxf->read(this, false);
        delete dxf;
        return success;
    } else if (fileExt == "DWG"){
        //loads dwg
        dwgR* dwg = new dwgR(fileI.c_str());
        bool success = dwg->read(this, false);
        delete dwg;
        return success;
    }
    std::cout << "file extension can be dxf or dwg" << std::endl;
    return false;
}

bool dx_iface::fileExport(const std::string& file, DRW::Version v, bool binary, dx_data *fData){
    cData = fData;
    dxfW = new dxfRW(file.c_str());
    bool success = dxfW->write(this, v, binary);
    delete dxfW;
    return success;

}

void dx_iface::writeEntity(DRW_Entity* e){
    switch (e->eType) {
    case DRW::POINT:
        dxfW->writePoint(static_cast<DRW_Point*>(e));
        break;
    case DRW::LINE:
        dxfW->writeLine(static_cast<DRW_Line*>(e));
        break;
    case DRW::CIRCLE:
        dxfW->writeCircle(static_cast<DRW_Circle*>(e));
        break;
    case DRW::ARC:
        dxfW->writeArc(static_cast<DRW_Arc*>(e));
        break;
    case DRW::SOLID:
        dxfW->writeSolid(static_cast<DRW_Solid*>(e));
        break;
    case DRW::ELLIPSE:
        dxfW->writeEllipse(static_cast<DRW_Ellipse*>(e));
        break;
    case DRW::LWPOLYLINE:
        dxfW->writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
    case DRW::POLYLINE:
        dxfW->writePolyline(static_cast<DRW_Polyline*>(e));
        break;
    case DRW::SPLINE:
        dxfW->writeSpline(static_cast<DRW_Spline*>(e));
        break;
//    case RS2::EntitySplinePoints:
//        writeSplinePoints(static_cast<DRW_Point*>(e));
//        break;
//    case RS2::EntityVertex:
//        break;
    case DRW::INSERT:
        dxfW->writeInsert(static_cast<DRW_Insert*>(e));
        break;
    case DRW::MTEXT:
        dxfW->writeMText(static_cast<DRW_MText*>(e));
        break;
    case DRW::TEXT:
        dxfW->writeText(static_cast<DRW_Text*>(e));
        break;
    case DRW::DIMLINEAR:
    case DRW::DIMALIGNED:
    case DRW::DIMANGULAR:
    case DRW::DIMANGULAR3P:
    case DRW::DIMRADIAL:
    case DRW::DIMDIAMETRIC:
    case DRW::DIMORDINATE:
        dxfW->writeDimension(static_cast<DRW_Dimension*>(e));
        break;
    case DRW::LEADER:
        dxfW->writeLeader(static_cast<DRW_Leader*>(e));
        break;
    case DRW::HATCH:
        dxfW->writeHatch(static_cast<DRW_Hatch*>(e));
        break;
    case DRW::IMAGE:
        dxfW->writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
        break;
    default:
        break;
    }
}

#ifdef CHECK_MEMORY
/*
class DRW_Point;
class DRW_Line;
class DRW_Ray;
class DRW_Xline;
class DRW_Circle;
class DRW_Arc;
class DRW_Ellipse;
class DRW_Trace;
class DRW_Solid;
class DRW_3Dface;
class DRW_Block;
class DRW_Insert;
class DRW_LWPolyline;
class DRW_Text;
class DRW_MText;
class DRW_Vertex;
class DRW_Polyline;
class DRW_Spline;
class DRW_Hatch;
class DRW_Image;
class DRW_Dimension;
class DRW_DimAligned;
class DRW_DimLinear;
class DRW_DimRadial;
class DRW_DimDiametric;
class DRW_DimAngular;
class DRW_DimAngular3p;
class DRW_DimOrdinate;
class DRW_Leader;
class DRW_Viewport;
*/
void Show_Dwg_Entity_Size()
{
	PRINT_SIZE(DRW_Line)
	PRINT_SIZE(DRW_Point)
	PRINT_SIZE(DRW_Circle)
	PRINT_SIZE(DRW_Polyline)
	PRINT_SIZE(DRW_Text)
	PRINT_SIZE(DRW_Insert)
	PRINT_SIZE(DRW_Block)
	PRINT_SIZE(DRW_Hatch)
	PRINT_SIZE(DRW_HatchLoop)
	PRINT_SIZE(DRW_LWPolyline)
	PRINT_SIZE(DRW_Variant)
	PRINT_SIZE(DRW_Vertex)
	PRINT_SIZE(DRW_Vertex2D)
	PRINT_SIZE(DRW_Viewport)
	PRINT_SIZE(DRW_Coord)
}
#endif 