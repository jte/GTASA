#include "StdInc.h"

void CCamera::CalculateMirroredMatrix(CVector a1, float a2, CMatrix* a3, CMatrix* a4)
{
    v10 = DotProduct(a1, a3->matrix.pos) - a2;
    a4->matrix.pos.x = a3->matrix.pos.x - a1.x * 2 * v10;
    a4->matrix.pos.y = a3->matrix.pos.y - a1.y * 2 * v10;
    a4->matrix.pos.z = a3->matrix.pos.z - a1.z * 2 * v10;

    v16 = DotProduct(a1, a3->matrix.top);
    a4->matrix.top.x = a3->matrix.top.x - a1.x * 2 * v16;
    a4->matrix.top.y = a3->matrix.top.y - a1.y * 2 * v16;
    a4->matrix.top.z = a3->matrix.top.z - a1.z * 2 * v16;

    v26 = DotProduct(a1, a3->matrix.at);
    a4->matrix.at.x = a3->matrix.at.x - a1.x * 2 * v26;
    a4->matrix.at.y = a3->matrix.at.y - a1.y * 2 * v26;
    a4->matrix.at.z = a3->matrix.at.z - a1.z * 2 * v26;

    a4->matrix.right.x = a4->matrix.at.y * a4->matrix.top.z - a4->matrix.at.z * a4->matrix.top.y;
    a4->matrix.right.y = a4->matrix.at.z * a4->matrix.top.x - a4->matrix.at.x * a4->matrix.top.z;
    a4->matrix.right.z = a4->matrix.at.x * a4->matrix.top.y - a4->matrix.at.y * a4->matrix.top.x;
}

void CCamera::CamShake(float shakeFactor, float shakeX, float shakeY, float shakeZ)
{
    CVector shake(shakeX, shakeY, shakeZ);
    CVector ds = this->cams[this->activeCam].Source - shake;
    float force = Clamp(0.0f, sqrt(ds.z * ds.z + ds.Magnitude2DSquared()), 100.0f);
    v15 = 1.0 - force / 100.0f;
    v12 = (this->fCameraSpeedSoFar - (CTimer::GetCurrentTimeMs() - this->m_uiCamShakeStart) / 1000.0f) * v15;
  
    v13 = v15 * shakeFactor * 0.35;
    if (v13 > Clamp<float>(0.0f, v12, 2.0f))
    {
        this->fCameraSpeedSoFar = v13;
        this->m_uiCamShakeStart = CTimer::GetCurrentTimeMs();
    }
}

void CCamera::Fade(float duration, short inOut)
{
    this->m_fTimeToFadeOut = duration;
    this->m_bFading = 1;
    this->m_iFadingDirection = inOut;
    this->m_uiFadeTimeStarted = CTimer::GetCurrentTimeMs();
    if ( !this->m_bPlayerIsInGarage || inOut == 1 )
    {
        this->m_bMusicFading = 1;
        this->m_iMusicFadingDirection = inOut;
        this->m_fTimeToFadeMusic = Clamp<float>(0.3f, duration * 0.3, duration);
        if ( inOut )
        {
          this->m_fTimeToWaitToFadeMusic = 0;
          this->m_uiFadeTimeStartedMusic = CTimer::GetCurrentTimeMs();
        }
        else
        {
          this->m_fTimeToWaitToFadeMusic = duration - this->m_fTimeToFadeMusic;
          this->m_fTimeToFadeMusic = ClampMin(this->m_fTimeToFadeMusic - 0.1f, 0.0f);
          this->m_uiFadeTimeStartedMusic = CTimer::GetCurrentTimeMs();
        }
    }
}

CVector& CCamera::GetGameCamPosition()
{
    return this->m_vecGameCamPos;
}

void CCamera::SetCamPositionForFixedMode(const CVector& pos, const CVector& dir)
{
    this->m_vecFixedModeSource = pos;
    this->m_vecFixedModeUpOffSet = dir;
    this->m_bGarageFixedCamPositionSet = 0;
}

void CCamera::CalculateDerivedValues(char a2, char a3)
{
    this->m_matInverse = Invert(this->m_cameraMatrix);
    CalculateFrustumPlanes(a2);
    if (this->m_cameraMatrix.matrix.top.x != 0.0 || this->m_cameraMatrix.matrix.top.y != 0.0)
    {
        if (a3)
        {
            this->fAlphaForPlayerAnim1rstPerson = atan2(this->m_cameraMatrix.matrix.top.y, this->m_cameraMatrix.matrix.top.x);
        }
    }
    else
    {
        this->m_cameraMatrix.matrix.top.x = 0.0001f;
    }
  
  
    v7 = pThis->m_cameraMatrix.matrix.top.x;
    v6 = pThis->m_cameraMatrix.matrix.top.y;
    pThis->CamFrontYNorm = pThis->m_cameraMatrix.matrix.top.x;
    pThis->DistanceToWater = pThis->m_cameraMatrix.matrix.top.y;
    v5 = sqrt(v6 * v6 + v7 * v7);
    if (v5 == 0.0f)
    {
        pThis->CamFrontYNorm = 1.0f;
    }
    else
    {
        v8 = 1.0 / v5;
        pThis->CamFrontYNorm = v8 * v7;
        pThis->DistanceToWater = v8 * v6;
    }
}

void CCamera::ProcessVectorTrackLinear()
{
    float currentTime = CTimer::GetCurrentTimeMs();
    if (currentTime > m_vectorTrackLinearEndTime)
    {
        if (m_lockCameraTargetPos)
        {
            m_processedVectorTrackLinear = true;
        }
    }
    else
    {
        float currentTransverseTime = Unlerp(m_vectorTrackLinearStartTime, currentTime, m_vectorTrackLinearEndTime);
        ProcessVectorTrackLinear(currentTransverseTime);
    }
}

void CCamera::ProcessVectorTrackLinear(float time)
{
    m_processedVectorTrackLinear = true;
    float weight = time;
    if (m_vectorTrackLinearConstSpeed)
    {
        weight = (sin(DEG_TO_RAD(270.0f - time * 180.0f)) + 1.0f) / 2.0f;
    }
    m_vectorTrackLinearCurrent = Lerp(m_vectorTrackLinearStart, weight, m_vectorTrackLinearEnd);
}

void CCamera::VectorTrackLinear(CVector* start, CVector* end, float duration, bool constSpeed)
{
    m_vectorTrackLinearStartTime = CTimer::GetCurrentTimeMs();
    m_vectorTrackLinearEndTime = m_vectorTrackLinearStartTime + duration;
    m_vectorTrackLinearStart = *start;
    m_vectorTrackLinearEnd = *end;
    m_vectorTrackLinearConstSpeed = constSpeed;
}