#pragma once

namespace core
{
	template<typename T> struct IsVariant : std::false_type {};

	template<typename ...Types>
	struct IsVariant<Variant<Types...>> : std::true_type {};

	template<class... Types> struct Overload : Types... { using Types::operator()...; };
	template<class... Types> Overload(Types...)->Overload<Types...>;

	template<class T, class U>
	struct VariantDefaults;

	template<class...Types, std::size_t... Indexes>
	struct VariantDefaults<Variant<Types...>, std::index_sequence<Indexes...>> 
	{
		using TVariant = Variant<Types...>;
		constexpr static TVariant variants[sizeof...(Types)]{ TVariant{std::in_place_index<Indexes>}... };
		// #todo: build display names as well
	};

	template<class Variant>
	struct VariantBuilder;

	template<class...Types>
	struct VariantBuilder<Variant<Types...>>
	{
		using indices = std::make_index_sequence<sizeof...(Types)>;
		using type = VariantDefaults<Variant<Types...>, indices>;
	};
}