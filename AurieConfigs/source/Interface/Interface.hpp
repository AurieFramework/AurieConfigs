#pragma once
#include "../Shared.hpp"
#include <list>
#include <fstream>
#include <nlohmann/json.hpp>

struct AurieConfigEntry
{
	std::filesystem::path m_FilePath;
	nlohmann::json m_JsonObject;

	bool operator==(const AurieConfigEntry& Other) const
	{
		return m_FilePath == Other.m_FilePath;
	}
};

struct AurieConfigManager : public AurieConfigInterface
{
private:
	std::list<AurieConfigEntry> m_ConfigEntries;
	std::filesystem::path GetConfigDirectory();

	void SaveToFile(
		IN nlohmann::json& JsonObject,
		IN std::ofstream& FileStream
	);

public:
	virtual Aurie::AurieStatus Create();

	virtual void Destroy();

	virtual void QueryVersion(
		OUT short& Major,
		OUT short& Minor,
		OUT short& Patch
	);

	virtual Aurie::AurieStatus OpenConfig(
		IN std::wstring_view Filename,
		OUT AurieConfig* Config
	) override final;

	virtual Aurie::AurieStatus CloseConfig(
		IN AurieConfig Config
	) override final;

	virtual Aurie::AurieStatus ReadInteger(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT int64_t& Value
	) override final;

	virtual Aurie::AurieStatus ReadNumber(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT double& Value
	) override final;

	virtual Aurie::AurieStatus ReadString(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::string& Value
	) override final;

	virtual Aurie::AurieStatus ReadIntegerArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<int64_t>& Value
	) override final;

	virtual Aurie::AurieStatus ReadNumberArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<double>& Value
	) override final;

	virtual Aurie::AurieStatus ReadStringArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<std::string>& Value
	) override final;

	virtual Aurie::AurieStatus WriteInteger(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const int64_t& Value
	) override final;

	virtual Aurie::AurieStatus WriteNumber(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const double& Value
	) override final;

	virtual Aurie::AurieStatus WriteString(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN std::string_view Value
	) override final;

	virtual Aurie::AurieStatus WriteIntegerArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<int64_t>& Value
	) override final;

	virtual Aurie::AurieStatus WriteNumberArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<double>& Value
	) override final;

	virtual Aurie::AurieStatus WriteStringArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<std::string>& Value
	) override final;
};