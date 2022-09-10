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

    struct kafka_config { /* url etc. */ };

    std::optional<kafka_config> parse_kafka_config(std::string&& config) {
        return std::make_optional(kafka_config{});
    }

    struct kafka_consumer
    {
        kafka_consumer(const kafka_config& config) {}
        kafka_consumer(kafka_config&& config) {}

        bool connect() { return true; }
        bool subscribe() { return true; }
    };

    std::optional<kafka_consumer> create_kafka_consumer(kafka_config&& config) {
        return std::make_optional<kafka_consumer>(std::move(config));
    }

    std::optional<kafka_consumer> connect(kafka_consumer&& consumer) {
        return consumer.connect()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> subscribe(kafka_consumer&& consumer) {
        return consumer.subscribe()? std::make_optional(consumer): std::nullopt;
    }

    std::optional<kafka_consumer> init_kafka() {
        auto fname = get_env("kafka-config-filename");
        if (!fname) { return std::nullopt; }

        auto contents = get_file_contents(std::move(fname.value()));
        if (!contents) { return std::nullopt; }

        auto config = parse_kafka_config(std::move(contents.value()));
        if (!config) { return std::nullopt; }

        auto cons = create_kafka_consumer(std::move(config.value()));
        if (!cons) { return std::nullopt; }

        auto consumer = cons.value();
        if (consumer.connect() && consumer.subscribe()) {
            return cons;
        }
        return std::nullopt;
    }
}

int main(int argc, char** argv) {
    auto cons = init_kafka();
    if (cons) { std::cout << "Consumer creation successful\n"; }
    else { std::cout << "Consumer creation failed\n"; }

    return 0;
}