#include "StdInc.h"

FxInterpInfoFloat_c::FxInterpInfoFloat_c()
{
    m_valueArray = NULL;
}

void FxInterpInfoFloat_c::Load(int fp)
{
    for (size_t i = 0; i < m_numValues; i++)
    {
        char line[256];
        char dummy[128];
        ReadLine(fp, line, 256);
        sscanf(line, "%s", dummy);
        ReadLine(fp, line, 256);
        sscanf(line, "%s", dummy);
        ReadLine(fp, line, 256);
        sscanf(line, "%s %d", dummy, &m_looped);
        ReadLine(fp, line, 256);
        sscanf(line, "%s %d", dummy, &m_numKeys);
        if (i == 0)
        {
            m_timeArray = g_fxMan.GetMemoryPool().GetMem(2 * m_numKeys, 2u);
        }
        m_valueArray[i] = g_fxMan.GetMemoryPool().GetMem(4 * m_numKeys, 4u);
        for (size_t j = 0; j < m_numKeys; j++)
        {
            ReadLine(fp, line, 256);
            sscanf(line, "%s", dummy);
            
            ReadLine(fp, line, 256);
            float time;
            sscanf(line, "%s %f", dummy, &time);
            
            m_timeArray[j] = FloatToFixedPoint<256>(time);
            ReadLine(fp, line, 256);
            sscanf(line, "%s %f", dummy, &m_valueArray[i][j]);
        }
    }
}

void FxInterpInfoFloat_c::GetVal(float* values, float time)
{
    if (m_numKeys == 1)
    {
        for (size_t i = 0; i < m_numValues; i++)
        {
            values[i] = m_valueArray[i][0];
        }
    }

    if (m_looped)
    {
        float lastTime = FixedPointToFloat<256>(m_timeArray[m_numKeys - 1]);
        time = time - (time / lastTime) * lastTime;
    }
    size_t k;
    for (k = 1; k < m_numKeys; k++)
    {
        float to = FixedPointToFloat<256>(m_timeArray[k]);
        if (time < to)
        {
            float from = FixedPointToFloat<256>(m_timeArray[k - 1]);
            float rel = Unlerp(from, time, to);
            for (size_t j = 0; j < m_numValues; j++)
            {
                values[j] = Lerp(m_valueArray[j][k - 1], rel, m_valueArray[j][k]);
            }
            break;
        }
    }
    if (m_numKeys <= 2 || k >= m_numKeys)
    {
        for (size_t i = 0; i < m_numValues; ++i)
        {
            values[i] = m_valueArray[i][m_numKeys - 1];
        }
    }
}

float FxInterpInfoFloat_c::GetVal(int startValue, float a3, float a4)
{
    if (m_numKeys == 1)
    {
        return a4 * m_valueArray[startValue][0];
    }
    float v5 = 0.0;
    float v6 = a3 - a4;//time step
    float v12 = a3;//current value
    size_t currentKey;//=v9
    for (currentKey = 0; currentKey < m_numKeys; currentKey++)
    {
        float nextTime = FixedPointToFloat<256>(m_timeArray[currentKey + 1]);//=v11
        if (v6 < nextTime)
        {
            if (currentKey == 0)
            { // first key, no interpolation done
                v12 = m_valueArray[startValue][currentKey];
            }
            else
            {
                float curValue = m_valueArray[startValue][currentKey];
                float prevValue = m_valueArray[startValue][currentKey - 1];
                float prevTime = FixedPointToFloat<256>(m_timeArray[currentKey - 1]);
                v12 = Lerp(prevValue, Unlerp(prevTime, v6, nextTime), curValue);
            }
            break;
        }
    }
    if (currentKey == m_numKeys)
    {
        return a4 * m_valueArray[startValue][currentKey - 1];
    }
    for (; currentKey < m_numKeys; currentKey++)
    {
        float curTime = FixedPointToFloat<256>(m_timeArray[currentKey]);//=a4a
        if (curTime == a3)
        {
            v5 += Lerp(m_valueArray[startValue][currentKey], 0.5f, v12) * (curTime - v6);
            break;
        }
        if (curTime > a3)
        {
            float curValue = m_valueArray[startValue][currentKey];
            float prevValue = m_valueArray[startValue][currentKey - 1];
            float prevTime = FixedPointToFloat<256>(m_timeArray[currentKey - 1]);
            float v17 = Lerp(v12, 0.5f, Lerp(prevValue, Unlerp(prevTime, a3, curTime), curValue));
            v5 += v17 * (a3 - v6);
            break;
        }
        v5 += ((m_valueArray[startValue][currentKey] - v12) * 0.5 + v12) * (curTime - v6);
        v6 = curTime;
        v12 = m_valueArray[startValue][currentKey];
    }
    if (currentKey == m_numKeys)
    {
        v5 += Lerp(v12, 0.5f, m_valueArray[startValue][currentKey - 1]) * (FixedPointToFloat<256>(m_timeArray[currentKey - 1]) - v6);
    }
    return v5;
}