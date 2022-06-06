#pragma once

namespace Common
{

	template<bool Predicate, typename T = void>
	struct EnableIfImpl {};

	template<typename T>
	struct EnableIfImpl<true, T>
	{
		using Type = T;
	};

	template<bool Predicate, typename T>
	using EnableIf = typename EnableIfImpl<Predicate, T>::Type;
}