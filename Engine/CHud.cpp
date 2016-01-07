#include "StdInc.h"

static struct 
{
	const char name[16];
	const char maskName[16];
} SpritesTextureNames[6] = 
{
	{"fist", "fistm"},
	{"siteM16", "siteM16m"},
	{"siterocket", "siterocketm"},
	{"radardisc", "radardiscA"},
	{"radarRingPlane", "radarRingPlaneA"},
	{"SkipIcon", "SkipIconA"}
};

void CHud::Initialise()
{
	  int v0; // eax@1
  int v1; // esi@1
  void *v2; // edi@1
  int v3; // esi@1
  signed int v4; // ebx@1

  CTxdStore::AddTxdSlot("hud");
  v1 = v0;
  CTxdStore::LoadTxd(v0, "MODELS\\HUD.TXD");
  CTxdStore::AddRef(v0);
  CTxdStore::PushCurrentTxd();
  CTxdStore::SetCurrentTxd(v1);
  for (size_t i = 0; i < ELEMS_COUNT(SpritesTextureNames); i++)
  {
	  Sprites[i]
  }
  v2 = &hudTextures2;
  v3 = (int)hudTextureNames2;
  v4 = 6;
  do
  {
    CSprite2d::SetTexture(v2, *(char **)v3, *(char **)(v3 + 4));
    v3 += 8;
    v2 = (char *)v2 + 4;
    --v4;
  }
  while ( v4 );
  CTxdStore::PopCurrentTxd();
  return CHud::ReInitialise();
}