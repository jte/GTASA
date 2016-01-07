#include "StdInc.h"

void* FxSystemBP_c::operator new(unsigned int size)
{
    return g_fxMan.GetMemoryPool().GetMem(size, 4);
}

FxSystemBP_c::FxSystemBP_c()
{
    m_boundingSphere = NULL;
}

FxSphere_c* FxSystemBP_c::GetBoundingSphere()
{
    return m_boundingSphere;
}

void FxSystemBP_c::SetBoundingSphere(RwV3d* center, float radius)
{
    if (!m_boundingSphere && radius > 0.0)
    {
        m_boundingSphere = g_fxMan.GetMemoryPool().GetMem(sizeof(*m_boundingSphere), 4);
        m_boundingSphere->startPlaneId = 0;
        m_boundingSphere->center = *center;
        m_boundingSphere->radius = radius;
    }
}

void FxSystemBP_c::Load(char*, int file, int a4)
{
    char line[256];
    ReadLine(file, line, sizeof(line));
    if (a4 > 100)
    {
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %s", &v25, &v33);          // FILENAME: <rockstar network disk location of effect>
    }
    //
    ReadLine(file, line, sizeof(line));
    char fxSystemName[32];
    sscanf(line, "%s %s", &v25, fxSystemName);            // NAME: <particle name>
    m_fxSystemNameHash = CKeyGen::GetUppercaseKey(fxSystemName);
    //
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %f", &v26, &m_length);// LENGTH: <float>
    if (a4 < 106)
    {
        m_intervalMin = 0.0f;
        m_length2 = 0.0f;
    }
    else
    {
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %f", &v26, &m_intervalMin);
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s %f", &v26, &m_length2);
    }
    //
    ReadLine(file, line, sizeof(line));
    uint32_t playMode = 0;
    sscanf(line, "%s %d", &v26, &playMode);
    m_playMode = playMode;
  //
    ReadLine(file, line, sizeof(line));
    float cullDist;
    sscanf(line, "%s %f", &v26, &cullDist);
    m_cullDist = FloatToFixedPoint<256>(cullDist);
    if (a4 > 103)
    {
        ReadLine(file, line, sizeof(line));
        RwV3d center;
        float radius;
        sscanf(line, "%s %f %f %f %f", &v26, &center.x, &center.y, &center.z, &radius);
        m_boundingSphere = NULL;
        SetBoundingSphere(&center, radius);
    }
    //
    ReadLine(file, line, sizeof(line));
    sscanf(line, "%s %d", &v26, &m_numFxEmitterBP);
    m_fxEmitterBPArray = g_fxMan.GetMemoryPool().GetMem(m_numFxEmitterBP * 4, 4);
    //
    for (size_t i = 0; i < m_numFxEmitterBP; i++)
    {
        ReadLine(file, line, sizeof(line));
        sscanf(line, "%s", &v26);
        if (!strncmp(v26, "FX_PRIM_EMITTER_DATA:", 22))
        {
            m_fxEmitterBPArray[i] = new FxEmitterBP_c;
            m_fxEmitterBPArray[i]->particleManager.bHasFlatParticleEmitter = 0;
            m_fxEmitterBPArray[i]->particleManager.bHasHeatHazeParticleEmitter = 0;
            ReadLine(file, line, sizeof(line));
            m_fxEmitterBPArray[i]->Load(file, v41, &v25);
            m_fxEmitterBPArray[i]->particleManager.bHasFlatParticleEmitter = CFxEmitterBP::IsFxInfoPresent(
                                                                                pParticleHeader->pFxEmitterBPArray[v11],
                                                                                0x4020u);
            m_fxEmitterBPArray[i]->particleManager.bHasHeatHazeParticleEmitter = CFxEmitterBP::IsFxInfoPresent(
                                                                                    pParticleHeader->pFxEmitterBPArray[v11],
                                                                                    0x4008u);
        }
    }
    if ( a4 >= 108 )
    {
    ReadLine((FILE *)fileHandle, (char *)&v30, 256);
    sscanf((const char *)&v30, "%s %d", &v26, &v25);
    }
    if ( a4 >= 109 )
    {
    ReadLine((FILE *)fileHandle, (char *)&v30, 256);
    sscanf((const char *)&v30, "%s %s", &v26, &v34);
    }
  v19 = 0;
  if ( pParticleHeader->fxEmitterBPCount > 0 )
  {
    v20 = &v35;
    do
    {                                           // CParticleEmitterBaseData::ReadTexture
      (*(void (__stdcall **)(int *, int))(pParticleHeader->pFxEmitterBPArray[v19++]->vtbl + 8))(v20, a4);
      v20 += 32;
    }
    while ( v19 < pParticleHeader->fxEmitterBPCount );
  }
}

ListItem_c* FxSystemBP_c::GetInList()
{
    return &m_inList;
}

void FxSystemBP_c::Unload()
{
    // GTASA: not implemented
}

uint32_t FxSystemBP_c::GetNumFxEmitterBP()
{
	return m_numFxEmitterBP;
}