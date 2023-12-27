#include "Interface.hpp"

using namespace Aurie;

fs::path AurieConfigManager::GetConfigDirectory()
{
	AurieModule* initial_module = g_ArInitialImage;
	AurieStatus last_status = AURIE_SUCCESS;
	fs::path image_folder;
	
	// Get the game folder
	last_status = Internal::MdpGetImageFolder(
		initial_module,
		image_folder
	);

	// Make sure we got one
	if (!AurieSuccess(last_status))
		return {};

	// Craft the path to the configs
	fs::path config_directory = image_folder / L"mods" / L"Configs";
	
	try
	{
		if (!fs::exists(config_directory))
			fs::create_directory(config_directory);
	}
	catch (...)
	{
		return {};
	}

	return config_directory;
}

void AurieConfigManager::SaveToFile(
	IN nlohmann::json& JsonObject, 
	IN std::ofstream& FileStream
)
{
	FileStream << std::setw(4) << JsonObject << std::endl;
}

AurieStatus AurieConfigManager::Create()
{
	if (GetConfigDirectory().empty())
		return AURIE_ACCESS_DENIED;

	return AURIE_SUCCESS;
}

void AurieConfigManager::Destroy()
{
	// Save every config and clear the array, invalidating the pointers

	for (auto& element : m_ConfigEntries)
	{
		std::ofstream file_stream(element.m_FilePath);
		SaveToFile(element.m_JsonObject, file_stream);
	}
	
	m_ConfigEntries.clear();
}

void AurieConfigManager::QueryVersion(
	OUT short& Major, 
	OUT short& Minor, 
	OUT short& Patch
)
{
	Major = AURIE_CONFIGS_MAJOR;
	Minor = AURIE_CONFIGS_MINOR;
	Patch = AURIE_CONFIGS_PATCH;
}

AurieStatus AurieConfigManager::OpenConfig(
	IN std::wstring_view Filename, 
	OUT AurieConfig* Config
)
{
	fs::path image_path = GetConfigDirectory();

	// Make sure we have a path - if we don't, we can't open any configs
	if (image_path.empty())
		return AURIE_MODULE_INTERNAL_ERROR;

	// Construct the file path and the file stream for both reading and writing
	AurieConfigEntry entry;
	entry.m_FilePath = image_path / Filename;

	// If either throws an error, we assume it's access-related
	try
	{
		if (!fs::exists(entry.m_FilePath))
		{
			// Create an empty file
			std::ofstream stream(entry.m_FilePath);
			stream.close();
		}
	}
	catch (...)
	{
		return AURIE_ACCESS_DENIED;
	}

	// If we have access to the file, we can open it and load it
	std::ifstream file_stream(entry.m_FilePath);
	
	try
	{
		entry.m_JsonObject = nlohmann::json::parse(file_stream);
	}
	catch (...)
	{
		// Initialize empty JSON if the parsing failed and threw a fit (exception)
		entry.m_JsonObject = nlohmann::json();
	}
	
	*Config = &m_ConfigEntries.emplace_back(entry);

	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::CloseConfig(
	IN AurieConfig Config
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	
	std::ofstream file_stream(config_entry->m_FilePath);
	SaveToFile(config_entry->m_JsonObject, file_stream);
	
	std::erase(
		m_ConfigEntries,
		*config_entry
	);

	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::ReadInteger(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT int64_t& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// ReadInteger does not implement non-integer handling
	if (!config_entry->m_JsonObject.at(ValueName).is_number_integer())
		return AURIE_NOT_IMPLEMENTED;

	Value = config_entry->m_JsonObject.at(ValueName).get<int64_t>();
	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::ReadNumber(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT double& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// ReadNumber does not implement non-number handling
	if (!config_entry->m_JsonObject.at(ValueName).is_number())
		return AURIE_NOT_IMPLEMENTED;

	Value = config_entry->m_JsonObject.at(ValueName).get<double>();
	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::ReadString(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT std::string& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// ReadString does not implement non-string handling
	if (!config_entry->m_JsonObject.at(ValueName).is_string())
		return AURIE_NOT_IMPLEMENTED;

	Value = config_entry->m_JsonObject.at(ValueName).get<std::string>();
	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::ReadIntegerArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT std::vector<int64_t>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// We hope that the array contains strings and only strings
	if (!config_entry->m_JsonObject.at(ValueName).is_array())
		return AURIE_NOT_IMPLEMENTED;

	try
	{
		Value = config_entry->m_JsonObject.at(ValueName).get<std::vector<int64_t>>();
	}
	catch (...)
	{
		return AURIE_NOT_IMPLEMENTED;
	}

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::ReadNumberArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT std::vector<double>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// We hope that the array contains strings and only strings
	if (!config_entry->m_JsonObject.at(ValueName).is_array())
		return AURIE_NOT_IMPLEMENTED;

	try
	{
		Value = config_entry->m_JsonObject.at(ValueName).get<std::vector<double>>();
	}
	catch (...)
	{
		return AURIE_NOT_IMPLEMENTED;
	}

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::ReadStringArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	OUT std::vector<std::string>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// We hope that the array contains strings and only strings
	if (!config_entry->m_JsonObject.at(ValueName).is_array())
		return AURIE_NOT_IMPLEMENTED;

	try
	{
		Value = config_entry->m_JsonObject.at(ValueName).get<std::vector<std::string>>();
	}
	catch (...)
	{
		return AURIE_NOT_IMPLEMENTED;
	}

	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::WriteInteger(
	IN AurieConfig Config,
	IN std::string_view ValueName,
	IN const int64_t& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteNumber(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN const double& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

AurieStatus AurieConfigManager::WriteString(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN std::string_view Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteIntegerArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN const std::vector<int64_t>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteNumberArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN const std::vector<double>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteStringArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN const std::vector<std::string>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::ReadBoolean(
	IN AurieConfig Config, 
	IN std::string_view ValueName, 
	OUT bool& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// ReadBoolean does not implement non-boolean handling
	if (!config_entry->m_JsonObject.at(ValueName).is_boolean())
		return AURIE_NOT_IMPLEMENTED;

	Value = config_entry->m_JsonObject.at(ValueName).get<bool>();
	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::ReadBooleanArray(
	IN AurieConfig Config, 
	IN std::string_view ValueName, 
	OUT std::vector<bool>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);

	if (!config_entry->m_JsonObject.contains(ValueName))
		return AURIE_OBJECT_NOT_FOUND;

	// We hope that the array contains strings and only strings
	if (!config_entry->m_JsonObject.at(ValueName).is_array())
		return AURIE_NOT_IMPLEMENTED;

	try
	{
		Value = config_entry->m_JsonObject.at(ValueName).get<std::vector<bool>>();
	}
	catch (...)
	{
		return AURIE_NOT_IMPLEMENTED;
	}

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteBoolean(
	IN AurieConfig Config, 
	IN std::string_view ValueName, 
	IN const bool& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}

Aurie::AurieStatus AurieConfigManager::WriteBooleanArray(
	IN AurieConfig Config,
	IN std::string_view ValueName, 
	IN const std::vector<std::string>& Value
)
{
	if (!Config)
		return AURIE_INVALID_PARAMETER;

	AurieConfigEntry* config_entry = reinterpret_cast<AurieConfigEntry*>(Config);
	config_entry->m_JsonObject[ValueName] = Value;

	return AURIE_SUCCESS;
}
