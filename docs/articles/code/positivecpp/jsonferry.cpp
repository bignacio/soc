#include <chrono>
#include <iostream>

#include <httplib.h>
#include <consumer.h>
#include <producer.h>

const constexpr auto topic = "ferry";

auto produce(cppkafka::Producer& producer, const httplib::Request& req) {
    const auto msg = req.body;
    auto builder = cppkafka::MessageBuilder(topic);
    builder.payload(msg);

    producer.produce(builder);
    std::cout << "Produced: " << msg << std::endl;
}

auto consume(cppkafka::Consumer& consumer, httplib::Response& res) {
    auto msg = consumer.poll();
    if (msg and not msg.get_error()) {
        const auto& payload = msg.get_payload();

        res.set_content(payload, "text/plain");
    }
}

auto startFerry(cppkafka::Consumer& consumer, cppkafka::Producer& producer) {
    httplib::Server server;

    server.Post("/produce", [&producer](const auto& req, auto& res) {
        produce(producer, req);
        res.set_content("OK", "text/plain");
    });

    consumer.subscribe({topic});
    server.Get("/consume", [&consumer](auto& req, auto& res) {
        consume(consumer, res);
    });

    std::cout << "Listening on port 8182" << std::endl;
    server.listen("127.0.0.1", 8182);
}

auto main() -> int {
    cppkafka::Configuration consumerConf = {
        {"metadata.broker.list", "127.0.0.1:19092"},
        {"group.id", "thegroupid"},
    };

    cppkafka::Configuration producerConf = {
        {"metadata.broker.list", "127.0.0.1:19092"},
    };

    cppkafka::Producer producer(producerConf);
    cppkafka::Consumer consumer(consumerConf);
    consumer.set_timeout(std::chrono::seconds{30});

    startFerry(consumer, producer);
    return 0;
}