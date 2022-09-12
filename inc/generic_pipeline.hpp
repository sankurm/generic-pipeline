#ifndef GENERIC_PIPELINE_HPP
#define GENERIC_PIPELINE_HPP

#include <functional>
#include <type_traits>
#include <optional>
#include <concepts>
#include <ranges>

template<typename T>
concept basic_optional = requires (T t) {
    typename T::value_type;
    std::convertible_to<T, bool>;
    std::same_as<std::remove_cvref<decltype(*t)>, typename T::value_type>;
    std::constructible_from<T, std::nullopt_t>;
};
static_assert(basic_optional<std::optional<int>>);

template<typename T, typename Callable>
requires (not std::ranges::range<T> && std::invocable<Callable, T>)
auto operator|(T&& val, Callable&& fn) -> typename std::invoke_result_t<Callable, T> {
    return std::invoke(std::forward<Callable>(fn), std::forward<T>(val));
}

//These overloads handle std::optional<T>::and_then case
template<typename T, typename Callable>
requires std::invocable<Callable, T> && basic_optional<typename std::invoke_result_t<Callable, T>>
auto operator|(std::optional<T>&& opt, Callable&& fn) -> typename std::invoke_result_t<Callable, T> {
    return opt? std::invoke(std::forward<Callable>(fn), *std::move(opt)): std::nullopt;
}
template<typename T, typename Callable>
requires std::invocable<Callable, T> && basic_optional<typename std::invoke_result_t<Callable, T>>
auto operator|(const std::optional<T>& opt, Callable&& fn) -> typename std::invoke_result_t<Callable, T> {
    return opt? std::invoke(std::forward<Callable>(fn), *opt): std::nullopt;
}

//These overloads handle std::optional<T>::transform case
template<typename T, typename Callable>
requires std::invocable<Callable, T> && (!basic_optional<typename std::invoke_result_t<Callable, T>>)
auto operator|(std::optional<T>&& opt, Callable&& fn) -> std::optional<typename std::invoke_result_t<Callable, T>> {
    return opt? std::make_optional(std::invoke(std::forward<Callable>(fn), *std::move(opt))): std::nullopt;
}
template<typename T, typename Callable>
requires std::invocable<Callable, T> && (!basic_optional<typename std::invoke_result_t<Callable, T>>)
auto operator|(const std::optional<T>& opt, Callable&& fn) -> std::optional<typename std::invoke_result_t<Callable, T>> {
    return opt? std::make_optional(std::invoke(std::forward<Callable>(fn), *opt)): std::nullopt;
}

//These overloads handle std::optional<T>::or_else case
template<typename T, typename Callable>
requires (!std::invocable<Callable, T>) && std::invocable<Callable> && std::same_as<typename std::invoke_result_t<Callable>, std::optional<T>>
auto operator|(std::optional<T>&& opt, Callable&& fn) -> typename std::invoke_result_t<Callable> {
    return opt? *opt: std::invoke(std::forward<Callable>(fn));
}
template<typename T, typename Callable>
requires (!std::invocable<Callable, T>) && std::invocable<Callable> && std::same_as<typename std::invoke_result_t<Callable>, std::optional<T>>
auto operator|(const std::optional<T>& opt, Callable&& fn) -> typename std::invoke_result_t<Callable> {
    return opt? *opt: std::invoke(std::forward<Callable>(fn));
}

#endif  //GENERIC_PIPELINE_HPP