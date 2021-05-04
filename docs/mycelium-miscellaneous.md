# Miscellaneous

This document contains drafts, ideas and thoughts that currently can't be assigned to any specific section of the documentation.

## Kubernetes CRDs

In the future, the components of **Mycelium** _could_ become a part of the Kubernetes API by using [Custom Resource Definitions](https://kubernetes.io/docs/tasks/extend-kubernetes/custom-resources/custom-resource-definitions/).

## High level resources

This section describes resources that could be used to provision baremetal via high level resources.

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

```yaml
apiVersion: baremetal.optrin.io/v1alpha1
kind: VirtualNetwork
metadata:
  name: rpi4b-net
  namespace: test
spec:
  mtu: 1000
```
