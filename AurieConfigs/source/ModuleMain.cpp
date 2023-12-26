#include "Interface/Interface.hpp"
using namespace Aurie;

static AurieConfigManager g_ConfigManager;

EXPORTED AurieStatus ModulePreinitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus last_status = AURIE_SUCCESS;

	last_status = ObCreateInterface(
		Module,
		&g_ConfigManager,
		"Aurie Config Manager"
	);

	return last_status;
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(Module);
	UNREFERENCED_PARAMETER(ModulePath);

	return AURIE_SUCCESS;
}