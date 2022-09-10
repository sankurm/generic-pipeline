#include <string>
#include <string_view>
#include <optional>
#include <iostream>

namespace 
{
    std::optional<std::string> get_env(std::string_view varname) {
        return "/config/kafka.json";
    }

    std::optional<std::string> get_file_contents(std::string&& filename) {
        return "file-contents-blah-blah";
    }

    struct kafka_consumer
    {
        kafka_consumer(std::string&& config) {}
    };
    std::optional<kafka_consumer> create_kafka_consumer(std::string&& config) {
        return kafka_consumer(std::move(config));
    }
}

std::optional<kafka_consumer> init_kafka() {
    auto fname = get_env("kafka-config-filename");
    if (!fname) { return std::nullopt; }

    auto config = get_file_contents(std::move(fname.value()));
    if (!config) { return std::nullopt; }

    return create_kafka_consumer(std::move(config.value()));
}

int main(int argc, char** argv) {
    auto cons = init_kafka();
    if (cons) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}