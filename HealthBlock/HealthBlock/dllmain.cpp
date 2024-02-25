// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <windows.h>


bool Hook(void* enderecoHook, void* nossaFunc, int len) {
    if (len >= 5) {
        DWORD protecao;
        VirtualProtect(enderecoHook, len, PAGE_EXECUTE_READWRITE, &protecao);
        DWORD enderecoRelativo = ((DWORD)nossaFunc - (DWORD)enderecoHook) - 5;
        memset(enderecoHook, 0x90, len); //0x90 -> NOP
        *(BYTE*)enderecoHook = 0xE9; //0xE9 opcode da instruçao JMP]
        *(DWORD*)((DWORD)enderecoHook + 1) = enderecoRelativo;
        DWORD temp;
        VirtualProtect(enderecoHook, len, protecao, &temp);

        return true;

    }
    else
    {
        return false;
    }
}

DWORD jmpBack;
_declspec(naked) void nossaFunc()
{
    _asm
    {   add eax, 0x9
        jmp[jmpBack]
    }

}

DWORD WINAPI MainThread(LPVOID param) {
    int hookLenght = 6;
    DWORD enderecoHook = 0x00007FF71F782342;
    jmpBack = enderecoHook + hookLenght;

    if (Hook((void*)enderecoHook, nossaFunc, hookLenght)){
        MessageBox(0, L"Hook Efetuada!", L"Sucesso", 0);
    }
    else
    {
        MessageBox(0, L"Nao foi possivel efetuar a Hook", L"Ops...", 0);
    }
    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;

        }
        Sleep(40);
    }
    MessageBox(0, L"Unjecting", L"Ig", 0);
    FreeLibraryAndExitThread((HMODULE)param, 0);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    }
    return TRUE;
}


