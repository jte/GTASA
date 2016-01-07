#pragma once

class CColBox;
class CColSphere;
struct CompressedVector;
class CColDisk;
class CColTriangle;
class CColTrianglePlane;

class CCollisionData
{
public:
	CCollisionData();
	void CalculateTrianglePlanes();
	//* Copy(CCollisionData const&)
	//* GetLinkPtr()
	//* GetShadTrianglePoint(CVector&, int) const
	void GetTrianglePoint(CVector& trianglePoint, int index) const;
	void RemoveCollisionVolumes();
	void RemoveTrianglePlanes();
	//* SetLinkPtr(CLink<CCollisionData*>*)
    CColSphere& GetSphere(size_t index) const;
    uint16_t GetNumSpheres() const;
    uint16_t GetNumBoxes() const;
    uint16_t GetNumTriangles() const;
    uint8_t GetNumDisks() const;
	void SetSphere(size_t index, const CColSphere& colSphere);
protected:
private:
    uint16_t m_numSpheres;
    uint16_t m_numBoxes;
    uint16_t m_numTriangles;
    uint8_t m_numDisks;
    // Flags:
    //      0x01 - has disks allocated
    uint8_t m_flags;
    CColSphere* m_spheres;
    CColBox* m_boxes;
    CColDisk* m_disks;
    CompressedVector* m_trianglePoints;
    CColTriangle* m_triangles;
    CColTrianglePlane* m_trianglePlanes;
    uint32_t m_numShadowTriangles;
    uint32_t m_numShadowVertices;
    CompressedVector* m_shadowTrianglePoints;
    void* m_shadowTriangles;
};