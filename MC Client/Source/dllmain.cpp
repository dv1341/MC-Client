#include <Windows.h>
#include <cstdint>
#include <map>

// Include Minhook
#include "../include/MinHook.h"


typedef void(__thiscall* KeymapHook)(uint64_t, bool);
KeymapHook _SendKey;

std::map<uint64_t, bool> keymap;

void KeymapDetour(uint64_t key, bool held)
{
	_SendKey(key, held);
	keymap[key] == held;
}

void Init() {
	if (MH_Initialize() == MH_OK) {
		uintptr_t baseAddr = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");

		// look for unknow initial value (byte) hold W key in menu and use hotkey to search for one then let go and search for 0
		// find out what accesses the address then press W again in menu then show dissasembeler
		//  then copy the first one to clipboard only adress and add "0x" to the begining (remember to scan in minecraft.windows.exe in memory scan options)
		uintptr_t keymapAddr = baseAddr + 0x1EE2F0;

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