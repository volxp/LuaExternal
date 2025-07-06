#include "DriverManager.hpp"



bool Api::attachprocess(DWORD pid) {
	Globals::processhandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);

	if (!Globals::processhandle)
		return false;

	return true;
}

