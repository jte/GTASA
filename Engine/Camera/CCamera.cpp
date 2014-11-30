#include "StdInc.h"

void CCamera::ProcessFade()
{
    if(m_bFading)
    {
        if(m_iFadingDirection == 1)
        {
                  if ( this->fFadeDuration == 0.0 )
        LODWORD(this->fFadeAlpha) = 0;
      else
        this->fFadeAlpha = this->fFadeAlpha - CTimer__ms_fTimeStep * 0.01999999955296516 / this->fFadeDuration * 255.0;
      if ( this->fFadeAlpha > 0.0 )
        goto LABEL_16;
      this->bFading = 0;
        }
        else
        {
                  if ( v1 )
        goto LABEL_16;
      if ( this->fFadeAlpha >= 255.0 )
        this->bFading = 0;
      LODWORD(v2) = 1132396544;
      if ( this->fFadeDuration == 0.0 )
        LODWORD(this->fFadeAlpha) = 1132396544;
      else
        this->fFadeAlpha = CTimer__ms_fTimeStep * 0.01999999955296516 / this->fFadeDuration * 255.0 + this->fFadeAlpha;
      if ( this->fFadeAlpha < 255.0 )
        goto LABEL_16;
        }
        m_fFadeAlpha = v2;
LABEL_16:
    CDraw__FadeValue.alpha = (unsigned __int64)this->fFadeAlpha;
    }
}