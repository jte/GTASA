#pragma once

struct BreakInfo_t
{
};

bool BreakablePluginAttach();
void* BreakableConstructor(void* object,int,int);
/*
	* BreakableDestructor(void*, int, int)
	* BreakableStreamGetSize(void const*, int, int)
	* BreakableStreamRead(RwStream*, int, void*, int, int)
	* BreakableStreamWrite(RwStream*, int, void const*, int, int)
	* BreakableTransfer(RpAtomic*, RpAtomic*)
    */