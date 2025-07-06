#pragma once
#include "../SilentDefs/Def.hpp"
#include "../Driver/DriverManager.hpp"
#include <iostream>
#include "Compressing.h"
#include "../SilentDefs/Offsets.hpp"

using namespace Globals;

namespace ExecutionService {
	inline void Execute(std::string Src){

		auto ScriptContainer = SInstance::WaitForChild(SInstance::GetChildren(CoreGui), "CoreModule", 5);
		if (!ScriptContainer)
			return;


		auto HolderValue = SInstance::ObjectValue(ScriptContainer);
		if (!HolderValue)
			return;


		SInstance::SetBytecode(HolderValue, LuauCompress::ZstdCompress(LuauCompress::Compile("return {['someExecution'] = function(...)\n" + Src + "\nend}")));
		Roblox::CoreBypass(HolderValue);
	}

	
}