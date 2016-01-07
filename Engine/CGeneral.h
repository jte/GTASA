#pragma once

class CGeneral
{
public:
	//* GetATanOfXY(float, float);
	static float GetAngleBetweenPoints(float x1, float y1, float x2, float y2);
	static float GetNodeHeadingFromVector(float x, float y);
	static float GetRadianAngleBetweenPoints(float x1, float y1, float x2, float y2);
	//* GetRandomNumberInRange(float, float)
	//* GetRandomNumberInRange(int, int)
	//* LimitAngle(float)
	//* LimitRadianAngle(float)
	static bool SolveQuadratic(float, float, float, float&, float&);
};