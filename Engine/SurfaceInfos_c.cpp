#include "StdInc.h"


void SurfaceInfos_c::LoadAdhesiveLimits()
{
    CFileMgr::SetDir("");
    int file = CFileMgr::OpenFile("data\\surface.dat", "rb");
    const char* line;
    for (size_t currentRow = 0; (line = CFileLoader::LoadLine(file)); currentRow++)
    {
        if (*line == ';' || !*line)
        {
            continue;
        }
        char name[240];
        sscanf(line, "%s", name);
        for (; *line != ' ' && *line != '\t'; line++)
        {
        }
        for (size_t currentColumn = 0; currentColumn < currentRow + 1; currentColumn++)
        {
            while (*line == ' ' || *line == '\t')
            {
                line++;
            }
            float limit = 0;
            if (*line != '-')
            {
                sscanf(line, "%f", &limit);
            }
            for (; *line && *line != ' ' && *line != '\t'; line++)
            {
            }
            m_adhesiveLimits[currentRow][currentColumn] = limit;
        }
    }
    CFileMgr::CloseFile(file);
}

EColSurfaceValue SurfaceInfos_c::GetSurfaceIdFromName(char* name)
{
    static const char surfaceNames[176][20] = 
    {
        "DEFAULT","TARMAC", "TARMAC_FUCKED", "TARMAC_REALLYFUCKED", "PAVEMENT", "PAVEMENT_FUCKED", "GRAVEL", "FUCKED_CONCRETE", 
        "PAINTED_GROUND", "GRASS_SHORT_LUSH", "GRASS_MEDIUM_LUSH", "GRASS_LONG_LUSH", "GRASS_SHORT_DRY", "GRASS_MEDIUM_DRY", 
        "GRASS_LONG_DRY", "GOLFGRASS_ROUGH", "GOLFGRASS_SMOOTH", "STEEP_SLIDYGRASS", "STEEP_CLIFF", "FLOWERBED", "MEADOW", "WASTEGROUND", 
        "WOODLANDGROUND", "VEGETATION", "MUD_WET", "MUD_DRY", "DIRT", "DIRTTRACK", "SAND_DEEP", "SAND_MEDIUM", "SAND_COMPACT", "SAND_ARID", 
        "SAND_MORE", "SAND_BEACH", "CONCRETE_BEACH", "ROCK_DRY", "ROCK_WET", "ROCK_CLIFF", "WATER_RIVERBED", "WATER_SHALLOW", "CORNFIELD", 
        "HEDGE", "WOOD_CRATES", "WOOD_SOLID", "WOOD_THIN", "GLASS", "GLASS_WINDOWS_LARGE", "GLASS_WINDOWS_SMALL", "EMPTY1", "EMPTY2", 
        "GARAGE_DOOR", "THICK_METAL_PLATE", "SCAFFOLD_POLE", "LAMP_POST", "METAL_GATE", "METAL_CHAIN_FENCE", "GIRDER", "FIRE_HYDRANT", 
        "CONTAINER", "NEWS_VENDOR", "WHEELBASE", "CARDBOARDBOX", "PED", "CAR", "CAR_PANEL", "CAR_MOVINGCOMPONENT", "TRANSPARENT_CLOTH", 
        "RUBBER", "PLASTIC", "TRANSPARENT_STONE", "WOOD_BENCH", "CARPET", "FLOORBOARD", "STAIRSWOOD", "P_SAND", "P_SAND_DENSE", "P_SAND_ARID", 
        "P_SAND_COMPACT", "P_SAND_ROCKY", "P_SANDBEACH", "P_GRASS_SHORT", "P_GRASS_DRY", "P_WOODLAND", "P_WOODDENSE", "P_ROADSIDE", 
        "P_ROADSIDEDES", "P_FLOWERBED", "P_WASTEGROUND", "P_CONCRETE", "P_OFFICEDESK", "P_711SHELF1", "P_711SHELF2", "P_711SHELF3", 
        "P_RESTUARANTTABLE", "P_BARTABLE", "P_UNDERWATERLUSH", "P_UNDERWATERBARREN", "P_UNDERWATERCORAL", "P_UNDERWATERDEEP", "P_RIVERBED", 
        "P_RUBBLE", "P_BEDROOMFLOOR", "P_KIRCHENFLOOR", "P_LIVINGRMFLOOR", "P_CORRIDORFLOOR", "P_711FLOOR", "P_FASTFOODFLOOR", "P_SKANKYFLOOR", 
        "P_MOUNTAIN", "P_MARSH", "P_BUSHY", "P_BUSHYMIX", "P_BUSHYDRY", "P_BUSHYMID", "P_GRASSWEEFLOWERS", "P_GRASSDRYTALL", "P_GRASSLUSHTALL", 
        "P_GRASSGRNMIX", "P_GRASSBRNMIX", "P_GRASSLOW", "P_GRASSROCKY", "P_GRASSSMALLTREES", "P_DIRTROCKY", "P_DIRTWEEDS", "P_GRASSWEEDS", 
        "P_RIVEREDGE", "P_POOLSIDE", "P_FORESTSTUMPS", "P_FORESTSTICKS", "P_FORRESTLEAVES", "P_DESERTROCKS", "P_FORRESTDRY", "P_BUILDINGSITE", 
        "P_DOCKLANDS", "P_INDUSTRIAL", "P_INDUSTJETTY", "P_CONCRETELITTER", "P_ALLEYRUBISH", "P_JUNKYARDPILES", "P_JUNKYARDGRND", "P_DUMP", 
        "P_CACTUSDENSE", "P_AIRPORTGRND", "P_CORNFIELD", "P_GRASSLIGHT", "P_GRASSLIGHTER", "P_GRASSLIGHTER2", "P_GRASSMID1", "P_GRASSMID2", 
        "P_GRASSDARK", "P_GRASSDARK2", "P_GRASSDIRTMIX", "P_RIVERBEDSTONE", "P_RIVERBEDSHALLOW", "P_RIVERBEDWEEDS", "P_SEAWEED", "DOOR", 
        "PLASTICBARRIER", "PARKGRASS", "STAIRSSTONE", "STAIRSMETAL", "STAIRSCARPET", "FLOORMETAL", "FLOORCONCRETE", "BIN_BAG", "THIN_METAL_SHEET", 
        "METAL_BARREL", "PLASTIC_CONE", "PLASTIC_DUMPSTER", "METAL_DUMPSTER", "WOOD_PICKET_FENCE", "WOOD_SLATTED_FENCE", "WOOD_RANCH_FENCE", 
        "HAY_BALE", "GORE", "RAILTRACK"
    };
    for (size_t i = 0; i < ELEMS_COUNT(surfaceNames); i++)
    {
        if (!strcmp(name, surfaceNames[i]))
        {
            return (EColSurfaceValue)i;
        }
    }
    return EColSurface::DEFAULT;
}

bool SurfaceInfos_c::CanClimb(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_canClimb;
}

bool SurfaceInfos_c::CantSprintOn(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_cantSprintOn;
}

bool SurfaceInfos_c::CreatesObjects(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsObjects;
}

bool SurfaceInfos_c::CreatesPlants(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsPlants;
}

bool SurfaceInfos_c::CreatesSparks(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsSparks;
}

bool SurfaceInfos_c::CreatesWheelDust(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelDust;
}

bool SurfaceInfos_c::CreatesWheelGrass(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelGrass;
}

bool SurfaceInfos_c::CreatesWheelGravel(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelGravel;
}

bool SurfaceInfos_c::CreatesWheelMud(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelMud;
}

bool SurfaceInfos_c::CreatesWheelSand(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelSand;
}

bool SurfaceInfos_c::CreatesWheelSpray(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_createsWheelSpray;
}

uint32_t SurfaceInfos_c::GetAdhesionGroup(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_adhesionGroup;
}

uint32_t SurfaceInfos_c::GetAdhesiveLimit(CColPoint& point)
{
    return m_adhesiveLimits[GetAdhesionGroup(point.surfaceTypeA)][GetAdhesionGroup(point.surfaceTypeB)];
}

uint32_t SurfaceInfos_c::GetBulletFx(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_bulletFx;
}

uint32_t SurfaceInfos_c::GetFlammability(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_flammability;
}

uint32_t SurfaceInfos_c::GetFrictionEffect(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_frictionEffect;
}

uint32_t SurfaceInfos_c::GetRoughness(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_roughness;
}

uint32_t SurfaceInfos_c::GetSkidmarkType(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_skidmarkType;
}

float SurfaceInfos_c::GetTyreGrip(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_tyreGrip * 0.1f;
}

float SurfaceInfos_c::GetWetMultiplier(uint32_t surfaceId)
{
    return m_surfaces[surfaceId].m_wetGrip * 0.01f * CWeather::GetWetRoads() + 1.0f;
}