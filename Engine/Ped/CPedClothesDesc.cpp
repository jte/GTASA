#include "CPedClothesDesc.h"

bool CPedClothesDesc::GetIsWeaingBalaclava()
{
    return m_modelHash[9] == CKeygen::GetUppercaseKey("balaclava");
}

void CPedClothesDesc::Init()
{
    memset(m_modelHash, 0, sizeof(m_modelHash));
    memset(m_textureHash, 0, sizeof(m_textureHash));
    m_FatStats = 0.0;
    m_MuscleStats = 0.0;
}

void CPedClothesDesc::SetModel(int key, int id)
{
    m_modelHash[id] = key;
}

void CPedClothesDesc::SetModel(const char *model, int id)
{
    if(model)
    {
        m_modelHash[id] = CKeygen::GetUppercaseKey(model);
    }
    else
    {
        m_modelHash[id] = 0;
    }
}

bool CPedClothesDesc::HasVisibleNewHairCut(int flag)
{
    return m_modelHash[9] != CKeygen::GetUppercaseKey("balaclava") && 
           m_modelHash[1] != CKeygen::GetUppercaseKey("head") && !m_modelHash[8] &&
           (flag != 1 || m_modelHash[1] == CKeygen::GetUppercaseKey("afro"));
}

CPedClothesDesc::CPedClothesDesc()
{
    Init();
}











