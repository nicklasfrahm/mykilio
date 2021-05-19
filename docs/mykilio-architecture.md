# Architecture

- we have controllers

  - a microservice subscribing to the MQTT broker
  - every controller is also registered as a thing
  - manages state consistency

- we have things

  - each thing has one or more publicKeys

- state changes are published to special topic

## Paths

### `/apis`

```json
{
  "data": [
    {
      "name": "core.mykil.io/v1alpha1",
      "links": [{ "rel": "self", "href": "/apis/core.mykil.io/v1alpha1" }]
    },
    {
      "name": "baremetal.mykil.io/v1alpha1",
      "links": [{ "rel": "self", "href": "/apis/baremetal.mykil.io/v1alpha1" }]
    }
  ],
  "links": [{ "rel": "self", "href": "/apis" }]
}
```

### `/apis/core.mykil.io/v1alpha1`

```json
{
  "data": {
    "resources": [
      {
        "name": "Namespace",
        "links": [
          { "rel": "self", "href": "/apis/core.mykil.io/v1alpha1/namespaces" }
        ]
      }
    ]
  },
  "links": [{ "rel": "self", "href": "/apis/core.mykil.io/v1alpha1" }]
}
```

## Concepts

This section describes the general concepts and structures behind the API.

### API groups

Mykilio does not expose a single API, but rather aggregates several APIs in so-called API groups. This concept is well-established within the infrastructure management ecosystem and has found widespread adoption due to its successful implementation in Kubernetes. The advantages of structuring an API into API groups, where several versions of an API group can coexist, are outlined [in this section of the Kubernetes documentation][kubernetes_concepts_overview]). In short, the aggregation of APIs allows for better lifecycle management of APIs with gradual deprecation. Additionally it allows custom vendor-specific APIs to be developed and exposed via the same ecosystem.

### Namespaces

- Virtual clusters, zones or regions

### Protocol mapping

Based on which protocol is used, the API groups are mapped to the protocol's native mechanism of representing hierarchies. In the case of HTTP, API groups are mapped to URLs, such as `/apis/baremetal.optrin.io/v1alpha1`. Very similarly, the API groups are mapped to topics in MQTT, such as `apis/baremetal.optrin.io/v1alpha1`. Please note that there is no leading forward slash (`/`) in MQTT as this may cause confusion as described [here][mqtt_topics_best_practices]).

## Ecosystem

Let's build something cool together!

### Libraries

To reduce the barrier to adoption, the goal of _Mykilio_ is not only to define the APIs, but also to provide libraries to interact with the APIs. Currently the following languages are considered first class citizens:

- Go
- JavaScript
- C

[kubernetes_concepts_overview]: https://kubernetes.io/docs/concepts/overview/kubernetes-api/#api-groups-and-versioning
[mqtt_topics_best_practices]: https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/#best-practices
