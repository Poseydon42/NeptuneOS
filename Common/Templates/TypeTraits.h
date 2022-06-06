#pragma once

#include <Templates/RemoveCV.h>

namespace Common
{

	template<typename T>
	struct IsIntegralImpl { static constexpr bool Value = false; };

	template<>
	struct IsIntegralImpl<bool> { static constexpr bool Value = true; };

	template<>
	struct IsIntegralImpl<char> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<short> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<int> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<long> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<long long> { static constexpr bool Value = true; };

	template<>
	struct IsIntegralImpl<unsigned char> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<unsigned short> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<unsigned int> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<unsigned long> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<unsigned long long> { static constexpr bool Value = true; };

	template<>
	struct IsIntegralImpl<char16_t> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<char32_t> { static constexpr bool Value = true; };
	template<>
	struct IsIntegralImpl<wchar_t> { static constexpr bool Value = true; };

	template<typename T>
	inline constexpr bool IsIntegral = IsIntegralImpl<typename RemoveCV<T>::Type>::Value;
}