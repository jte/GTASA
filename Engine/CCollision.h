#pragma once

bool ProcessDiscCollision(CColPoint &p1, const CMatrix& mat, const CColDisk& disk, CColPoint& p2, bool& b1, float& f1, CColPoint& p3);

class CCollision
{
public:
	//* BuildCacheOfCameraCollision(CColSphere*, CColSphere*)
	//* CalculateTrianglePlanes(CColModel*)
	//* CalculateTrianglePlanes(CCollisionData*)
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
	//* DistToLine(CVector const*, CVector const*, CVector const*)
	//* DistToLineSqr(CVector const*, CVector const*, CVector const*)
	//* DistToMathematicalLine(CVector const*, CVector const*, CVector const*)
	//* DistToMathematicalLine2D(float, float, float, float, float, float)
	//* GetBoundingBoxFromTwoSpheres(CColBox*, CColSphere*, CColSphere*)
	//* GetPrincipleAxis(CVector*)
	//* Init()
	//* IsStoredPolyStillValidVerticalLine(CVector const&, float, CColPoint&, CStoredCollPoly*)
	//* IsThisVehicleSittingOnMe(CVehicle*, CVehicle*)
	//* PointInPoly(CVector*, CColTriangle*, CVector*, CVector*)
	//* PointInTriangle(CVector const&, CVector const*)
	bool ProcessColModels(CMatrix const &mat1, CColModel &colModel1, CMatrix const &mat2, CColModel &colModel2, CColPoint *pColPoint1, CColPoint *pColPoint2, float *f1, bool b1);
	bool ProcessLineBox(const CColLine& colLine, const CColBox& colBox, CColPoint& colPoint, float& limit);
	//* ProcessLineOfSight(CColLine const&, CMatrix const&, CColModel&, CColPoint&, float&, bool, bool)
	bool ProcessLineSphere(const CColLine& colLine, const CColSphere& colSphere, CColPoint& colPoint, float& limit); // DONE
	bool ProcessLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*);
	bool ProcessSphereBox(CColSphere const &first, CColBox const &second, CColPoint &point, float &fMaxTouchDistance);
	bool ProcessSphereSphere(CColSphere const &first, CColSphere const &second, CColPoint &point, float &depth);
	//* ProcessSphereTriangle(CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&)
	bool ProcessVerticalLine(CColLine const &colLine, CMatrix const &mat, CColModel &colModel, CColPoint &colPoint, float&, bool, bool, CStoredCollPoly*);
	//* ProcessVerticalLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*)
	//* RayPolyPOP(CVector*, CVector*, CColTriangle*, CVector*, CVector*)
	//* RemoveTrianglePlanes(CColModel*)
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
	bool TestSphereBox(const CSphere& sphere, const CBox& box); // DONE
	bool TestSphereSphere(const CColSphere& first, const CColSphere& second); // DONE
	//* TestSphereTriangle(CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&)
	//* TestVerticalLineBox(CColLine const&, CBox const&)
	void Update(); // DONE
private:
	static bool bCamCollideWithBuildings;
	static bool bCamCollideWithObjects;
	static bool bCamCollideWithPeds;
	static bool bCamCollideWithVehicles;
	* ms_colModelCache
	* ms_collisionInMemory
	static size_t ms_iProcessLineNumCrossings;
	static float relVelCamCollisionVehiclesSqr;
};