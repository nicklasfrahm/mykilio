# Core API

This API implements basic resources to provide the base for API aggregation, authentication and authorization.

The API is located at:

- **HTTPS:** `/apis/core.mykil.io/v1alpha1`
- **MQTTS:** `apis/core.mykil.io/v1alpha1`

## Resources

This sections outlines examples of the resources available as part of this API. This document only contains examples. For the actual JSON schema please refer to **THIS (TBD)**.

### Public key

A public key is used to authenticate things against the platform. They function similarly to the `authorized_keys` file for on SSH server.

```yaml
apiVersion: baremetal.mykil.io/v1alpha1
kind: PublicKey
metadata:
  name:
  namespace:
spec:
  signatureScheme:
    algorithm: ECDSA
    curve: secp521r1
    hash: SHA-512
  pem: |
    -----BEGIN PUBLIC KEY-----
    MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEpig8iz7fcL4bjXX+GnhmISIUEjmw
    vHPv5Pc1baGx8hbcFDdsoeA8cX+MXFv/VaD01vJ9H0WEAlAuq5RhYvSoHw==
    -----END PUBLIC KEY-----
```
