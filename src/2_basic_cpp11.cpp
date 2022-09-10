#include <string>
#include <iostream>
#include <exception>

namespace 
{
    struct env_error : public std::exception {};
    struct file_error : public std::exception {};
    struct json_error : public std::exception {};
    struct creation_error : public std::exception {};
    struct connect_error : public std::exception {};
    struct subscribe_error : public std::exception {};

    std::string get_env(std::string_view varname) {
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
        std::string fname = get_env("kafka-config-filename");
        if (fname.empty()) { throw env_error{}; }

        auto contents = get_file_contents(std::move(fname));
        if (contents.empty()) { throw file_error{}; }

        auto config = parse_kafka_config(std::move(contents));
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