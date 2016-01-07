#include "StdInc.h"

FxSystem_c* FxManager_c::CreateFxSystem(FxSystemBP_c* fxSystemBP, RwMatrixTag* matrix1, RwMatrixTag* matrix2, unsigned char force)
{
    if (!fxSystemBP || !ShouldCreate(fxSystemBP, matrix1, matrix2, force))
    {
        return NULL;
    }
    FxSystem_c* fxSystem = new FxSystem_c;
    fxSystem->Init(fxSystemBP, matrix1, matrix2);
    m_fxSystemList.AddItem(fxSystem->GetInList());
    switch (g_fx.GetFxQuality())
    {
        case 0:
            fxSystem->SetRateMult(0.5f);
        break;
        case 1:
            fxSystem->SetRateMult(0.75f);
        break;
        default:
            fxSystem->SetRateMult(1.0f);
        break;
    }
    return fxSystem;
}

FxFrustumInfo_c* FxManager_c::GetFrustumInfo()
{
    return &m_frustumInfo;
}

void FxManager_c::CalcFrustumInfo(RwCamera* camera)
{
    float viewDist = sqrt(camera->viewWindow.x * camera->viewWindow.x + camera->viewWindow.y * camera->viewWindow.y);
    float fov = RAD_TO_DEG(atan2(1.0f, viewDist));
    float distance = sqrt(viewDist * viewDist + 1.0) * camera->farPlane / sin(DEG_TO_RAD(180.0 - (fov + fov))) * sin(DEG_TO_RAD(fov));
    RwFrame* camFrame = RwFrameGetParent(camera);
    m_frustumInfo.plane.normal.x = camFrame->modelling.pos.x + camFrame->modelling.at.x * distance;
    m_frustumInfo.plane.normal.y = camFrame->modelling.pos.y + camFrame->modelling.at.y * distance;
    m_frustumInfo.plane.normal.z = camFrame->modelling.pos.z + camFrame->modelling.at.z * distance;
    m_frustumInfo.plane.distance = distance;
    m_frustumInfo.cameraFrustumPlanes[0].normal = camera->frustumPlanes[2].plane.normal;
    m_frustumInfo.cameraFrustumPlanes[0].distance = camera->frustumPlanes[2].plane.distance;
    m_frustumInfo.cameraFrustumPlanes[1].normal = camera->frustumPlanes[3].plane.normal;
    m_frustumInfo.cameraFrustumPlanes[1].distance = camera->frustumPlanes[3].plane.distance;
    m_frustumInfo.cameraFrustumPlanes[2].normal = camera->frustumPlanes[4].plane.normal;
    m_frustumInfo.cameraFrustumPlanes[2].distance = camera->frustumPlanes[4].plane.distance;
    m_frustumInfo.cameraFrustumPlanes[3].normal = camera->frustumPlanes[5].plane.normal;
    m_frustumInfo.cameraFrustumPlanes[3].distance = camera->frustumPlanes[5].plane.distance;
}

FxSystemBP_c* FxManager_c::FindFxSystemBP(char* name)
{
    if (!m_fxSystemBPList.head)
    {
        return 0;
    }
    uint32_t nameHash = CKeyGen::GetUppercaseKey(name);
    FxSystemBP_c* fxSystemBP = m_fxSystemBPList.head; // list item cast
    while (nameHash != fxSystemBP->m_fxSystemNameHash)
    {
        fxSystemBP = fxSystemBP->m_inList.next; // list item cast
        if (!fxSystemBP)
        {
            return 0;
        }
    }
    return fxSystemBP;
}

bool FxManager_c::LoadFxProject(char* fileName)
{
    char txdName[MAX_PATH];
    strcpy(txdName, fileName);
    strcat(txdName, "PC.txd");
    m_txdIndex = CTxdStore::AddTxdSlot("fx");
    CTxdStore::LoadTxd(m_txdIndex, txdName);
    CTxdStore::AddRef(m_txdIndex);
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(m_txdIndex);
    int file = ::OpenFile(fileName, "r");
    if (file <= 0)
    {
        return 0;
    }
    char line[256+128];
    ReadLine(file, &line[128], 256);
    sscanf(&line[128], "%s", line);
    ReadLine(file, &line[128], 256);
    ReadLine(file, &line[128], 256);
    sscanf(&line[128], "%s", line);
    while (!strncmp(line, "FX_SYSTEM_DATA:", 15))
    {
        LoadFxSystemBP(fileName, file);
        ReadLine(file, &line[128], 256);
        ReadLine(file, &line[128], 256);
        sscanf(&line[128], "%s", line);
    }
    CloseFile(file);
    CTxdStore::PopCurrentTxd();
    m_buffer.Optimise();
    return 1;
}

FxMemoryPool_c FxManager_c::GetMemoryPool()
{
    return m_buffer;
}

int __thiscall FxManager_c::Render(CFxManager *this, int a2, int a3)
{
  double v3; // st7@1
  List_c *v4; // esi@1
  float v5; // ST1C_4@2
  ListItem_c *i; // esi@1

  v3 = 1.0 - CCustomBuildingDNPipeline::m_fDNBalanceParam;
  v4 = (List_c *)this;
  this->field_B8 = 0;
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
    
  for ( i = v4->head; i; i = i->next )
  {
    v5 = v3 * 0.60000002 + 0.40000001;
    FxSystemBP_c::Render((CFxSystemBP *)i, (RwCamera *)a2, SLODWORD(v5), a3);
  }
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
}

void FxManager_c::SetWindData(RwV3d* windVector, float* windStrength)
{
    m_windVector = windVector;
    m_windStrength = *windStrength;
}

RwMatrix* FxManager_c::FxRwMatrixCreate()
{
    return m_matrices[m_currentMatrix++];
}

void FxManager_c::FxRwMatrixDestroy(RwMatrixTag*)
{
    // this could be a cause of some bugs
    // proper implementation should look for the matrix ptr and move down other matrices to
    // fill in the slot
    m_currentMatrix--;
}

bool FxManager_c::ShouldCreate(FxSystemBP_c* fxSystemBP, RwMatrix* offsetMatrix, RwMatrix* parentMatrix, bool forceCreate)
{
    if (forceCreate)
    {
        return true;
    }
    RwMatrix* composite = FxRwMatrixCreate();
    if (parentMatrix)
    {
        RwMatrixMultiply(composite, offsetMatrix, parentMatrix);
    }
    else
    {
        *composite = *offsetMatrix;
    }
    FxSphere_c sphere;
    RwV3dTransformPoints(&sphere.center, &fxSystemBP->GetBoundingSphere()->center, 1, composite);
    sphere.radius = fxSystemBP->GetBoundingSphere()->radius;
    FxRwMatrixDestroy(composite);
    return m_frustumInfo.IsCollision(&sphere);
}

void FxManager_c::LoadFxSystemBP(char* fileName, int file)
{
    char line[256];
    ReadLine(file, line, 256);
    sscanf(line, "%d", &a1);
    FxSystemBP_c* fxSystemBP = new FxSystemBP_c;
    fxSystemBP->Load(fileName, file,);
    m_fxSystemBPList.AddItem(fxSystemBP->GetInList());
    /*
  v4 = (CFxSystemBP *)FxSystemBP_c::operator new(0x24u);
  v8 = v4;
  v5 = 0;
  __$EHRec$.state = 0;
  if ( v4 )
    v5 = FxSystemBP_c::FxSystemBP_c(v4);
  __$EHRec$.state = -1;
  FxSystemBP_c::Load(v5, (char *)szFileName, file, a1);
  List_c::AddItem(&pThis->fxSystemBPList, &v5->InList);*/
}