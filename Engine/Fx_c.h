#pragma once

class FxSystem_c;

enum eFxStaticSystem
{
    FX_STATIC_BLOOD,
    FX_STATIC_BOAT_SPLASH,
    FX_STATIC_BUBBLE,
    FX_STATIC_CAR_DEBRIS,
    FX_STATIC_COLLISION_SMOKE,
    FX_STATIC_GUN_SHELL,
    FX_STATIC_SAND,
    FX_STATIC_SAND2,
    FX_STATIC_SMOKE_HUGE,
    FX_STATIC_SMOKE_EXPAND,
    FX_STATIC_SPARK,
    FX_STATIC_SPARK2,
    FX_STATIC_SPLASH,
    FX_STATIC_WAKE,
    FX_STATIC_WATER_SPLASH,
    FX_STATIC_WHEEL_DIRT,
    FX_STATIC_GLASS
};

class Fx_c
{
public:
    /*
    * AddBlood(CVector&, CVector&, int, float)
	* AddBulletImpact(CVector&, CVector&, int, int, float)
	* AddDebris(CVector&, RwRGBA&, float, int)
	* AddGlass(CVector&, RwRGBA&, float, int)
	* AddPunchImpact(CVector&, CVector&, int)*/
	void AddSparks(CVector& pos, CVector& dir, float force, int count, CVector acrossLine, uint8_t blur, float spread, float life);
	/* AddTyreBurst(CVector&, CVector&)
	* AddWheelDust(CVehicle*, CVector, unsigned char, float)
	* AddWheelGrass(CVehicle*, CVector, unsigned char, float)
	* AddWheelGravel(CVehicle*, CVector, unsigned char, float)
	* AddWheelMud(CVehicle*, CVector, unsigned char, float)
	* AddWheelSand(CVehicle*, CVector, unsigned char, float)
	* AddWheelSpray(CVehicle*, CVector, unsigned char, unsigned char, float)
	* AddWood(CVector&, CVector&, int, float)*/
	void CreateEntityFx(CEntity* entity, char* name, RwV3d* point, RwMatrixTag* mat);
	/* CreateMatFromVec(RwMatrixTag*, CVector*, CVector*)*/
	void DestroyEntityFx(CEntity* entity);
	/* Exit()
	* ExitEntitySystems()
	* ExitStaticSystems()
	* Fx_c()*/
	int GetFxQuality();
	void Init();
	void InitEntitySystems();
	void InitStaticSystems();
	void Render(RwCamera*, uint8_t);
	/* Reset()
	* SetFxQuality(FxQuality_e)
	* TriggerBulletSplash(CVector&)
	* TriggerFootSplash(CVector&) */
	void TriggerGunshot(CEntity*, CVector&, CVector&, bool);
	void TriggerTankFire(CVector& start, CVector& end);
	void TriggerWaterHydrant(CVector& pos);
	void TriggerWaterSplash(CVector& pos);
	void Update(RwCamera*, float);
	//* ~Fx_c()

    FxSystem_c* GetStaticSystem(eFxStaticSystem staticSystem);
private:
    FxSystem_c* m_staticSystems[17];
    /*
    00000000 CFx             struc ; (sizeof=0x80)
00000000 pParticleArray  dd 17 dup(?)            ; offset
00000044 field_44        CParticleData__LListHead ?
00000050 field_50        dd ?
00000054 fDrawDistance   dd ?
00000058 verticesCount2  dd ?
0000005C verticesCount   dd ?
00000060 transformRenderFlags dd ?
00000064 renderRaster    dd ?
00000068 transformLTM    dd ?                    ; offset
0000006C pVerts          dd ?                    ; offset
00000070 field_70        dd ?
00000074 field_74        dd ?
00000078 field_78        dd ?
0000007C field_7C        dd ?
00000080 CFx             ends
*/
};