#include "StdInc.h"

static RwUInt8* gCharsetRasterBits;
static uint8_t gPlateDesign;
static RpMaterial* gPlateMaterial;

bool CCustomCarPlateMgr::Initialise()
{
    int16_t vehicleTxdSlot = CTxdStore::FindTxdSlot("vehicle");
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(vehicleTxdSlot);

    pCharsetTex = RwTextureRead("platecharset", 0);
    RwTextureSetFilterMode(pCharsetTex, rwFILTERLINEAR);
    RwTextureSetAddressing(pCharsetTex, rwTEXTUREADDRESSCLAMP);

    pPlatebackTexTab[0] = RwTextureRead("plateback1", 0);
    RwTextureSetFilterMode(pPlatebackTexTab[0], rwFILTERLINEAR);
    RwTextureSetAddressing(pPlatebackTexTab[0], rwTEXTUREADDRESSCLAMP);

    pPlatebackTexTab[1] = RwTextureRead("plateback2", 0);
    RwTextureSetFilterMode(pPlatebackTexTab[1], rwFILTERLINEAR);
    RwTextureSetAddressing(pPlatebackTexTab[1], rwTEXTUREADDRESSCLAMP);

    pPlatebackTexTab[2] = RwTextureRead("plateback3", 0);
    RwTextureSetFilterMode(pPlatebackTexTab[2], rwFILTERLINEAR);
    RwTextureSetAddressing(pPlatebackTexTab[2], rwTEXTUREADDRESSCLAMP);

    CTxdStore::PopCurrentTxd();
    gCharsetRasterBits = RwRasterLock(CCustomCarPlateMgr::pCharsetTex->raster, 0, 2);
}

bool CCustomCarPlateMgr::GeneratePlateText(char* buffer, int length)
{
    if (length < 4)
    {
        return false;
    }
    buffer[0] = 'A' - (rand() / 0x8000 * -23);
    buffer[1] = 'A' - (rand() / 0x8000 * -23);
    buffer[2] = '0' - (rand() / 0x8000 * -9);
    buffer[3] = '0' - (rand() / 0x8000 * -9);
    for (size_t i = 4; i < length; i += 4)
    {
        buffer[i] = ' ';
        buffer[i+1] = '0' - (rand() / 0x8000 * -9);
        buffer[i+2] = 'A' - (rand() / 0x8000 * -23);
        buffer[i+3] = 'A' - (rand() / 0x8000 * -23);
    }
    return true;
}

void CCustomCarPlateMgr::Shutdown()
{
	if (pCharsetTex)
	{
		RwRasterUnlock(pCharsetTex->raster);
		gCharsetRasterBits = 0;
		RwTextureDestroy(pCharsetTex);
		pCharsetTex = 0;
	}
	for (size_t i = 0; i < ELEMS_COUNT(pPlatebackTexTab); i++)
	{
		if (pPlatebackTexTab[i])
		{
			RwTextureDestroy(pPlatebackTexTab[i]);
		}
	}
	int16_t vehTxdSlot = CTxdStore::FindTxdSlot("vehicle");
	if (vehTxdSlot != -1)
	{
		CTxdStore::RemoveTxd(vehTxdSlot);
	}
}

uint8_t CCustomCarPlateMgr::GetMapRegionPlateDesign()
{
	uint8_t weatherRegion = CWeather::GetWeatherRegion();
	if (weatherRegion == 1)
	{
		return 2;
	}
	else
	{
		return weatherRegion > 2 && weatherRegion <= 4;
	}
}

RpMaterial* CCustomCarPlateMgr::SetupMaterialPlatebackTexture(RpMaterial *material, uint8_t plateDesign)
{
	if (plateDesign == -1)
	{
		plateDesign = GetMapRegionPlateDesign();
	}
	RpMaterialSetTexture(material, pPlatebackTexTab[plateDesign]);
	return material;
}

static void AsciiCharToRasterIndex(char ch, int& index_i, int& index_j)
{
	ch = tolower(ch);
	struct 
	{
		char ch;
		int i;
		int j;
	} map[] =
	{
		{'a', 0, 0},
		{'b', 1, 0},
		{'c', 2, 0},
		{'d', 3, 0},
		{'e', 0, 1},
		{'f', 1, 1},
		{'g', 2, 1},
		{'h', 3, 1},
		{'i', 0, 2},
		{'j', 1, 2},
		{'k', 2, 2},
		{'l', 3, 2},
		{'m', 0, 3},
		{'n', 1, 3},
		{'o', 2, 3},
		{'p', 3, 3},
		{'q', 0, 4},
		{'r', 1, 4},
		{'s', 2, 4},
		{'t', 3, 4},
		{'u', 0, 5},
		{'v', 1, 5},
		{'w', 2, 5},
		{'x', 3, 5},
		{'y', 0, 6},
		{'z', 1, 6},
		{'0', 2, 6},
		{'1', 3, 6},
		{'2', 0, 7},
		{'3', 1, 7},
		{'4', 2, 7},
		{'5', 3, 7},
		{'6', 0, 8},
		{'7', 1, 8},
		{'8', 2, 8},
		{'9', 3, 8}
	};
	for (size_t i = 0; i < ELEMS_COUNT(map); i++)
	{
		if (ch == map[i].ch)
		{
			index_i = map[i].i;
			index_j = map[i].j;
			return;
		}
	}
	index_i = 0;
	index_j = 9;
}

static bool RenderToRaster(char* szName, RwRaster* charsetRaster, int uiDesign, RwRaster* targetRaster)
{
	RwUInt8* targetRasterBytes = RwRasterLock(targetRaster, 0, 5);
	if (!targetRasterBytes || !gCharsetRasterBits)
	{
		return false;
	}
	RwInt32 charsetRasterStride = charsetRaster->stride;
	if (charsetRasterStride == 0)
	{
		return false;
	}
	RwInt32 targetRasterStride = targetRaster->stride;
	if (targetRasterStride == 0)
	{
		return false;
	}
	for (size_t nameChIndex = 0; nameChIndex < 8; nameChIndex++)
    {
		int index_i = 1;
		int index_j = 1;
		AsciiCharToRasterIndex(szName[nameChIndex], index_i, index_j);
		RwUInt8* sourceBytes = &gCharsetRasterBits[32 * (index_i + (index_j * 64))];
		for (size_t rows = 0; rows < 16; rows++)
		{
			memcpy(targetRasterBytes, sourceBytes, 32);
			sourceBytes += charsetRasterStride;
			targetRasterBytes += targetRasterStride;
		}
		targetRasterBytes += 32;
    }
	RwRasterUnlock(targetRaster);
	return true;
}

RwTexture* CCustomCarPlateMgr::CreatePlateTexture(char* plateText, uint8_t plateDesign)
{
	if (plateDesign == -1)
	{
		plateDesign = GetMapRegionPlateDesign();
	}
	RwRaster* raster = RwRasterCreate(64, 16, 32, 0x200 | 0x400 | 0x04);
	if (!raster)
	{
		return NULL;
	}
	if (!pCharsetTex->raster)
	{
		RwRasterDestroy(raster);
		return NULL;
	}
	if (!RenderToRaster(plateText, pCharsetTex->raster, g_pLicenseplateDesigns[plateDesign], raster))
	{
		RwRasterDestroy(raster);
		return NULL;
	}
	RwTexture* texture = RwTextureCreate(raster);
	if (!texture)
	{
		return NULL;
	}
	RwTextureSetName(texture, plateText);
	RwTextureSetFilterMode(texture, rwFILTERNEAREST);
	return texture;
}

RwTexture* CCustomCarPlateMgr::SetupMaterialPlateTexture(RpMaterial* material, char* plateText, uint8_t plateDesign)
{
	if (!material)
	{
		return NULL;
	}
	RwTexture* texture = CreatePlateTexture(plateText, plateDesign);
	if (texture)
	{
		RpMaterialSetTexture(material, texture);
		RwTextureDestroy(texture);
	}
	return texture;
}

RpMaterial* SetTextMaterialCB(RpMaterial* material, void* data)
{
	char* plateText = (char*)data;
	RwTexture* texture = material->texture;
	if (texture)
	{
		if (texture != (RwTexture *)0xFFFFFFF0)
		{
			if (!stricmp("carplate", texture->name))
			{
				gPlateMaterial = material;
				CCustomCarPlateMgr::SetupMaterialPlateTexture(material, plateText, gPlateDesign);
			}
			else if (!stricmp("carpback", texture->name))
			{
				CCustomCarPlateMgr::SetupMaterialPlatebackTexture(material, gPlateDesign);
			}
		}
	}
	return material;
}

static RpAtomic* SetTextAtomicCB(RpAtomic* atomic, void* data)
{
	RpGeometryForAllMaterials(atomic->geometry, SetTextMaterialCB, data);
	return atomic;
}

RpMaterial* CCustomCarPlateMgr::SetupClump(RpClump* clump, char* plateText, uint8_t plateDesign)
{
	gPlateDesign = plateDesign;
	gPlateMaterial = 0;
	RpClumpForAllAtomics(clump, SetTextAtomicCB, (void*)plateText);
	return gPlateMaterial;
}

bool CCustomCarPlateMgr::SetupClumpAfterVehicleUpgrade(RpClump* clump, RpMaterial* material, uint8_t plateDesign)
{
	if (!material)
	{
		return false;
	}
	gPlateDesign = plateDesign;
	gPlateMaterial = material;
	RpClumpForAllAtomics(clump, SetTextAtomicCB, 0);
	return true;
}