#include "StdInc.h"

void FxEmitterBP_c::Load(int file, int a3, FxName32_t* texNames)
{
    FxPrimBP_c::Load(file, a3, texNames);
    char line[256];
    ReadLine(file, line, sizeof(line));
    sscanf(&v8, "%s %f", &v7, &v6);
    m_fxInfoManager.fLodStart = (unsigned __int64)(v6 * 64.0);
  ReadLine(a2, &v8, 256);
  sscanf(&v8, "%s %f", &v7, &v6);
  pThis->__parent.fxInfoManager.fLodEnd = (unsigned __int64)(v6 * 64.0);
}

void* FxEmitterBP_c::operator new(size_t size)
{
    return g_fxMan.GetMemoryPool().GetMem(size, 4);
}

void FxEmitterBP_c::LoadTextures(FxName32_t* name, int a3)
{
    char maskName[64];

    sprintf(maskName, "%sm", name->texName[0]);
    m_textures[0] = RwTextureRead(name->texName[0], maskName);
    if (!m_textures[0])
    {
        m_textures[0] = RwTextureRead(name->texName[0], NULL);
    }
    if (a3 <= 101)
    {
        return;
    }
    for (size_t i = 1; i < ELEMS_COUNT(name->texName); i++)
    {
        if (strcmp(name->texName[i], "NULL") != 0)
        {
            sprintf(maskName, "%sm", name->texName[i]);
            m_textures[i] = RwTextureRead(name->texName[i], maskName);
            if (!m_textures[i])
            {
                m_textures[i] = RwTextureRead(name->texName[i], NULL);
            }
        }
    }
}

FxPrim_c* FxEmitterBP_c::CreateInstance()
{
    return new FxEmitter_c;
}