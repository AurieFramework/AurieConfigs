#pragma once
#include <Aurie/shared.hpp>

using AurieConfig = PVOID;

#ifndef AURIE_CONFIGS_MAJOR
#define AURIE_CONFIGS_MAJOR 1
#endif // AURIE_CONFIGS_MAJOR

#ifndef AURIE_CONFIGS_MINOR
#define AURIE_CONFIGS_MINOR 1
#endif // AURIE_CONFIGS_MINOR

#ifndef AURIE_CONFIGS_PATCH
#define AURIE_CONFIGS_PATCH 0
#endif // AURIE_CONFIGS_PATCH

struct AurieConfigInterface : public Aurie::AurieInterfaceBase
{
	virtual Aurie::AurieStatus OpenConfig(
		IN std::wstring_view Filename,
		OUT AurieConfig* Config
	) = 0;

	virtual Aurie::AurieStatus CloseConfig(
		IN AurieConfig Config
	) = 0;

	virtual Aurie::AurieStatus ReadInteger(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT int64_t& Value
	) = 0;

	virtual Aurie::AurieStatus ReadNumber(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT double& Value
	) = 0;

	virtual Aurie::AurieStatus ReadString(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::string& Value
	) = 0;

	virtual Aurie::AurieStatus ReadIntegerArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<int64_t>& Value
	) = 0;

	virtual Aurie::AurieStatus ReadNumberArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<double>& Value
	) = 0;

	virtual Aurie::AurieStatus ReadStringArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<std::string>& Value
	) = 0;

	virtual Aurie::AurieStatus WriteInteger(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const int64_t& Value
	) = 0;

	virtual Aurie::AurieStatus WriteNumber(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const double& Value
	) = 0;

	virtual Aurie::AurieStatus WriteString(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN std::string_view Value
	) = 0;

	virtual Aurie::AurieStatus WriteIntegerArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<int64_t>& Value
	) = 0;

	virtual Aurie::AurieStatus WriteNumberArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<double>& Value
	) = 0;

	virtual Aurie::AurieStatus WriteStringArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<std::string>& Value
	) = 0;

	virtual Aurie::AurieStatus ReadBoolean(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT bool& Value
	) = 0;

	virtual Aurie::AurieStatus ReadBooleanArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		OUT std::vector<bool>& Value
	) = 0;

	virtual Aurie::AurieStatus WriteBoolean(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const bool& Value
	) = 0;

	virtual Aurie::AurieStatus WriteBooleanArray(
		IN AurieConfig Config,
		IN std::string_view ValueName,
		IN const std::vector<std::string>& Value
	) = 0;
};