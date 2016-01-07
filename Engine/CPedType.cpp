#include "StdInc.h"

const char aPedTypeNames[][16] =
{
	"PLAYER1", "PLAYER2",
	"PLAYER_NETWORK", "PLAYER_UNUSED",
	"CIVMALE", "CIVFEMALE",
	"COP",
	"GANG1", "GANG2",
	"GANG3", "GANG4",
	"GANG5", "GANG6",
	"GANG7", "GANG8",
	"GANG9", "GANG10",
	"DEALER",
	"MEDIC",
	"FIREMAN",
	"CRIMINAL",
	"BUM",
	"PROSTITUTE",
	"SPECIAL",
	"MISSION1", "MISSION2",
	"MISSION3", "MISSION4",
	"MISSION5", "MISSION6",
	"MISSION7", "MISSION8"
};

int CPedType::FindPedType(char* pedType)
{
	size_t i;
	for (i = 0; i < ELEMS_COUNT(aPedTypeNames); i++)
	{
		if (!strcmp(pedType, aPedTypeNames[i]))
		{
			break;
		}
	}
	return i;
}

uint32_t CPedType::GetPedFlag(int n)
{
	return n >= 32 ? 0 : 1 << n;
}

void CPedType::LoadPedData()
{
	int pedDataFp = CFileMgr::OpenFile("DATA\\PED.DAT", "r");
	char* line = NULL;
	uint32_t pedType = 32;
	for (line = (char*)CFileLoader::LoadLine(pedDataFp); line; line = (char*)CFileLoader::LoadLine(pedDataFp))
	{
		char data[32];
		if (!*line || *line == '#')
		{
			continue;
		}
		sscanf(line, "%s", data);
		if (!strcmp(data, "Hate"))
		{
			uint32_t hate = 0;
			strtok(line, " ,\t");
			for (char* j = strtok(0, " ,\t"); j; j = strtok(0, " ,\t"))
			{
				hate |= GetPedFlag(FindPedType(j));
			}
			ms_apPedTypes[pedType].SetAcquaintances(4, hate);
		}
		else if (!strcmp(data, "Dislike"))
		{
			uint32_t dislike = 0;
			strtok(line, " ,\t");
			for (char* j = strtok(0, " ,\t"); j; j = strtok(0, " ,\t"))
			{
				dislike |= GetPedFlag(FindPedType(j));
			}
			ms_apPedTypes[pedType].SetAcquaintances(3, dislike);
		}
		else if (!strcmp(data, "Like"))
		{
			uint32_t like = 0;
			strtok(line, " ,\t");
			for (char* j = strtok(0, " ,\t"); j; j = strtok(0, " ,\t"))
			{
				like |= GetPedFlag(FindPedType(j));
			}
			ms_apPedTypes[pedType].SetAcquaintances(1, like);
		}
		else if (!strcmp(data, "Respect"))
		{
			uint32_t dislike = 0;
			strtok(line, " ,\t");
			for (char* j = strtok(0, " ,\t"); j; j = strtok(0, " ,\t"))
			{
				dislike |= GetPedFlag(FindPedType(j));
			}
			ms_apPedTypes[pedType].SetAcquaintances(0, dislike);
		}
		else
		{
			pedType = FindPedType(data);
		}
	}
	CFileMgr::CloseFile(pedDataFp);
}

CAcquaintance* CPedType::GetPedTypeAcquaintances(int pedType)
{
	return &ms_apPedTypes[pedType];
}

void CPedType::Initialise()
{
	ms_apPedTypes = new CAcquaintance[32];
	LoadPedData();
}

void CPedType::Shutdown()
{
	if (ms_apPedTypes)
	{
		delete[] ms_apPedTypes;
		ms_apPedTypes = NULL;
	}
}

bool CPedType::PoliceDontCareAboutCrimesAgainstPedType(int pedType)
{
	return pedType >= 7 && pedType <= 16 || pedType == 20 || pedType == 17;
}