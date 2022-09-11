#include <string>
#include <iostream>
#include <exception>
#include <type_traits>
#include <functional>

namespace framework
{
    //The generic implementation cannot be put in a different namespace as it will not be considered for the parameters
    //Global operator is a must - and it is a potential disrupter for types that have their own overload of operator|
    template<typename T, typename Callable>
    auto operator|(T&& val, Callable&& fn) -> typename std::result_of<Callable(T)>::type {
        return std::forward<Callable>(fn)(std::forward<T>(val));
    }
}

//Pre-C++17 code without std::optional
//Code relies on special values like empty string, kafka_config to be convertible to bool and return bools to determine success of a step
namespace app
{
    struct env_error : public std::exception {};
    struct file_error : public std::exception {};
    struct json_error : public std::exception {};
    struct creation_error : public std::exception {};
    struct connect_error : public std::exception {};
    struct subscribe_error : public std::exception {};

    std::string get_env(std::string&& varname) {
        return "/config/kafka.json";
    }

    std::string get_file_contents(std::string&& filename) {
        if (filename.empty()) { throw env_error{}; }
        return "file-contents-blah-blah";
    }

    struct kafka_config
    {
        /* url etc. */
        operator bool() { return true; }
    };

    kafka_config parse_kafka_config(std::string&& config) {
        if (config.empty()) { throw file_error{}; }
        return kafka_config{};
    }

    struct kafka_consumer
    {
        kafka_consumer(const kafka_config& config) {}
        kafka_consumer(kafka_config&& config) {}

        bool connect() { return true; }
        bool subscribe() { return true; }

        operator bool() { return true; }
    };

    kafka_consumer create_kafka_consumer(kafka_config&& config) {
        if (!config) { throw json_error{}; }
        return kafka_consumer{std::move(config)};
    }

    kafka_consumer connect(kafka_consumer&& consumer) {
        if (!consumer) { throw connect_error{}; }
        consumer.connect();
        return consumer;
    }
}

namespace framework
{
    //All functions check before use
    //This is the wrong-way round - each function is assuming which is the previous function to throw the correct exception
    app::kafka_consumer init_kafka() {
        return app::get_env("kafka-config-filename")
                        | app::get_file_contents
                        | app::parse_kafka_config
                        | app::create_kafka_consumer
                        | app::connect
                        //| std::mem_fn(&kafka_consumer::subscribe);  //mem_fn possible only if the member returns what the next step or return type needs to be
                        | [](app::kafka_consumer&& consumer) {
                            if (!consumer) { throw app::connect_error{}; }
                            consumer.subscribe();
                            return consumer;
                        };
    }
}

int main(int argc, char** argv) {
    auto consumer = framework::init_kafka();
    if (consumer) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}