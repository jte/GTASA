#include "StdInc.h"

void CRope::Render()
{
    if(!(TheCamera.AreVerticesVisible(m_ropeSegments[16], 20.0f, &TheCamera.matInverse.matrix) || 
       TheCamera.useMirrorMatrix && TheCamera.AreVerticesVisible(m_ropeSegments[16], 20.0f, &TheCamera.matMirrorInverse.matrix)))
    {
        return;
    }
    CVector dpos = TheCamera.GetPos() - m_ropeSegments[0];
    if(dpos.Magnitude2DSquared() >= 120.0f)
    {
        return;
    }
    DefinedState();
    RwIm3DVertex TempVertexBuffer[32];
    for(size_t i = 0; i < ELEMS_COUNT(TempVertexBuffer); i++)
    {
        TempVertexBuffer[i].color = 0x80000000;
        TempVertexBuffer[i].objVertex = m_ropeSegments[i];
    }
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    if(RwIm3DTransform(TempVertexBuffer, ELEMS_COUNT(TempVertexBuffer), NULL, 0))
    {
        RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, RopeIndes, 62);
        RwIm3DEnd();
    }
    if(m_ropeType == 6)
    {
        TempVertexBuffer[0].color = 0x80000000u;
        TempVertexBuffer[0].objVertex = m_ropeSegments[0];
        TempVertexBuffer[1].color = 0x80000000u;
        TempVertexBuffer[1].objVertex.x = 709.32f; 
        TempVertexBuffer[1].objVertex.y = 916.2f;
        TempVertexBuffer[1].objVertex.z = 53.0f; 
        if(RwIm3DTransform(TempVertexBuffer, 2, 0, 0))
        {
            RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, RopeIndices, 2);
            RwIm3DEnd();
        }
    } 
}