#pragma once

class CTimeInfo
{
public:
    CTimeInfo* FindOtherTimeModel(char *modelName);
private:
    uint8_t m_timeOn;
    uint8_t m_timeOff;
    uint16_t m_objectId;
};