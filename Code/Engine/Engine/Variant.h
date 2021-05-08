#pragma once

#include <variant>

template<typename ...Types>
using Variant = std::variant<Types...>;