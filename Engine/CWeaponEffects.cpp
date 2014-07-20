#include "StdInc.h"

CPlayerCrossHair gCrossHair[2];
RwTexture* gpCrossHairTexFlight[2];
RwTexture* gpCrossHairTex;

void CWeaponEffects::Init()
{
    for(size_t i = 0; i < ELEMS_COUNT(gCrossHair); i++)
    {
        gCrossHair[i].field_0 = 0;
        gCrossHair[i].field_8 = CVector(0.0f, 0.0f, 0.0f);
        gCrossHair[i].field_4 = 0;
        *(_BYTE *)((gCrossHair + 20) = -1;
        *(_BYTE *)((gCrossHair + 21) = 0;
        *(_BYTE *)((gCrossHair + 22) = 0;
        *(_BYTE *)((gCrossHair + 23) = 127;
        *(_DWORD *)((gCrossHair + 8) + 16) = 1.0f;
        *(_DWORD *)((gCrossHair + 8) + 20) = 0;
        *(_BYTE *)((gCrossHair + 8) + 32) = 0;
        *(_DWORD *)((gCrossHair + 8) + 28) = 0;
    }
}

CWeaponEffects::Render()
{
   if(TheCamera.b1rstPersonRunCloseToAWall)
    {
        return;
    }
    for(size_t i = 0; i < ELEMS_COUNT(gCrossHair); i++)
    {
        if(gCrossHair[i].uiClearTime == 0 || gCrossHair[i].uiClearTime >= CTimer::GetCurrentTimeMs())
        {
            gCrossHair[i].bActive = false;
            gCrossHair[i].uiClearTime = 0;
            break;
        }
        if(gCrossHair[i].uiClearTime == -1)
        {
            break;
        }
        if(CWorld::Players[0].field_0 == 0)
        {
            gCrossHair[i].bActive = false;
            break;
        }
        if(!gCrossHair[i].bActive)
        {
            break;
        }
        if(gCrossHair[i].field_28 == 1)
        {
            CVector camView; // vecTargetPos in camera view space
            float screenX;
            float screenY;
            if(CSprite::CalcScreenCoors(gCrossHair[i].vecTargetPos, &camView, &screenX, &screenY, true, true))
            {
                float scale = 20.0f / screenY;
                if(scale > 1.0f)
                {
                    screenX *= scale;
                    screenY *= scale;
                }
                RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
                RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
                RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, TRUE);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, 5);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, 6);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, gpCrossHairTexFlight[0]->raster);
                for(size_t j = 0; j < 2; j++)
                {
                    float h = ClampMax(screenY, 20.0f) * (0.95f + j) * 1.8f;
                    float w = ClampMax(screenX, 28.0f) * (0.95f + j) * 1.8f;
                    CSprite::RenderOneXLUSprite_Rotate_Aspect(camView.x, camView.y, camView.z, w, h, 0, 0, 0, 255, 1.0f/100.0f, 0, 255);
                }
                float h = ClampMax(screenY, 20.0f) * 1.8f;
                float w = ClampMax(screenX, 28.0f) * 1.8f;
                CSprite::RenderOneXLUSprite_Rotate_Aspect(camView.x, camView.y, camView.z, w, h, gCrossHair[i].red, gCrossHair[i].green, gCrossHair[i].blue, 255, 1.0f/100.0f, 0, gCrossHair[i].alpha);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, gpCrossHairTexFlight[1]->raster);
                
                v15 = (CTimer::GetCurrentTimeMs() % 1024) * 0.006135923322290182;
                gCrossHair[i].uiScreenSize -= 20.0 * v15;
                gCrossHair[i].uiScreenSize = ClampMin(gCrossHair[i].uiScreenSize, 0.0f);
                gCrossHair[i].uiScreenScale = gCrossHair[i].uiScreenSize == 0.0f ? 1.0f : 0.0f;
                flt_C8A89C = sin(v15) * gCrossHair[i].uiScreenSize;
                flt_C8A898 = cos(v15) * gCrossHair[i].uiScreenSize;
                for(size_t j = 0; j < 2; j++)
                {
                    float h = ClampMax(screenY, 20.0f) * (0.95f + j) / 1.25f;
                    float w = ClampMax(screenX, 20.0f) * (0.95f + j) / 1.25f;
                    CSprite::RenderOneXLUSprite_Rotate_Aspect(camView.x - flt_C8A89C, camView.y - flt_C8A898, camView.z, w, h, 0, 0, 0, 255, 0.009999999776482582, v15, -1);
                }
                float h = ClampMax(screenY, 20.0f) / 1.25f;
                float w = ClampMax(screenX, 28.0f) / 1.25f;
                CSprite::RenderOneXLUSprite_Rotate_Aspect(camView.x - flt_C8A89C, camView.y - flt_C8A898, v106, v30, v29, gCrossHair[i].red, gCrossHair[i].green, gCrossHair[i].blue, gCrossHair[i].alpha, 1.0f/100.0f, v15, 255);
            }
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, TRUE);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, TRUE);
        }
        else
        {
              RwEngineInstance->dOpenDevice.fpRenderStateSet(8, LODWORD(0));
      RwEngineInstance->dOpenDevice.fpRenderStateSet(6, LODWORD(0));
      RwEngineInstance->dOpenDevice.fpRenderStateSet(12, 1);
      RwEngineInstance->dOpenDevice.fpRenderStateSet(1, LODWORD(0));
      v34 = v3 - 8;
      CSprite__CalcScreenCoors((RwV3D *)(v3 - 8), &v104, &v95, &v94, SLOBYTE(0), SLOBYTE(0));
      if ( (double)RsGlobal.MaximumWidth < v104.x
        || v104.x < 0.0
        || (double)RsGlobal.MaximumHeight < v105
        || v105 < 0.0 )
      {
        if ( (unsigned __int8)CGameLogic__IsCoopGameGoingOn(v33) )
        {
          v52 = *(_DWORD *)v97;
          v53 = *(_DWORD *)(*(_DWORD *)v97 + 20);
          v55 = *(float *)(v3 - 4) - TheCamera.cameraXYZ.matrix.matrix.pos.y;
          v56 = atan2(
                  (*(float *)v34 - TheCamera.cameraXYZ.matrix.matrix.pos.x)
                * TheCamera.cameraXYZ.matrix.matrix.right.x
                + TheCamera.cameraXYZ.matrix.matrix.right.z
                * (*(float *)v3 - TheCamera.cameraXYZ.matrix.matrix.pos.z)
                + TheCamera.cameraXYZ.matrix.matrix.right.y * v55,
                  TheCamera.cameraXYZ.matrix.matrix.at.x * (*(float *)v34 - TheCamera.cameraXYZ.matrix.matrix.pos.x)
                + TheCamera.cameraXYZ.matrix.matrix.at.z * (*(float *)v3 - TheCamera.cameraXYZ.matrix.matrix.pos.z)
                + TheCamera.cameraXYZ.matrix.matrix.at.y * v55);
          *(float *)&v101 = sin(v56);
          v54 = cos(v56);
          if ( v53 == LODWORD(0) )
            v57 = v52 + 4;
          else
            v57 = v53 + 48;
          v58 = *(float *)v34 - *(float *)v57;
          v59 = *(float *)(v3 - 4) - *(float *)(v57 + 4);
          v60 = *(float *)v3 - *(float *)(v57 + 8);
          v61 = atan2(
                  v58 * TheCamera.cameraXYZ.matrix.matrix.right.x
                + TheCamera.cameraXYZ.matrix.matrix.right.z * v60
                + TheCamera.cameraXYZ.matrix.matrix.right.y * v59,
                  TheCamera.cameraXYZ.matrix.matrix.at.x * v58
                + TheCamera.cameraXYZ.matrix.matrix.at.z * v60
                + TheCamera.cameraXYZ.matrix.matrix.at.y * v59);
          v98 = sin(v61);
          v96 = cos(v61);
          v110 = (1.0 - v54) * (double)RsGlobal.MaximumWidth * 0.5;
          v111 = (1.0 - *(float *)&v101) * (double)RsGlobal.MaximumHeight * 0.5;
          v62 = v98 * 0.5 + v96;
          v100 = v62;
          v112 = v62 * flt_8D6140 + v110;
          v63 = flt_8D6140 * v98;
          v64 = v96 * flt_8D6140;
          v113 = v63 + v111 - v64 * 0.5;
          v114 = v64 + v110 - v63 * 0.5;
          v65 = v96 * 0.5 + v98;
          *(float *)&v101 = v65;
          v115 = v65 * flt_8D6140 + v111;
          sub_70D320(
            LODWORD(v110),
            LODWORD(v111),
            LODWORD(v112),
            LODWORD(v113),
            LODWORD(v114),
            LODWORD(v115),
            2.5,
            SLOBYTE(0),
            SLOBYTE(0),
            SLOBYTE(0),
            255,
            1065353216,
            -1);
          v66 = *(_BYTE *)(v3 + 6);
          v67 = *(_BYTE *)(v3 + 5);
          v68 = *(_BYTE *)(v3 + 4);
          v110 = v96 * flt_8D6140 * 0.1 + v110;
          v69 = flt_8D6140 * v98;
          v111 = 0.1000000014901161 * v69 + v111;
          v112 = v100 * flt_8D6140 * 0.800000011920929 + v110;
          v70 = v96 * flt_8D6140;
          v113 = (v69 - 0.5 * v70) * 0.800000011920929 + v111;
          v114 = (v70 - v69 * 0.5) * 0.800000011920929 + v110;
          v115 = *(float *)&v101 * flt_8D6140 * 0.800000011920929 + v111;
          sub_70D320(
            LODWORD(v110),
            LODWORD(v111),
            LODWORD(v112),
            LODWORD(v113),
            LODWORD(v114),
            LODWORD(v115),
            2.5,
            v68,
            v67,
            v66,
            255,
            1065353216,
            -1);
        }
      }
      else
      {
        v35 = max(*(float *)(v3 + 8), 1.200000047683716);
        v36 = max(v35, 0.300000011920929);
        *(float *)(v3 + 8) = v36;
        v98 = 5.0 * v36;
        v96 = v36 * 25.0;
        if ( v106 < 2.5 )
          LODWORD(v106) = 1075838976;
        v37 = sub_609CD0(*(_DWORD *)v97);
        if ( v37 > 0.0 )
        {
          v98 = v37 * flt_8D6148;
          v96 = *(float *)(v3 + 8) * 20.0 + v98;
        }
        RwEngineInstance->dOpenDevice.fpRenderStateSet(10, 1);
        RwEngineInstance->dOpenDevice.fpRenderStateSet(11, 1);
        v38 = 0;
        LODWORD(v100) = 0;
        do
        {
          v39 = (double)SLODWORD(v100) * 120.0;
          v40 = 0.01745329238474369 * v39 + *(float *)(v3 + 12);
          v41 = v98 + *(float *)v2;
          v110 = -(sin(v40) * v41);
          v42 = -(cos(v40) * v41);
          v43 = (v39 + 15.0) * 0.01745329238474369 + *(float *)(v3 + 12);
          v44 = v96 + *(float *)v2;
          v112 = -(sin(v43) * v44);
          v113 = -(cos(v43) * v44);
          v45 = (v39 - 15.0) * 0.01745329238474369 + *(float *)(v3 + 12);
          v114 = -(sin(v45) * v44);
          v115 = -(cos(v45) * v44);
          v110 = v110 + v104.x;
          v111 = v42 + v105;
          v112 = v112 + v104.x;
          v113 = v113 + v105;
          v114 = v114 + v104.x;
          v115 = v115 + v105;
          sub_70D320(
            LODWORD(v110),
            LODWORD(v111),
            LODWORD(v112),
            LODWORD(v113),
            LODWORD(v114),
            LODWORD(v115),
            v106,
            SLOBYTE(0),
            SLOBYTE(0),
            SLOBYTE(0),
            255,
            1065353216,
            *(_BYTE *)(v3 + 7));
          v46 = (v114 + v112 + v110) * 0.3333333432674408;
          v47 = (v115 + v113 + v111) * 0.3333333432674408;
          v110 = (v110 - v46) * 0.75 + v46;
          v111 = (v111 - v47) * 0.75 + v47;
          v112 = (v112 - v46) * 0.75 + v46;
          v113 = (v113 - v47) * 0.75 + v47;
          v114 = (v114 - v46) * 0.75 + v46;
          v115 = (v115 - v47) * 0.75 + v47;
          RwEngineInstance->dOpenDevice.fpRenderStateSet(10, 5);
          RwEngineInstance->dOpenDevice.fpRenderStateSet(11, 6);
          RwEngineInstance->dOpenDevice.fpRenderStateSet(20, 1);
          sub_70D320(
            LODWORD(v110),
            LODWORD(v111),
            LODWORD(v112),
            LODWORD(v113),
            LODWORD(v114),
            LODWORD(v115),
            v106,
            *(_BYTE *)(v3 + 4),
            *(_BYTE *)(v3 + 5),
            *(_BYTE *)(v3 + 6),
            255,
            1065353216,
            *(_BYTE *)(v3 + 7));
          ++v38;
          LODWORD(v100) = v38;
        }
        while ( v38 < 3 );
        v48 = *(_DWORD *)(v3 - 12);
        if ( v48 <= LODWORD(0) || v48 == -1 )
        {
          *(float *)(v3 + 12) = *(float *)(v3 + 12) + 0.05000000074505806;
        }
        else
        {
          *(float *)(v3 + 12) = *(float *)(v3 + 12) + 0.75;
          *(float *)v2 = *(float *)v2 + 2.0;
          *(float *)(v3 + 8) = *(float *)(v3 + 8) * 0.8999999761581421;
        }
        if ( *(float *)(v3 + 12) > 6.283185482025146 )
          *(_DWORD *)(v3 + 12) = LODWORD(0);
        if ( v48 == LODWORD(0) )
        {
          v49 = v99;
          if ( byte_8D6144[v99] )
          {
            v50 = *(float *)(v3 + 8) + *(float *)(v3 + 8) + *(float *)v2;
            *(float *)v2 = v50;
            if ( v50 > *(float *)(v3 + 8) * 20.0 )
              byte_8D6144[v49] = 0;
          }
          else
          {
            v51 = *(float *)v2 - 2.0;
            *(float *)v2 = v51;
            if ( v51 < 0.0 )
              byte_8D6144[v49] = 1;
          }
        }
      }
      RwEngineInstance->dOpenDevice.fpRenderStateSet(12, LODWORD(0));
      RwEngineInstance->dOpenDevice.fpRenderStateSet(8, 1);
      RwEngineInstance->dOpenDevice.fpRenderStateSet(6, 1);
        }
    }
 
if(TheCamera.cams[TheCamera.activeCam].Mode == 49)
{
  RwEngineInstance->dOpenDevice.fpRenderStateSet(8, LODWORD(0));
  RwEngineInstance->dOpenDevice.fpRenderStateSet(6, LODWORD(0));
  RwEngineInstance->dOpenDevice.fpRenderStateSet(12, 1);
  RwEngineInstance->dOpenDevice.fpRenderStateSet(10, 5);
  RwEngineInstance->dOpenDevice.fpRenderStateSet(11, 6);
  RwEngineInstance->dOpenDevice.fpRenderStateSet(1, gpCrossHairTex->raster);
  v74 = (CTimer::GetCurrentTimeMs() % 1024) * 0.006135923322290182;
  v75 = (*(float *)(TheCamera.cams[TheCamera.activeCam].Y_Targetting + 1.0) * RsGlobal.MaximumHeight * 0.5;
  v76 = (*(float *)(TheCamera.cams[TheCamera.activeCam].X_Targetting + 1.0) * RsGlobal.MaximumWidth * 0.5;
  sub_70D490(v76, v75, 100.0, 10.0, 10.0, 0xFFu, 0x80u, LOBYTE(0), 128, 0.009999999776482582, v74, -1);
  v71 = ;
  v72 = (int);
  if ( CWorld::Players[0].pPed->pCurrentObjective != NULL)
  {
    if ( CWorld::Players[0].pPed->pCurrentObjective->pDriver != CWorld__Players[0].pPed )
      CWorld__Players[1].pPed = (int)CWorld__Players[0].pPed;
  }
  v77 = 0;
  if ( CWorld__Players[1].pPed != LODWORD(0) )
  {
        v80 = CWorld__Players[1].pPed->Weapons[CWorld__Players[1].pPed->bCurrentWeaponSlot].m_eWeaponType;
        v81 = CPed__GetWeaponSkill((int)CWorld__Players[1].pPed);
        v78 = CWeaponInfo__GetWeaponInfo(v80, v81);
        v79 = v72->__parent.__parent.__parent.xyz;
        if ( v79 == LODWORD(v1) )
          v82 = (int)&v72->__parent.__parent.__parent.transform;
        else
          v82 = (int)&v79->matrix.matrix.pos;
        v83 = v77->z + v77->z;
    CWeapon__FindNearestTargetEntityWithScreenCoors(TheCamera.cams[TheCamera.activeCam].X_Targetting, TheCamera.cams[heCamera.activeCam].Y_Targetting,
      v83, CWorld__Players[1].pPed->GetPos(), 0, 0);
  }
  if ( v77 != dword_C8A894 )
  {
    dword_C8A894 = v77;
    dword_C8A890 = LODWORD(CTimer__m_snTimeInMilliseconds);
  }
  if ( v77 != LODWORD(0) )
  {
    v84 = *(_DWORD *)(v77 + 20);
    if ( v84 == LODWORD(0) )
      v85 = v77 + 4;
    else
      v85 = v84 + 48;
    if ( CSprite__CalcScreenCoors((RwV3D *)v85, &v104, &v95, &v94, 1, 1) )
    {
      v86 = 20.0 / v94;
      if ( v86 > 1.0 )
      {
        v95 = v95 * v86;
        v94 = v86 * v94;
      }
      v101 = LODWORD(CTimer__m_snTimeInMilliseconds) - dword_C8A890;
      v87 = 3.0 - (double)(LODWORD(CTimer__m_snTimeInMilliseconds) - dword_C8A890) * 0.001953125;
      if ( v87 < 1.0 )
        v87 = 1.0;
      v88 = (LODWORD(CTimer__m_snTimeInMilliseconds) - dword_C8A890) / 4 + 70;
      if ( v88 > 255 )
        LOWORD(v88) = 255;
      v101 = LOWORD(CTimer__m_snTimeInMilliseconds) & 0x3FF;
      v89 = (double)(LOWORD(CTimer__m_snTimeInMilliseconds) & 0x3FF) * 0.006135923322290182;
      v90 = v94 * v87;
      v91 = v87 * v95;
      sub_70D490(v104.x, v105, v106, v91, v90, v88, LOBYTE(0), LOBYTE(0), v88, 0.009999999776482582, v89, -1);
    }
  }
  RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 1);
  RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 1);
}
v92 = 0;
v93 = (char *)&CWorld__Players[0].field_158;
do
{
  result = sub_56EF90(v92);
  v93 += 400;
  ++v92;
}
while ( (signed int)v93 < (signed int)&CWorld__ms_aSectors[43] );
}