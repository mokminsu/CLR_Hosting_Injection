#pragma once

extern "C" __declspec(dllexport) void NETInjection(const char* assembly, const char* typeName, const char* method, const char* argument, DWORD* returnValue);