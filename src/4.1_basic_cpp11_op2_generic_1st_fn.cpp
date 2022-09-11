#include <string>
#include <iostream>
#include <exception>
#include <type_traits>

//The generic implementation also takes care of the return type of Callable being different than T
//Here, parse_kafka_config(std::string&&) -> kafka_config
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

    kafka_consumer init_kafka() {
        //using namespace std::string_literals;
        //Can use "kafka-config-filename"s as they need C++14
        //std::string("kafka-config-filename") | get_env
        auto config = get_env("kafka-config-filename")
                        | get_file_contents
                        | parse_kafka_config;
        if (!config) { throw json_error{}; }

        auto consumer = create_kafka_consumer(std::move(config));
        if (!consumer) { throw creation_error{}; }

        if (!consumer.connect()) { throw connect_error{}; }
        if (!consumer.subscribe()) { throw subscribe_error{}; }
        
        return consumer;
    }
}

int main(int argc, char** argv) {
    auto consumer = init_kafka();
    if (consumer) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}