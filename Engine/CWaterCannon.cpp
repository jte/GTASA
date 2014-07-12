#include "CWaterCannon.h"
#define MAX_SEGMENTS (32)
void CWaterCannon::PushPeds()
{
    for(size_t i = 0; i < ARRAY_COUNT(m_vecSegments); i++)
    {
        if(m_bActive[i])
        {
            Clamp(-10000.0, m_vecSegments[i].x, 10000.0);
            Clamp(-10000.0, m_vecSegments[i].y, 10000.0);
            Clamp(-10000.0, m_vecSegments[i].z, 10000.0);
        }
    }
    for(size_t i = 0; i < PedPool->size; i++)
    {
        if(!PedPool->flags[i] || !PedPool->objects[i])
        {
            continue;
        }
        CPed *ped = PedPool->objects[i];
        if(!ped->bUsesCollision)
        {
           continue;
        }
        if(!IsWithinLimit(-10000.0, GetPos()->x, 10000.0))
        {
            continue;
        }
        if(!IsWithinLimit(-10000.0, GetPos()->y, 10000.0))
        {
            continue;
        }
        if(!IsWithinLimit(-10000.0, GetPos()->z, 10000.0))
        {
            continue;
        }
        for(size_t j = 0; j < MAX_SEGMENTS; j++)
        {
            if(!m_bActive[j])
            {   
                continue;
            }
            CVector dx = m_vecSegments[j] - ped->GetPos();
            if(dx.LengthSquared() >= 5.0)
            {
                continue;
            }
            CEventPedShotByWaterCannon event(m_vecSegments[j], m_vecReleasedSegments[j]);
            ped->pedEvents->Add(&event, false);
            ped->bIsStanding = false;
            ped->bWasStanding = false;
            ped->ApplyMoveForce(0.0, 0.0, dt);
            float x_force = (0.1000000014901161 * m_vecReleasedSegments[j].x - ped->m_vecLinearVelocity.x) * 0.1000000014901161;
            if(x_force * m_vecReleasedSegments[j].x > 0.0)
            {   
                ped->m_vecLinearVelocity.x += x_force;
            }
            float y_force = (0.1000000014901161 * m_vecReleasedSegments[j].y - ped->m_vecLinearVelocity.y) * 0.1000000014901161;
            if(y_force * m_vecReleasedSegments[j].y > 0.0)
            {   
                ped->m_vecLinearVelocity.y += y_force;
            }
            CVector pos = ped->GetPos();
            // force action
            CVector dir_action = ped->m_vecLinearVelocity * -0.300000011920929;
            // an offset to the center of a ped's height
            dir.z += 0.5;
            // force reaction
            CVector dir_reaction = ped->m_vecLinearVelocity * 0.300000011920929;
            FxPrtMult_c spray(1.0, 1.0, 1.0, 0.6, 0.75, 0.0, 0.2);
            CLASS_Fx.pParticleArray[9].AddParticle(pos, dir_reaction, 0, spray, -1.0, 1.2, 0.6, 0);
            CLASS_Fx.pParticleArray[9].AddParticle(pos, dir_action, 0, spray, -1.0, 1.2, 0.6, 0);
        }
    }
}