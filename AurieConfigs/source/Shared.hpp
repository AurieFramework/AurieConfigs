#pragma once
#include <Aurie/shared.hpp>

using AurieConfig = PVOID;

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
};