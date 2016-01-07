#pragma once

#include "CColPoint.h"
#include "CColLine.h"

bool ProcessDiscCollision(CColPoint &p1, const CMatrix& mat, const CColDisk& disk, CColPoint& p2, bool& b1, float& f1, CColPoint& p3);

class CStoredCollPoly
{
public:
private:
}

class CCollision
{
public:
	//* BuildCacheOfCameraCollision(CColSphere*, CColSphere*)
	static void CalculateTrianglePlanes(CColModel* colModel);
	static void CalculateTrianglePlanes(CCollisionData* colData);
	//* CameraConeCastVsWorldCollision(CColSphere*, CColSphere*, float*, float)
	//* CheckCameraCollisionBuildings(int, int, CColBox*, CColSphere*, CColSphere*, CColSphere*)
	//* CheckCameraCollisionObjects(int, int, CColBox*, CColSphere*, CColSphere*, CColSphere*)
	//* CheckCameraCollisionPeds(int, int, CVector*, CVector*, float*)
	//* CheckCameraCollisionVehicles(int, int, CColBox*, CColSphere*, CColSphere*, CColSphere*, CVector*)
	//* CheckPeds(CVector*, CVector*, float*)
	//* Closest3(CVector*, CVector*)
	//* ClosestPointOnLine(CVector*, CVector*, CVector*, CVector*)
	//* ClosestPointOnPoly(CColTriangle*, CVector*, CVector*)
	//* ClosestPointsOnPoly(CColTriangle*, CVector*, CVector*, CVector*)
	//* DistAlongLine2D(float, float, float, float, float, float)
	static float DistToLine(const CVector*, const CVector*, const CVector*);
	//* DistToLineSqr(CVector const*, CVector const*, CVector const*)
	//* DistToMathematicalLine(CVector const*, CVector const*, CVector const*)
	//* DistToMathematicalLine2D(float, float, float, float, float, float)
	static void GetBoundingBoxFromTwoSpheres(CColBox* box, CColSphere* s1, CColSphere* s2);
	//* GetPrincipleAxis(CVector*)
	//* Init()
	//* IsStoredPolyStillValidVerticalLine(CVector const&, float, CColPoint&, CStoredCollPoly*)
	//* IsThisVehicleSittingOnMe(CVehicle*, CVehicle*)
	//* PointInPoly(CVector*, CColTriangle*, CVector*, CVector*)
	//* PointInTriangle(CVector const&, CVector const*)
	static bool ProcessColModels(CMatrix const &mat1, CColModel &colModel1, CMatrix const &mat2, CColModel &colModel2, CColPoint *pColPoint1, CColPoint *pColPoint2, float *f1, bool b1);
	static bool ProcessLineBox(const CColLine& colLine, const CColBox& colBox, CColPoint& colPoint, float& limit);
	//* ProcessLineOfSight(CColLine const&, CMatrix const&, CColModel&, CColPoint&, float&, bool, bool)
	static bool ProcessLineSphere(const CColLine& colLine, const CColSphere& colSphere, CColPoint& colPoint, float& depth); // DONE
	static bool ProcessLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*);
	static bool ProcessSphereBox(CColSphere const &first, CColBox const &second, CColPoint &point, float &fMaxTouchDistance);
	static bool ProcessSphereSphere(const CColSphere& first, const CColSphere& second, CColPoint& point, float& depth);
	//* ProcessSphereTriangle(CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&)
	static bool ProcessVerticalLine(const CColLine& colLine, const CMatrix& mat, CColModel& colModel, CColPoint& colPoint, float& argFloat, bool, bool, CStoredCollPoly *pCollPoly);
	//* ProcessVerticalLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*)
	//* RayPolyPOP(CVector*, CVector*, CColTriangle*, CVector*, CVector*)
	static void RemoveTrianglePlanes(CColModel* colModel);
	//* RemoveTrianglePlanes(CCollisionData*)
//* Shutdown()
	void SortOutCollisionAfterLoad(); // DONE
	//* SphereCastVersusVsPoly(CColSphere*, CColSphere*, CColTriangle*, CColTrianglePlane*, CompressedVector*)
	//* SphereCastVsBBox(CColSphere*, CColSphere*, CColBox*)
	//* SphereCastVsCaches(CColSphere*, CVector*, int, CColCacheEntry*, int*, CColCacheEntry*)
	//* SphereCastVsEntity(CColSphere*, CColSphere*, CEntity*)
	//* SphereCastVsSphere(CColSphere*, CColSphere*, CColSphere*)
	//* SphereVsEntity(CColSphere*, CEntity*)
	//* Test2DLineAgainst2DLine(float, float, float, float, float, float, float, float)
	//* TestLineBox(CColLine const&, CBox const&)
	//* TestLineBox_DW(CColLine const&, CBox const&)
	//* TestLineOfSight(CColLine const&, CMatrix const&, CColModel&, bool, bool)
	//* TestLineSphere(CColLine const&, CColSphere const&)
	//* TestLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&)
	static bool TestSphereBox(const CSphere& sphere, const CBox& box);
	static bool TestSphereSphere(const CColSphere& first, const CColSphere& second);
	//* TestSphereTriangle(CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&)
	//* TestVerticalLineBox(CColLine const&, CBox const&)
	void Update(); // DONE
private:
	static bool bCamCollideWithBuildings;
	static bool bCamCollideWithObjects;
	static bool bCamCollideWithPeds;
	static bool bCamCollideWithVehicles;
	static CLinkList<CCollisionData*> ms_colModelCache;
	//* ms_collisionInMemory
	static size_t ms_iProcessLineNumCrossings;
	static float relVelCamCollisionVehiclesSqr;
};