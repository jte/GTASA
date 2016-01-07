#include "StdInc.h"

FxPrimBP_c::FxPrimBP_c()
{
    memset(m_textures, 0, sizeof(m_textures));
}

FxPrimBP_c::~FxPrimBP_c()
{
    for (size_t i = 0; i < ELEMS_COUNT(m_textures); i++)
    {
        RwTextureDestroy(m_textures[i]);
        m_textures[i] = NULL;
    }
}

void FxPrimBP_c::GetRWMatrix(RwMatrixTag* rwMatrix)
{
    CMatrix mat;
    if (m_matrix)
    {
        m_matrix->DecompressIntoFullMatrix(mat);
        *rwMatrix = mat;
    }
    else
    {
        rwMatrix->right = CVector(1.0f, 0.0f, 0.0f);
        rwMatrix->up =    CVector(0.0f, 1.0f, 0.0f);
        rwMatrix->at =    CVector(0.0f, 0.0f, 1.0f);
        rwMatrix->pos =   CVector(0.0f, 0.0f, 0.0f);
    }
}

void FxPrimBP_c::Load(int file, int a3, FxName32_t* texNames)
{
    char line[256];
    char dummy[128];
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s", dummy);
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %s", dummy, dummy);
    ReadLine(file, line, sizeof(line));
    CCompressedMatrixNotAligned<32767> localMat;
    sscanf(line, "%s %f %f %f %f %f %f %f %f %f %f %f %f", dummy, 
                &localMat.right.x, &localMat.right.y, &localMat.right.z, 
                &localMat.up.x, &localMat.up.y, &localMat.up.z, 
                &localMat.at.x, &localMat.at.y, &localMat.at.z, 
                &localMat.pos.x, &localMat.pos.y, &localMat.pos.z);
    if ( localMat.right.x != 1.0 || localMat.right.y != 0.0 || localMat.right.z != 0.0
        || localMat.up.x != 0.0 || localMat.up.y != 1.0 || localMat.up.z != 0.0
        || localMat.at.x != 0.0 || localMat.at.y != 0.0 || localMat.at.z != 1.0
        || localMat.pos.x != 0.0 || localMat.pos.y != 0.0 || localMat.pos.z != 0.0 )
    {
        m_matrix = g_fxMan.GetMemoryPool().GetMem(sizeof(CCompressedMatrixNotAligned<32767>), 2);
        *m_matrix = localMat;
    }
    else
    {
        m_matrix = NULL;
    }
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %s", dummy, texNames->texName1);              // TEXTURE: <name>
    if (a3 > 101)
    {
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %s", dummy, texNames->texName2);  // TEXTURE2: <name>
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %s", dummy, texNames->texName3);  // TEXTURE3: <name>
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %s", dummy, texNames->texName4);  // TEXTURE4: <name>
    }
    ReadLine(file, line, sizeof(line));
    uint32_t alphaOn;
    sscanf(line, "%s %d", dummy, &m_alphaOn);
    m_alphaOn = alphaOn;
    //
    uint32_t srcBlendId;
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %d", dummy, &srcBlendId);
    m_srcBlendId = srcBlendId;
    //
    uint32_t destBlendId;
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %d", dummy, &destBlendId);
    m_destBlendId = destBlendId;
    m_fxInfoManager.Load(file, a3);
}