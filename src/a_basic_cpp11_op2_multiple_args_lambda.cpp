#include <string>
#include <iostream>
#include <exception>
#include <type_traits>
#include <functional>
#include <fstream>

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
        if (/* varname not set OR value is empty */false) { throw env_error{}; }
        return "/config/kafka.json";
    }

    std::string get_file_contents(std::string&& filename) {
        std::ifstream file(filename, std::ios::in);
        if (!file && false) { throw file_error{}; }
        return "file-contents-blah-blah";
    }

    struct kafka_config
    {
        /* url etc. */
        operator bool() { return true; }
    };

    enum config_type { json, xml, yaml, config_map };

    template<config_type format>
    kafka_config parse_kafka_config(std::string&& config) {
        if (/* parsing fails == */ false) { throw json_error{}; }
        return kafka_config{};
    }

    struct certificate {};
    certificate get_certificate() { return certificate{}; }

    struct kafka_consumer
    {
        kafka_consumer(const kafka_config& config) {}
        kafka_consumer(kafka_config&& config) {}

        bool connect(const certificate&) { return true; }
        bool subscribe() { return true; }

        operator bool() { return true; }
    };

    kafka_consumer create_kafka_consumer(kafka_config&& config) {
        return kafka_consumer{std::move(config)};
    }

    kafka_consumer connect(kafka_consumer&& consumer, const certificate& cert) {
        if (!consumer.connect(cert)) { throw creation_error{}; }
        return consumer;
    }

    //Invoking an operation taking more than 1 argument
    //Can use a lambda - Need C++14 to be able to write this code though
    kafka_consumer init_kafka() {
        return get_env("kafka-config-filename")
                        | get_file_contents
                        | parse_kafka_config<xml>
                        | create_kafka_consumer
                        //| connect
                        | [cert = get_certificate()](kafka_consumer&& consumer) { return connect(std::move(consumer), cert); }
                        | [](kafka_consumer&& consumer) {
                            if (!consumer.subscribe()) { throw connect_error{}; }
                            return consumer;
                        };
    }
}

int main(int argc, char** argv) {
    auto consumer = init_kafka();
    if (consumer) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}