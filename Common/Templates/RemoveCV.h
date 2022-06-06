#pragma once

namespace Common
{
	template<typename T>
	struct RemoveCV
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<volatile T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const volatile T>
	{
		using Type = T;
	};
}