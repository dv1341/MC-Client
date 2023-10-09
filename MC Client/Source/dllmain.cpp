#include <Windows.h>
#include <cstdint>
#include <map>
#include <Psapi.h>

// Include Minhook
#include "../include/MinHook.h"

// Sig Finder
#include "../include/SigFinder.h"

typedef void(__thiscall* KeymapHook)(uint64_t, bool);
KeymapHook _SendKey;

std::map<uint64_t, bool> keymap;

void KeymapDetour(uint64_t key, bool held)
{
	//_SendKey(key, held);
	keymap[key] == held;
}

void Init() {
	if (MH_Initialize() == MH_OK) {
		uintptr_t keymapAddr = SigFinder::findSig("48 83 EC 48 0F B6 C1 4C 8D 05 ? ? ?");//0x1EE2F0(1.20.31)

		if (MH_CreateHook((void*)keymapAddr, &KeymapDetour, reinterpret_cast<LPVOID*>(&_SendKey)) == MH_OK) {
			MH_EnableHook((void*)keymapAddr);

		}
	}
}

BOOL APIENTRY DllMain(HMODULE HModule, DWORD DWord, LPVOID LPVoid)
{
	if (DWord == DLL_THREAD_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
	return TRUE;
}