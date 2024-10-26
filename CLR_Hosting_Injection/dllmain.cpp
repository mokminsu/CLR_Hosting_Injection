#include "pch.h"
#include "dllmain.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void NETInjection(const char* assembly, const char* typeName, const char* method, const char* argument, DWORD* returnValue)
{
	ICLRMetaHost* pMetaHost = nullptr;
	ICLRRuntimeInfo* pRuntimeInfo = nullptr;
	ICLRRuntimeHost* pRuntimeHost = nullptr;
	HRESULT hr;

	wchar_t wAssembly[256], wTypeName[256], wMethod[256], wArgument[256];
	MultiByteToWideChar(CP_ACP, 0, assembly, -1, wAssembly, 256);
	MultiByteToWideChar(CP_ACP, 0, typeName, -1, wTypeName, 256);
	MultiByteToWideChar(CP_ACP, 0, method, -1, wMethod, 256);
	MultiByteToWideChar(CP_ACP, 0, argument, -1, wArgument, 256);

	hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pMetaHost);
	hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo);
	hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&pRuntimeHost);
	
	hr = pRuntimeHost->Start();

	hr = pRuntimeHost->ExecuteInDefaultAppDomain(wAssembly, wTypeName, wMethod, wArgument, returnValue);

	if (pRuntimeHost) pRuntimeHost->Release();
	if (pRuntimeInfo) pRuntimeInfo->Release();
	if (pMetaHost) pMetaHost->Release();
}