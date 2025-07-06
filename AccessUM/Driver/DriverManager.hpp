#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "../SilentDefs/Def.hpp"



namespace Api {
	bool attachprocess(DWORD pid);

	template <typename T>
	T read_mem(uintptr_t addr) {
		T buffer;
		SIZE_T bytesRead = 0;
		if (ReadProcessMemory(Globals::processhandle, reinterpret_cast<LPCVOID>(addr), &buffer, sizeof(T), &bytesRead) && bytesRead == sizeof(T)) {
			return buffer;
		}
		return T();
	}

	template <typename T>
	void write_mem(uintptr_t addr, const T& value) {
		SIZE_T bytesWritten = 0;
		WriteProcessMemory(Globals::processhandle, reinterpret_cast<LPVOID>(addr), &value, sizeof(T), &bytesWritten);
	}
}