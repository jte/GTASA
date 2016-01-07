#include "StdInc.h"

void CAnimBlendNode::Init()
{
    m_assoc = NULL;
    m_sequence = NULL;
    m_timeDelta = 0.0f;
    m_startKeyFrameId = -1;
    m_endKeyFrameId = -1;
}

void CAnimBlendNode::GetCurrentTranslation(CVector& currentTranslation, float mult)
{
    // Initialize vector
    currentTranslation = CVector(0.0f, 0.0f, 0.0f);
    const SClumpAnimAssoc& clumpAssoc = m_assoc->GetClumpAnimAssoc();
    // Distance between keyframes
    float dist = clumpAssoc.blendAmount;
    if (!(clumpAssoc.flags & 0x10))
    {
        dist *= mult;
    }
    if (dist > 0.0f && m_sequence->GetIsRoot())
    {
        SRootKeyFrame* startKeyFrame = (SRootKeyFrame*)m_sequence->GetKeyFrame(m_startKeyFrameId, true);
        SRootKeyFrame* endKeyFrame = (SRootKeyFrame*)m_sequence->GetKeyFrame(m_endKeyFrameId, true);
        float time = 0.0f;
        if (endKeyFrame->time != 0.0f)
        {
            time = (endKeyFrame->time - m_timeDelta) / endKeyFrame->time;
        }
        currentTranslation = Lerp(startKeyFrame->translation, time, endKeyFrame->translation) * dist;
    }
}

void CAnimBlendNode::GetEndTranslation(CVector& endTranslation, float mult)
{
    endTranslation = CVector(0.0f, 0.0f, 0.0f);
    const SClumpAnimAssoc& clumpAssoc = m_assoc->GetClumpAnimAssoc();
    float dist = clumpAssoc.blendAmount;
    if (!(clumpAssoc.flags & 0x10))
    {
        dist *= mult;
    }
    if (dist > 0.0f && m_sequence->GetIsRoot())
    {
        SRootKeyFrame* endKeyFrame = (SRootKeyFrame*)m_sequence->GetKeyFrame(m_sequence->GetNumKeyFrames() - 1, true);
        endTranslation = endKeyFrame->translation * dist;
    }
}

void CAnimBlendNode::CalcDeltas()
{
    if (!m_sequence->GetIsInitialized())
    {
        return;
    }
    bool root = m_sequence->GetIsRoot();
    SRootKeyFrame* startKeyFrame = (SRootKeyFrame*)m_sequence->GetKeyFrame(m_startKeyFrameId, root);
    SRootKeyFrame* endKeyFrame = (SRootKeyFrame*)m_sequence->GetKeyFrame(m_endKeyFrameId, root);
    GetSlerpParams(startKeyFrame->rotation, endKeyFrame->rotation, m_theta0, m_theta1);
}

bool CAnimBlendNode::SetupKeyFrameCompressed()
{
    if (m_sequence->GetNumKeyFrames() == 0)
    {
        return false;
    }
    m_endKeyFrameId = 1;
    m_startKeyFrameId = 0;
    if (m_sequence->GetNumKeyFrames() == 1)
    {
        m_endKeyFrameId = 0;
        m_timeDelta = 0.0f;
    }
    else
    {
              v4 = LOBYTE(v1->usFlags);
      v3 = v1->pKeyFrames;
      if ( (v4 >> 1) & 1 )
        v5 = v3 + 16;
      else
        v5 = v3 + 10;
      this->fTimeDelta = (double)*(_WORD *)(v5 + 8) * 0.016666668;
    }
    CalcDeltasCompressed();
    return true;
}