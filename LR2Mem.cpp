#include "LR2Bindings.hpp"
#include "safetyhook/safetyhook.hpp"

static void OnMainLoop(SafetyHookContext& regs) {
	if (LR2::isInit) return;
	__asm {
		push eax
		push ebx
		mov eax, FS:[0x04]
		mov LR2::stackOffset, eax
		mov ebx, dword ptr ds:[eax-0x0A084C]
		mov LR2::pSqlite, ebx
		mov ebx, dword ptr ds:[eax-0x0A07C8]
		mov LR2::pGame, ebx
		pop ebx
		pop eax
	};
	LR2::isInit = true;
}

void LR2::Init() {
	safetyhook::create_mid((void*)0x431970, OnMainLoop);
}