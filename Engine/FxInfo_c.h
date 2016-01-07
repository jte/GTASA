#pragma once

struct EmissionInfo_t
{
    float emissionRate;
    float radius;
    float sizeMinX;
    float sizeMinY;
    float sizeMinZ;
    float sizeMaxX;
    float sizeMaxY;
    float sizeMaxZ;
    float emSpeed;
    float emSpeedBias;
    CVector emissionDirection;
    float emAngleMin;
    float emAngleMax;
    float emLife;
    float emLifeBias;
    CVector emPos;
    float emRotationAngleMin;
    float emRotationAngleMax;
    float windMin;
    float windMax;
    float rainMin;
    float rainMax;
};

class FxInfo_c
{
public:
    virtual ~FxInfo_c();
    virtual void Load(int file) = 0;
    virtual void GetValue(float,float,float,float,uint8_t,void *) = 0;
protected:
    uint16_t m_id;
    bool m_timeModeParticle;
    uint8_t pad1;
};