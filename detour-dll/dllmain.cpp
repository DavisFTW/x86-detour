// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <Windows.h>
typedef int(__cdecl* tSum)(int a, int b);

tSum oSum;

int __cdecl hkSum(int a, int b)
{
   // std::cout << "hooked" << std::endl;


    a = 10;
    b = 10;

	std::cout << "hooked " << std::endl;
    return oSum(a, b);
}

BOOL Detour32(BYTE* src, BYTE* dst, const std::uintptr_t len)
{
    DWORD oldProtection = 0;

    if (len < 5) return false;

    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtection);

    std::uintptr_t addressToJump = dst - src - 5;

    *src = 0xE9;

    *reinterpret_cast<std::uintptr_t*>(src + 1) = addressToJump;

    VirtualProtect(src, len, oldProtection, nullptr);
    return true;

}

BYTE* trampHook32(BYTE* src, BYTE* dst, const std::uintptr_t len)
{
    if (len < 5) return 0;

    //create GATEWAY
    BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy_s(gateway, len, src, len);

    std::uintptr_t addressToJump = src - gateway - 5;

    *(gateway + len) = 0xE9;

    *(std::uintptr_t*)((std::uintptr_t)gateway + len + 1) = addressToJump;

    if (!Detour32(src, dst, len)) return 0;

    return gateway;
}


DWORD WINAPI entry(HMODULE hModule)
{
    DWORD hkFunctionAddr = (DWORD)&hkSum;
    auto xd = (BYTE*)(GetModuleHandleA(nullptr) + 0x1000);
	//oSum = reinterpret_cast<tSum>(0x3B1000);
    oSum = reinterpret_cast<tSum>(trampHook32((BYTE*)0x00501000, (BYTE*)hkFunctionAddr, 11));
	//oSum(10, 10);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry, hModule, 0, nullptr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



/*
* 


*/