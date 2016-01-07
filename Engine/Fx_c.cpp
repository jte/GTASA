#include "StdInc.h"

void Fx_c::Update(RwCamera* camera, float f1)
{
    g_fxMan.Update(camera, f1);
}

void Fx_c::Render(RwCamera* camera, uint8_t uc1)
{
    g_fxMan.Render(camera, uc1);
}

int Fx_c::GetFxQuality()
{
    return m_fxQuality;
}

void Fx_c::InitStaticSystems()
{
    CVector origin = CVector(0.0f, 0.0f, 0.0f);
    m_staticSystems[0] = FxManager_c::CreateFxSystem("prt_blood", origin, 0, 1);
    m_staticSystems[1] = FxManager_c::CreateFxSystem("prt_boatsplash", origin, 0, 1);
    m_staticSystems[2] = FxManager_c::CreateFxSystem("prt_bubble", origin, 0, 1);
    m_staticSystems[3] = FxManager_c::CreateFxSystem("prt_cardebris", origin, 0, 1);
    m_staticSystems[4] = FxManager_c::CreateFxSystem("prt_collisionsmoke", origin, 0, 1);
    m_staticSystems[5] = FxManager_c::CreateFxSystem("prt_gunshell", origin, 0, 1);
    m_staticSystems[6] = FxManager_c::CreateFxSystem("prt_sand", origin, 0, 1);
    m_staticSystems[7] = FxManager_c::CreateFxSystem("prt_sand2", origin, 0, 1);
    m_staticSystems[8] = FxManager_c::CreateFxSystem("prt_smoke_huge", origin, 0, 1);
    m_staticSystems[9] = FxManager_c::CreateFxSystem("prt_smokeII_3_expand", origin, 0, 1);
    m_staticSystems[10] = FxManager_c::CreateFxSystem("prt_spark", origin, 0, 1);
    m_staticSystems[11] = FxManager_c::CreateFxSystem("prt_spark_2", origin, 0, 1);
    m_staticSystems[12] = FxManager_c::CreateFxSystem("prt_splash", origin, 0, 1);
    m_staticSystems[13] = FxManager_c::CreateFxSystem("prt_wake", origin, 0, 1);
    m_staticSystems[14] = FxManager_c::CreateFxSystem("prt_watersplash", origin, 0, 1);
    m_staticSystems[15] = FxManager_c::CreateFxSystem("prt_wheeldirt", origin, 0, 1);
    m_staticSystems[16] = FxManager_c::CreateFxSystem("prt_glass", origin, 0, 1);
}



FxSystem_c* Fx_c::GetStaticSystem(eFxStaticSystem staticSystem)
{
    return m_staticSystems[staticSystem];
}

void Fx_c::Init()
{
    g_fxMan.Init();
    g_fxMan.LoadFxProject("models\\effects.fxp");
    g_fxMan.SetWindData(CWeather::WindDir.matrix.right, &WindFloat);
    InitStaticSystems();
    pThis->field_50 = 0;
}

void Fx_c::CreateEntityFx(CEntity* entity, char* name, RwV3d* point, RwMatrixTag* mat)
{
    FxSystem_c* fxSystem = FxManager_c::CreateFxSystem(name, point, mat, 1);
    if (fxSystem)
    {
        ListItem_c* entityItem = new ListItem_c;
        entityItem->fxSystem = fxSystem;
        entityItem->entity = entity;
        m_entityFxList.AddItem(entityItem);
        fxSystem->Play();
    }
}

void Fx_c::DestroyEntityFx(CEntity* entity)
{
    if (m_entityFxList.head)
    {
        ListItem_c* node = m_entityFxList.head;
        do
        {
            if (node->entity == entity)
            {
                m_entityFxList.RemoveItem(node);
                node->fxSystem->Kill();
                delete node;
            }
            node = node->next;
        } while (node);
    }
}

void Fx_c::AddSparks(CVector& pos, CVector& dir, float force, int count, CVector acrossLine, uint8_t blur, float spread, float life)
{
    float distSq = (TheCamera.GetPos() - pos).MagnitudeSquared();
    if (distSq <= 150.0f * 150.0f && (distSq <= 15.0f * 15.0f || !(CTimer::GetFrameCounter() & 1)))
    {
        FxPrtMult_c prtInfo(1.0, 1.0, 1.0, 1.0, 1.0, 0.0, fLife * 0.8);
        for (size_t i = 0; i < count; i++)
        {
            float prtForce = 1.0f - i / count;
            FxSystem_c* fxSystem = (blur) ? g_fx.pStaticSystems[10] : g_fx.pStaticSystems[11];
            v35 = pos - acrossLine * CTimer::GetTimeStep() * prtForce;
            a3 = (dir + (rand() / 32767.0f * 2 * spread - spread)) * force;
            fxSystem.AddParticle(v35, a3, prtForce * 0.05, prtInfo, -1.0f, 1.2, 0.6, 0);
        }
    }
}

void Fx_c::TriggerTankFire(CVector& start, CVector& end)
{
    if ((TheCamera.GetPos() - start).MagnitudeSquared() < 25.0f * 25.0f)
    {
        RwMatrixTag* mat = g_fxMan.FxRwMatrixCreate();
        CFx::CreateMatFromVec(mat, start, end);
        CVector origin(0.0f, 0.0f, 0.0f);
        FxSystem_c* fxSystem = FxManager_c::CreateFxSystem("tank_fire", origin, mat, 0);
        if (fxSystem)
        {
            fxSystem->InitMatrix();
            fxSystem->Start();
        }
        g_fxMan.FxRwMatrixDestroy(mat);
    }
}

void CFx::TriggerWaterSplash(CVector& pos)
{
    if ((TheCamera.GetPos() - pos).MagnitudeSquared() < 25.0f * 25.0f)
    {
        FxSystem_c* fxSystem = FxManager_c::CreateFxSystem("water_splash_big", pos, 0, 0);
        if (fxSystem)
        {
            fxSystem->Start();
        }
    }
}

void CFx::TriggerWaterHydrant(CVector& pos)
{
    if ((TheCamera.GetPos() - pos).MagnitudeSquared() < 25.0f * 25.0f)
    {
        FxSystem_c* fxSystem = FxManager_c::CreateFxSystem("water_hydrant", pos, 0, 0);
        if (fxSystem)
        {
            fxSystem->Start();
        }
    }
}

void Fx_c::TriggerGunshot(CEntity* entity, CVector& pos, CVector& dir, bool createGunFlash)
{
    if ((TheCamera.GetPos() - pos).MagnitudeSquared() >= 25.0f * 25.0f)
    {
        return;
    }
    
    if (!entity)
    {
        v13 = g_fxMan.FxRwMatrixCreate();
        CFx::CreateMatFromVec(v13, pos, dir);
        point = CVector(0,0,0);
    }
    else
    {
        point = Multiply3x3(pos - entity->GetPos(), entity->GetMatrix());
        if (!entity->GetRwObject())
        {
            return;
        }
        v13 = &entity->GetRwObject()->parent->modelling;
    }
      
    if (!v13)
    {
        return;
    }
    if (createGunFlash)
    {
        FxSystem_c* fxSystem = FxManager_c::CreateFxSystem("gunflash", point, v13, 0);
        if (fxSystem)
        {
            if (!entity)
            {
                fxSystem.InitMatrix();
            }
            fxSystem.Start();
        }
    }
    FxSystem_c* fxSystem = FxManager_c::CreateFxSystem("gunsmoke", point, v13, 0);
    if (fxSystem)
    {
        if (!entity)
        {
            fxSystem.InitMatrix();
        }
        fxSystem.Start();
    }
    if (!entity)
    {
        g_fxMan.FxRwMatrixDestroy(v13);
    }
}