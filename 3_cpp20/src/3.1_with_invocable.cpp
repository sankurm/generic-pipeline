#include <string>
#include <string_view>
#include <optional>
#include <iostream>
#include <functional>
#include <concepts>

template<typename T, typename Callable>
requires std::invocable<Callable, T>
auto operator|(T&& val, Callable&& fn) -> typename std::invoke_result_t<Callable, T> {
    return std::invoke(std::forward<Callable>(fn), std::forward<T>(val));
}

//Handle unwrapping of std::optional
template<typename T, typename Callable>
requires std::invocable<Callable, T>
auto operator|(std::optional<T>&& opt, Callable&& fn) -> typename std::invoke_result_t<Callable, T> {
    return opt? std::invoke(std::forward<Callable>(fn), *std::move(opt)): std::nullopt;
}
template<typename T, typename Callable>
requires std::invocable<Callable, const T&>
auto operator|(const std::optional<T>& opt, Callable&& fn) -> typename std::invoke_result_t<Callable, const T&> {
    return opt? std::invoke(std::forward<Callable>(fn), *opt): std::nullopt;
}

//C++17 - disciplined code using std::optional
namespace 
{
    std::optional<std::string> get_env(std::string_view varname) {
        return "/config/kafka.json";
    }

    std::optional<std::string> get_file_contents(std::string&& filename) {
        return "file-contents-blah-blah";
    }

    struct kafka_config { /* url etc. */ };

    std::optional<kafka_config> parse_kafka_config(std::string&& config) {
        return std::make_optional(kafka_config{});
    }

    struct kafka_consumer
    {
        kafka_consumer(const kafka_config& config) {}
        kafka_consumer(kafka_config&& config) {}

        bool connect() const { return true; }
        bool subscribe() { return true; }
    };

    std::optional<kafka_consumer> create_kafka_consumer(kafka_config&& config) {
        return std::make_optional<kafka_consumer>(std::move(config));
    }
    /*kafka_consumer create_kafka_consumer(const kafka_config& config) {
        return kafka_consumer(config);
    }*/

    std::optional<kafka_consumer> connect(const kafka_consumer& consumer) {
        return consumer.connect()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> subscribe(kafka_consumer&& consumer) {
        return consumer.subscribe()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> init_kafka() {
        using namespace std::string_literals;
        auto opt_consumer = "kafka-config-filename"s
                | get_env
                | get_file_contents
                | parse_kafka_config
                | create_kafka_consumer;
        return opt_consumer
                | connect
                | subscribe;
    }
}

int main(int argc, char** argv) {
    auto cons = init_kafka();
    if (cons) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}