#pragma once

#include "CColPoint.h"

struct EColSurface
{
    enum EColSurfaceValue
    {
        DEFAULT,
        TARMAC,
        TARMAC_FUCKED,
        TARMAC_REALLYFUCKED,
        PAVEMENT,
        PAVEMENT_FUCKED,
        GRAVEL,
        FUCKED_CONCRETE,
        PAINTED_GROUND,
        GRASS_SHORT_LUSH,
        GRASS_MEDIUM_LUSH,
        GRASS_LONG_LUSH,
        GRASS_SHORT_DRY,
        GRASS_MEDIUM_DRY,
        GRASS_LONG_DRY,
        GOLFGRASS_ROUGH,
        GOLFGRASS_SMOOTH,
        STEEP_SLIDYGRASS,
        STEEP_CLIFF,
        FLOWERBED,
        MEADOW,
        WASTEGROUND,
        WOODLANDGROUND,
        VEGETATION,
        MUD_WET,
        MUD_DRY,
        DIRT,
        DIRTTRACK,
        SAND_DEEP,
        SAND_MEDIUM,
        SAND_COMPACT,
        SAND_ARID,
        SAND_MORE,
        SAND_BEACH,
        CONCRETE_BEACH,
        ROCK_DRY,
        ROCK_WET,
        ROCK_CLIFF,
        WATER_RIVERBED,
        WATER_SHALLOW,
        CORNFIELD,
        HEDGE,
        WOOD_CRATES,
        WOOD_SOLID,
        WOOD_THIN,
        GLASS,
        GLASS_WINDOWS_LARGE,
        GLASS_WINDOWS_SMALL,
        EMPTY1,
        EMPTY2,
        GARAGE_DOOR,
        THICK_METAL_PLATE,
        SCAFFOLD_POLE,
        LAMP_POST,
        METAL_GATE,
        METAL_CHAIN_FENCE,
        GIRDER,
        FIRE_HYDRANT,
        CONTAINER,
        NEWS_VENDOR,
        WHEELBASE,
        CARDBOARDBOX,
        PED,
        CAR,
        CAR_PANEL,
        CAR_MOVINGCOMPONENT,
        TRANSPARENT_CLOTH,
        RUBBER,
        PLASTIC,
        TRANSPARENT_STONE,
        WOOD_BENCH,
        CARPET,
        FLOORBOARD,
        STAIRSWOOD,
        P_SAND,
        P_SAND_DENSE,
        P_SAND_ARID,
        P_SAND_COMPACT,
        P_SAND_ROCKY,
        P_SANDBEACH,
        P_GRASS_SHORT,
        P_GRASS_MEADOW,
        P_GRASS_DRY,
        P_WOODLAND,
        P_WOODDENSE,
        P_ROADSIDE,
        P_ROADSIDEDES,
        P_FLOWERBED,
        P_WASTEGROUND,
        P_CONCRETE,
        P_OFFICEDESK,
        P_711SHELF1,
        P_711SHELF2,
        P_711SHELF3,
        P_RESTUARANTTABLE,
        P_BARTABLE,
        P_UNDERWATERLUSH,
        P_UNDERWATERBARREN,
        P_UNDERWATERCORAL,
        P_UNDERWATERDEEP,
        P_RIVERBED,
        P_RUBBLE,
        P_BEDROOMFLOOR,
        P_KIRCHENFLOOR,
        P_LIVINGRMFLOOR,
        P_CORRIDORFLOOR,
        P_711FLOOR,
        P_FASTFOODFLOOR,
        P_SKANKYFLOOR,
        P_MOUNTAIN,
        P_MARSH,
        P_BUSHY,
        P_BUSHYMIX,
        P_BUSHYDRY,
        P_BUSHYMID,
        P_GRASSWEEFLOWERS,
        P_GRASSDRYTALL,
        P_GRASSLUSHTALL,
        P_GRASSGRNMIX,
        P_GRASSBRNMIX,
        P_GRASSLOW,
        P_GRASSROCKY,
        P_GRASSSMALLTREES,
        P_DIRTROCKY,
        P_DIRTWEEDS,
        P_GRASSWEEDS,
        P_RIVEREDGE,
        P_POOLSIDE,
        P_FORESTSTUMPS,
        P_FORESTSTICKS,
        P_FORRESTLEAVES,
        P_DESERTROCKS,
        P_FORRESTDRY,
        P_SPARSEFLOWERS,
        P_BUILDINGSITE,
        P_DOCKLANDS,
        P_INDUSTRIAL,
        P_INDUSTJETTY,
        P_CONCRETELITTER,
        P_ALLEYRUBISH,
        P_JUNKYARDPILES,
        P_JUNKYARDGRND,
        P_DUMP,
        P_CACTUSDENSE,
        P_AIRPORTGRND,
        P_CORNFIELD,
        P_GRASSLIGHT,
        P_GRASSLIGHTER,
        P_GRASSLIGHTER2,
        P_GRASSMID1,
        P_GRASSMID2,
        P_GRASSDARK,
        P_GRASSDARK2,
        P_GRASSDIRTMIX,
        P_RIVERBEDSTONE,
        P_RIVERBEDSHALLOW,
        P_RIVERBEDWEEDS,
        P_SEAWEED,
        DOOR,
        PLASTICBARRIER,
        PARKGRASS,
        STAIRSSTONE,
        STAIRSMETAL,
        STAIRSCARPET,
        FLOORMETAL,
        FLOORCONCRETE,
        BIN_BAG,
        THIN_METAL_SHEET,
        METAL_BARREL,
        PLASTIC_CONE,
        PLASTIC_DUMPSTER,
        METAL_DUMPSTER,
        WOOD_PICKET_FENCE,
        WOOD_SLATTED_FENCE,
        WOOD_RANCH_FENCE,
        UNBREAKABLE_GLASS,
        HAY_BALE,
        GORE,
        RAILTRACK,
    };

    // Enum in a byte
    EColSurface( void )                       {}
    EColSurface( EColSurfaceValue newValue )  { m_ucValue = newValue; }
    operator uint8_t  ( void ) const                 { return m_ucValue; }
    uint8_t   m_ucValue;
};
typedef EColSurface::EColSurfaceValue EColSurfaceValue;

class SurfaceInfo_c
{
public:
    uint8_t m_tyreGrip;
    uint8_t m_wetGrip;
    uint16_t pad;
    union
    {
        struct
        {
            uint32_t flags1;
            uint32_t flags2;
        };
        struct
        {
            uint32_t m_adhesionGroup : 3;
            uint32_t m_skidmarkType : 2;
            uint32_t m_frictionEffect : 3;
            uint32_t m_bulletFx : 3;
            uint32_t m_softLanding : 1;
            uint32_t m_seeThrough : 1;
            uint32_t m_shootThrough : 1;
            uint32_t m_sand : 1;
            uint32_t m_water : 1;
            uint32_t m_shallowWater : 1;
            uint32_t m_beach : 1;
            uint32_t m_steepSlope : 1;
            uint32_t m_glass : 1;
            uint32_t m_stairs : 1;
            uint32_t m_skateable : 1;
            uint32_t m_pavement : 1;
            uint32_t m_roughness : 2; //23
            uint32_t m_flammability : 2;//25
            uint32_t m_createsSparks : 1; //27
            uint32_t m_cantSprintOn : 1;
            uint32_t m_leavesFootsteps : 1;
            uint32_t m_producesFootDust : 1;
            uint32_t m_makesCarDirty : 1; // 31
            // flags2
            uint32_t m_makesCarClean : 1;
            uint32_t m_createsWheelGrass : 1;
            uint32_t m_createsWheelGravel : 1;
            uint32_t m_createsWheelMud : 1;
            uint32_t m_createsWheelDust : 1;
            uint32_t m_createsWheelSand : 1;
            uint32_t m_createsWheelSpray : 1;
            uint32_t m_createsPlants : 1;
            uint32_t m_createsObjects : 1;
            uint32_t m_canClimb : 1;
            uint32_t m_audioConcrete : 1;
            uint32_t m_audioGrass : 1;
            uint32_t m_audioSand : 1;
            uint32_t m_audioGravel : 1;
            uint32_t m_audioWood : 1;
            uint32_t m_audioWater : 1;
            uint32_t m_audioMetal : 1;
            uint32_t m_audioLongGrass : 1;
            uint32_t m_audioTile : 1;
        };
    };
};

class SurfaceInfos_c
{
public:
	static bool CanClimb(uint32_t surfaceId);
	static bool CantSprintOn(uint32_t surfaceId);
	static bool CreatesObjects(uint32_t surfaceId);
	static bool CreatesPlants(uint32_t surfaceId);
	static bool CreatesSparks(uint32_t surfaceId);
	static bool CreatesWheelDust(uint32_t surfaceId);
	static bool CreatesWheelGrass(uint32_t surfaceId);
	static bool CreatesWheelGravel(uint32_t surfaceId);
	static bool CreatesWheelMud(uint32_t surfaceId);
	static bool CreatesWheelSand(uint32_t surfaceId);
	static bool CreatesWheelSpray(uint32_t surfaceId);
	static uint32_t GetAdhesionGroup(uint32_t surfaceId);
	static uint32_t GetAdhesiveLimit(CColPoint& point);
	static uint32_t GetBulletFx(uint32_t surfaceId);
	static uint32_t GetFlammability(uint32_t surfaceId);
	static uint32_t GetFrictionEffect(uint32_t surfaceId);
	static uint32_t GetRoughness(uint32_t surfaceId);
	static uint32_t GetSkidmarkType(uint32_t surfaceId);
    static EColSurfaceValue GetSurfaceIdFromName(char* name);
	static float GetTyreGrip(uint32_t surfaceId);
	static float GetWetMultiplier(uint32_t surfaceId);
	//* Init()
	/* IsAudioConcrete(unsigned int)
	* IsAudioGrass(unsigned int)
	* IsAudioGravel(unsigned int)
	* IsAudioLongGrass(unsigned int)
	* IsAudioMetal(unsigned int)
	* IsAudioSand(unsigned int)
	* IsAudioTile(unsigned int)
	* IsAudioWater(unsigned int)
	* IsAudioWood(unsigned int)
	* IsBeach(unsigned int)
	* IsGlass(unsigned int)
	* IsPavement(unsigned int)
	* IsSand(unsigned int)
	* IsSeeThrough(unsigned int)
	* IsShallowWater(unsigned int)
	* IsShootThrough(unsigned int)
	* IsSkateable(unsigned int)
	* IsSoftLanding(unsigned int)
	* IsStairs(unsigned int)
	* IsSteepSlope(unsigned int)
	* IsWater(unsigned int)
	* LeavesFootsteps(unsigned int)
	*/ 
    static void LoadAdhesiveLimits();
	/* LoadSurfaceAudioInfos()
	* LoadSurfaceInfos()
	* MakesCarClean(unsigned int)
	* MakesCarDirty(unsigned int)
	* ProducesFootDust(unsigned int)
	* SurfaceInfos_c()
	* ~SurfaceInfos_c()
    */
private:
    static uint32_t m_adhesiveLimits[6][6];
    static SurfaceInfo_c m_surfaces[179];
};