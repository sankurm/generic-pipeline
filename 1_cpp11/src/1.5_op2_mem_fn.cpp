#include <string>
#include <iostream>
#include <exception>
#include <type_traits>
#include <functional>

//The generic implementation also takes care of the return type of Callable being different than T
template<typename T, typename Callable>
auto operator|(T&& val, Callable&& fn) -> typename std::result_of<Callable(T)>::type {
    return std::forward<Callable>(fn)(std::forward<T>(val));
}

//Pre-C++17 code without std::optional
//Code relies on special values like empty string, kafka_config to be convertible to bool and return bools to determine success of a step
namespace 
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
        return "file-contents-blah-blah";
    }

    struct kafka_config
    {
        /* url etc. */
        operator bool() { return true; }
    };

    kafka_config parse_kafka_config(std::string&& config) {
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
        return kafka_consumer{std::move(config)};
    }

    kafka_consumer connect(kafka_consumer&& consumer) {
        consumer.connect();
        return consumer;
    }

    kafka_consumer init_kafka() {
        return get_env("kafka-config-filename")
                        | get_file_contents
                        | parse_kafka_config
                        | create_kafka_consumer
                        | connect
                        //| std::mem_fn(&kafka_consumer::subscribe);  //mem_fn possible only if the member returns what the next step or return type needs to be
                        | [](kafka_consumer&& consumer) { consumer.subscribe(); return consumer; };
    }
}

int main(int argc, char** argv) {
    auto consumer = init_kafka();
    if (consumer) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}