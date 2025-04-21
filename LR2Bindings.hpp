#include "LR2Typedefs.hpp"

#include <Windows.h>
#include <winnt.h>
#include <winternl.h>

#include <iostream>

typedef unsigned char undefined;

namespace LR2 {
	inline bool isInit = false;
	inline std::uintptr_t stackOffset = 0;

	inline game* pGame = 0;
	inline void* pSqlite = 0;

	static char trampBuff[10];

	static bool Detour32(void* src, void* dst, int len) {
		const unsigned int CALL = 0xE8;
		const unsigned int JMP = 0xE9;
		const unsigned int NOP = 0x90;
		const unsigned int CALL_SIZE = 5;
		const unsigned int JMP_SIZE = 5;

		if (len < CALL_SIZE) return false;

		DWORD curProtection = 0;
		BOOL hResult = VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

		//void* gateway = VirtualAlloc(0, JMP_SIZE + JMP_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		void* gateway = &trampBuff;
		DWORD shit = 0;
		VirtualProtect(gateway, 10, PAGE_EXECUTE_READWRITE, &shit);

		uintptr_t relativeAddress = ((uintptr_t)gateway - (uintptr_t)src) - JMP_SIZE;
		intptr_t  gatewayToSourceRelativeAddr = (intptr_t)src - (intptr_t)gateway - JMP_SIZE - JMP_SIZE;
		intptr_t  gatewayToDestinationRelativeAddr = (intptr_t)dst - (intptr_t)gateway - JMP_SIZE;

		*(char*)((uintptr_t)gateway) = CALL;
		*(uintptr_t*)((uintptr_t)gateway + 1) = gatewayToDestinationRelativeAddr;

		std::memset(src, NOP, len);

		*(BYTE*)src = JMP;
		*(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;

		DWORD temp = 0;
		hResult = VirtualProtect(src, len, curProtection, &temp);

		*(uintptr_t*)((uintptr_t)gateway + 5) = JMP;
		*(uintptr_t*)((uintptr_t)gateway + 6) = gatewayToSourceRelativeAddr + len;

		return true;
	}

	inline __declspec(naked) void zalupaAsm() {
		__asm {
			xor ebx, ebx
			cmp dword ptr ds : LR2::isInit, ebx
			jne retSeg

			push eax
			mov eax, FS : [0x04]
			mov LR2::stackOffset, eax
			mov ebx, dword ptr ds : [eax - 0xA084C]
			mov LR2::pSqlite, ebx
			xor ebx,ebx
			lea eax, dword ptr ds : [eax - 0x0A07C8]
			mov LR2::pGame, eax

			inc LR2::isInit
			pop eax

			retSeg :
			cmp dword ptr ss : [esp + 0x23E70] , ebx
				ret
		};
	}
	static void Init() {
		Detour32((void*)0x431970, zalupaAsm, 9);
	};
}