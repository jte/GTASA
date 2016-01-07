#include "StdInc.h"

bool CGeneral::SolveQuadratic(float a, float b, float c, float& x1, float& x2)
{
    // D = b^2 - 4ac
    // x1,2 = (-b+-sqrt(D))/2*a
    float D = b * b - 4.0f * a * c;
    if (D >= 0.0f)
    {
        // Drt = sqrt(D)
        float sqrtD = sqrt(D);
        x1 = (-b - sqrtD) / (2.0f * a);
        x2 = (-b + sqrtD) / (2.0f * a);
        return true;
    }
    else
    {
        return false;
    }
}

float GetAngleBetweenPoints(float x1, float y1, float x2, float y2)
{
    return RAD_TO_DEG(GetRadianAngleBetweenPoints(x1, y1, x2, y2));
}

float CGeneral::GetRadianAngleBetweenPoints(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if (dy == 0.0f)
    {
        dy = 0.0001f;
    }
    // Df = [-pi, pi]
    float angle = atan2(1.0f, dx / dy);
    //   (-,+)|(+,+)
    //  ------|-------
    //   (-,-)|(+,-)
    if (dx <= 0.0f && dy <= 0.0f)
    {   // III quadrant
        return -angle;
    }
    else if (dx <= 0.0f && dy > 0.0f)
    {   // II quadrant
        return -M_PI - angle;
    }
    else if (dx > 0.0f && dy <= 0.0f)
    {   // IV quadrant
        return -angle;
    }
    else if (dx > 0.0f && dy > 0.0f)
    {   // I quadrant
        return M_PI - angle;
    }
}

float CGeneral::GetNodeHeadingFromVector(float x, float y)
{
    float angle = CGeneral::GetRadianAngleBetweenPoints(x, y, 0.0, 0.0);
    if (angle < 0.0f)
    {
        angle += 2*M_PI;
    }
    angle = 2*M_PI - angle + M_PI/8;
    if (angle >= 2*M_PI)
    {
        angle -= 2*M_PI;
    }
    return floor(angle * (M_PI/2) * 8.0);
}