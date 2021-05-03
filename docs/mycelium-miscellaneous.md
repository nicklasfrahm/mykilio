# Miscellaneous

This document contains drafts, ideas and thoughts that currently can't be assigned to any specific section of the documentation.

## High level resources

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
