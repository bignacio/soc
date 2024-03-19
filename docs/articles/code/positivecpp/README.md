# Example REST service and Kafka producer/consumer in C++

This is an example (not production grade!!) of how building a REST service that can publish to and consume messages from Kafka.
All in C++

## Setup and build

Install [cppkafka as instructed in the github page](https://github.com/mfontanini/cppkafka).

Checkout/downloads all filres in this directory. Then, build and run with

```bash
cmake -B build
make -C build

build/jsonferry
```

There is a docker-compose.yaml file that starts up Redpanda (a dev friendly Kafka compatible service.).

start it up with
```bash
docker compose up -d
```

(you can shut it down after with `docker compose down`)

Then you can produce a message to Kafka by POSTing to the `produce` endpoint

```bash
curl localhost:8182/produce -d'hello there'
```

and consume a message GETing from the `consume` endpoint

```bash
curl localhost:8182/consume
```
