# The Mycelium API

This document describes the general concepts and structures behind the API. **Mycelium** does not expose a single API, but rather aggregates several APIs in so-called API groups. This concept is well-established within the cluster management ecosystem and has found widespread adoption due to its successful implementation in Kubernetes. The advantages of structuring an API into API groups where several versions of an API group can coexist is outlined [in this section of the Kubernetes documentation](https://kubernetes.io/docs/concepts/overview/kubernetes-api/#api-groups-and-versioning).

**Note:** In the future, the components of **Mycelium** could become a part of the Kubernetes API by using [Custom Resource Definitions](https://kubernetes.io/docs/tasks/extend-kubernetes/custom-resources/custom-resource-definitions/).

Based on which protocol is used, the API groups are mapped to the native mechanism of representing hierarchies. In the case of HTTP API groups are mapped to URL, such as `/apis/baremetal.optrin.io/v1alpha1`. Very similarly, the API groups are mapped to topics in MQTT, such as `apis/baremetal.optrin.io/v1alpha1`. Please note that there is no leading forward slash (`/`) in MQTT as this may cause confusion as described [here](https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/#best-practices).

## Draft resources

This section describes resources that are required for day one operations.

```yaml
apiVersion: baremetal.optrin.io/v1alpha1
kind: Plan
metadata:
  name: rpi-4b
spec:
  # TODO: Evaluate overlap with Tinkerbell.
```

```yaml
apiVersion: baremetal.optrin.io/v1alpha1
kind: Worker
metadata:
  name: rpi4b-0
  namespace: test
spec:
  hardware:
    sku: rpi-4b
  software:
    os:
      codename: ubuntu-focal
```

## Roadmap

This section describes resources that could extend the API in a meaningful way in the future.

```yaml
apiVersion: baremetal.optrin.io/v1alpha1
kind: VirtualNetwork
metadata:
  name: rpi4b-net
  namespace: test
spec:
  mtu: 1000
```
