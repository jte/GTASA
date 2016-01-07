#include "StdInc.h"

float CAEAudioUtility::GetRandomNumberInRange(float start, float end)
{
    return rand() / 32767.0f * (end - start) + start;
}

int CAEAudioUtility::GetRandomNumberInRange(int start, int end)
{
  return start + (rand() / 32768.0f * (end - start + 1));
}