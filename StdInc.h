#pragma once

// TODO: plugin ctors/copyctors/dtors & streams should use "offset" parameters for accessing "object"

#define ELEMS_COUNT(x) (sizeof(x) / sizeof(x[0]))

// C/C++ Std Lib Includes
#include <cstdint>
#include <ctype.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
// Platform Includes
// Windows NT
#include <Windows.h>
#include <d3d9.h>

// RenderWare Includes
#include "RwSDK\rwsdk\include\d3d9\rwplcore.h"
#include "RwSDK\rwsdk\include\d3d9\rpanisot.h"
#include "RwSDK\rwsdk\include\d3d9\rphanim.h"
#include "RwSDK\rwsdk\include\d3d9\rpskin.h"
#include "RwSDK\rwsdk\include\d3d9\rpmatfx.h"
#include "RwSDK\rwsdk\include\d3d9\rpuvanim.h"
#include "RwSDK\shared\democom\camera.h"
#include "rwcore.h"
#include "rpworld.h"
#include "skeleton.h"
#include "events.h"

// Engine Includes
#define rwVENDORID_ROCKSTAR (0x253F2)

typedef int32_t AssocGroupId;
typedef int32_t AnimationId;

struct Scene_t
{
    RpWorld *World;
    RwCamera *Camera;
} Scene;

#include "Engine\Maths\CMatrix.h"
#include "Engine\Maths\CVector.h"
#include "Engine\Maths\CQuaternion.h"
#include "Engine\CPool.h"
#include "Engine\CPools.h"
#include "Engine\CPlaceable.h"
#include "Engine\Maths\CMatrixLink.h"
#include "Engine\Maths\CMatrixLinkList.h"
#include "Engine\CTxdStore.h"
#include "Engine\CKeyGen.h"
#include "Engine\CRect.h"
//#include "Engine\Entity\CEntity.h"
#include "Engine\Utils.h"
#include "Engine\CBaseModelInfo.h"
#include "Engine\CClumpModelInfo.h"
#include "Engine\Game\CTimer.h"

/*

#include "Engine\Animations\CAnimBlendAssociation.h"
#include "Engine\Animations\CAnimBlendHierarchy.h"
#include "Engine\Animations\CAnimBlendNode.h"
#include "Engine\Animations\CAnimBlendSequence.h"
#include "Engine\Animations\CAnimManager.h"
#include "Engine\Animations\CAnimBlendStaticAssociation.h"
#include "Engine\Animations\CAnimBlendClumpData.h"
#include "Engine\Animations\CAnimBlock.h"
*/
#include "Engine\CLinkList.h"
#include "Engine\CMemoryMgr.h"

//#include "Engine\Physics\CPhysical.h"
#include "Engine\CFileLoader.h"
#include "Engine\CFileMgr.h"

// Compressed vector/matrix, FixedPoint
#include "Engine\CompressedVector.h"
#include "Engine\FixedPoint.h"
#include "Engine\CompressedQuaternion.h"

// Collision Engine
/*
#include "Engine\CColModel.h"
#include "Engine\CCollision.h"
#include "Engine\CCollisionData.h"
#include "Engine\CColBox.h"
#include "Engine\CColDisk.h"
#include "Engine\CColLine.h"
#include "Engine\CColPoint.h"
#include "Engine\CColSphere.h"
#include "Engine\CColTriangle.h"
#include "Engine\CColTrianglePlane.h"
*/
//

#include "CGame.h"
#include "Engine\Entity\CEntity.h"
#include "Engine\CModelInfo.h"
#include "Engine\CVehicleModelInfo.h"
#include "Engine\Camera\CCam.h"

// <Plugins
#include "Engine\PipelinePlugin.h"
#include "Engine\CVisibilityPlugins.h"
#include "Engine\CCarFXRenderer.h"
#include "Engine\CCustomCarEnvMapPipeline.h"
//#include "Engine\RwPlugins\RpAnimBlendClump.h"
#include "Engine\CCustomBuildingRenderer.h"
#include "Engine\CCustomBuildingDNPipeline.h"
//

extern CMatrixLinkList g_MatrixLinkList;
extern RpLight* pDirect;
extern RpLight* pAmbient;

#define DEFAULT_SCREEN_WIDTH (640)
#define DEFAULT_SCREEN_HEIGHT (480)

#ifdef WIDE_SCREEN
#define DEFAULT_ASPECTRATIO (16.0f/9.0f)
#else
#define DEFAULT_ASPECTRATIO (4.0f/3.0f)
#endif

#define DEFAULT_VIEWWINDOW (0.5f)

#define GRID_UNIT 0.02
#define GRID_OFFSET 60.0
#define MAX_SCAN_CODE 0xFFFF
#define GRID_MAX_UNITS 120
#define GRID_MAX_CELLS 60
#define GRID_MAX_COORD 3000.0
#define GRID_CELL_SIZE 16

#define RW_TYPE_ATOMIC 1
#define RW_TYPE_CLUMP 2
 
struct CColLighting
{
    uint8_t   day:4;    // 0-15
    uint8_t   night:4;  // 0-15
};