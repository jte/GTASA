#include "StdInc.h"

int __fastcall cHandlingDataMgr::FindExactWord(const char *a2, const char *a3, int a4, int a5)
{
  int v5; // r10@1
  const char *v6; // r9@1
  const char *v7; // r4@1
  int v8; // r8@1
  int v9; // r6@1
  size_t v10; // r5@2
  int result; // r0@6
  int v12; // [sp+0h] [bp+0h]@2
  int v13; // [sp+14h] [bp+14h]@1

  v5 = a4;
  v6 = a2;
  v7 = a3;
  v8 = a5;
  v9 = 0;
  while ( 1 )
  {
    v10 = strlen(v7);
    strncpy((char *)&v12, v7, v10);
    if ( !strncmp(v6, (const char *)&v12, v10) )
      break;
    ++v9;
    v7 += v5;
    if ( v9 >= a5 )
      goto LABEL_6;
  }
  v8 = v9;
LABEL_6:
  result = v8;
  return result;
}

uint32_t cHandlingDataMgr::FindExactWord(char* line, char* nameTable, int entrySize, int entryCount)
{
	char entry[20];
	uint32_t entryIndex;
	const char* p = nameTable;
	for (entryIndex = 0; entryIndex < entryCount; entryIndex++)
	{
		uint32_t entryLength = strlen(p);
		strncpy(entry, p, entryLength);
		if (!strncmp(line, entry, entryLength))
		{
			break;
		}
		p += entrySize;
	}
	return entryIndex;
}