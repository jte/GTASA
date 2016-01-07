#pragma once

#include "CAESound.h"

class CAEAudioEntity
{
public:
private:
    class CEntity* m_entity;
    CAESound m_sound;

    static void* m_pAudioEventVolumes;
};