# Mycelium

Mycelium aims to provide an eventually consistent, declarative way of managing baremetal servers.

## API concepts

This section describes the general concepts and structures behind the API. **Mycelium** does not expose a single API, but rather aggregates several APIs in so-called API groups. This concept is well-established within the cluster management ecosystem and has found widespread adoption due to its successful implementation in Kubernetes. The advantages of structuring an API into API groups where several versions of an API group can coexist is outlined [in this section of the Kubernetes documentation][kubernetes_concepts_overview]).

Based on which protocol is used, the API groups are mapped to the protocol's native mechanism of representing hierarchies. In the case of HTTP, API groups are mapped to URLs, such as `/apis/baremetal.optrin.io/v1alpha1`. Very similarly, the API groups are mapped to topics in MQTT, such as `apis/baremetal.optrin.io/v1alpha1`. Please note that there is no leading forward slash (`/`) in MQTT as this may cause confusion as described [here][mqtt_topics_best_practices]).

## Declarative and eventually consistent

TBD

## Language support

To reduce the barrier to adoption, the goal of **Mycelium** is not only to define the APIs, but also to provide libraries to interact with the APIs. Currently the following languages are considered first class citizens:

- C / C++
- Go
- JavaScript

<!-- Glossary -->

[kubernetes_concepts_overview]: https://kubernetes.io/docs/concepts/overview/kubernetes-api/#api-groups-and-versioning
[mqtt_topics_best_practices]: https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/#best-practices
