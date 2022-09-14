#include <string>
#include <string_view>
#include <optional>
#include <iostream>
#include <functional>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <fstream>

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
requires std::invocable<Callable, T> && basic_optional<typename std::invoke_result_t<Callable, const T&>>
auto operator|(const std::optional<T>& opt, Callable&& fn) -> typename std::invoke_result_t<Callable, const T&> {
    return opt? std::invoke(std::forward<Callable>(fn), *opt): std::nullopt;
}

//These overloads handle std::optional<T>::transform case
template<typename T, typename Callable>
requires std::invocable<Callable, T> && (!basic_optional<typename std::invoke_result_t<Callable, T>>)
auto operator|(std::optional<T>&& opt, Callable&& fn) -> std::optional<typename std::invoke_result_t<Callable, T>> {
    return opt? std::make_optional(std::invoke(std::forward<Callable>(fn), *std::move(opt))): std::nullopt;
}
template<typename T, typename Callable>
requires std::invocable<Callable, T> && (!basic_optional<typename std::invoke_result_t<Callable, const T&>>)
auto operator|(const std::optional<T>& opt, Callable&& fn) -> std::optional<typename std::invoke_result_t<Callable, const T&>> {
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

//C++17 - disciplined code using std::optional
namespace 
{
    std::optional<std::string> get_env(std::string_view varname) {
        return "/config/kafka.json";
    }

    /*std::string get_file_contents(std::string&& filename) {
        return "file-contents-blah-blah";
    }*/
    std::optional<std::string> get_file_contents(std::string&& filename) {
        std::ifstream file(filename, std::ios::in);
        if (!file) { return std::nullopt; }
        return "file-contents-blah-blah";
    }
    std::optional<std::string> get_default_config() {
        return "default-contents";
    }

    struct kafka_config { /* url etc. */ };

    /*std::optional<kafka_config> parse_kafka_config(std::string&& config) {
        return std::make_optional(kafka_config{});
    }*/
    kafka_config parse_kafka_config(std::string&& config) {
        return kafka_config{};
    }

    struct kafka_consumer
    {
        kafka_consumer(const kafka_config& config) {}
        kafka_consumer(kafka_config&& config) {}

        bool connect() const { return true; }
        bool subscribe() { return true; }
    };

    /*std::optional<kafka_consumer> create_kafka_consumer(kafka_config&& config) {
        return std::make_optional<kafka_consumer>(std::move(config));
    }*/
    kafka_consumer create_kafka_consumer(const kafka_config& config) {
        return kafka_consumer(config);
    }

    std::optional<kafka_consumer> connect(const kafka_consumer& consumer) {
        return consumer.connect()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> subscribe(kafka_consumer&& consumer) {
        return consumer.subscribe()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> init_kafka() {
        return get_env("kafka-config-filename")
                | get_file_contents | get_default_config
                | parse_kafka_config
                | create_kafka_consumer
                | connect
                | subscribe;
    }
}

int main(int argc, char** argv) {
    auto cons = init_kafka();
    if (cons) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    std::vector ints{1, 3, 5, 7, 9};
    auto pipeline = ints | std::views::transform([](int n) { return n*n; });
    for (auto n : pipeline) { std::cout << n << ' '; }
    std::cout << '\n';

    [[maybe_unused]] auto dummy = get_default_config();
    return 0;
}