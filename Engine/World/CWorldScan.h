#pragma once

class CWorldScan
{
public:
	void ScanWorld(RwV2d*, int, void (*)(int, int));
	void SetExtraRectangleToScan(float, float, float, float);
private:
};