# Mykilio

_Mykilio_ is the proposal for a new Living Standard with the goal to reimagine infrastructure management and monitoring. It aims to be scalable, lightweight, extensible and secure by applying IoT principles to the datacenter and homelabs. Additionally, the goal is to provide a full ecosystem for the development of vendor-agnostic interfaces for infrastructure management and automation.

Historically, computing, network and storage infrastructure components are often perceived as highly specialized hardware, that were designed to fulfill the ever-increasing performance, bandwidth and capacity requirements of modern applications. At their core however, an infrastructure component is _just_ another physical object connected to the internet. This becomes even more apparent, when considering a variety of definitions of the _Internet of Things_ created by companies and research papers, which can be summarized as shown below:

<!-- prettier-ignore -->
!!! quote "Internet of Things"
    The _Internet of Things_ is the idea of connecting any physical object to the internet and thereby also to other connected physical objects. This allows to gather enormous amounts of previously unavailable data that can be utilized to develop new services and create additional value.

The objective of _Mykilio_ is to bridge the gap between the _Internet of Things_ and modern infrastructure management.

## Principles

_Mykilio_ follows the guiding principles outlined in this section, which are inspired by the popular ideas of Kubernetes and Redfish. The principles serve as the foundation of _Mykilio_ but are, as everything, open for discussion.

### Eventual consistency

As the aforementioned connected physical objects, further described as replicas, form a distributed system one also needs to analyze the consistency model of the system. Considering a set of **_0 &leq; n &leq; N_** replicas with a shared state **_S_** there are two possible choices for the architect of the system. They can either design the system for:

- **strong consistency**, where _S(1) = S(n) = S(N)_. This system is only strongly consistent and fault tolerant and if and only if the state is replicated synchronously and the system enters a read-only mode upon the failure of a single replica.

- **weak consistency**, where _S(1) ~ S(n) ~ S(N)_. This system can tolerate a fault of up to _ceil(N&nbsp;/&nbsp;2&nbsp;-&nbsp;1)_ replicas before being required to enter a read-only mode to remain state consistency.

Failures are unavoidable in large-scale distributed systems as every additional component increases the likelihood of a fault[^1]. So as to provide reliable and reponsive - read low latency - services to the users of a system, most architects of distributed systems opt for **eventual consistency**, which is a specific form of **weak consistency**, where the system converges to the expected state _S_ for _t &rarr; &infin;_ if and only if no changes are applied to the system after _t = 0_.

Eventually consistent systems are generally harder to implement, because they may require complex consensus algorithms if high-availability and fault tolerance are a requirement. They however also allow the architects to design systems, which can be distributed over long distances (e.g. geo-distribution), connected via unreliable network connections (e.g. cellular networks) or network connections with a high latency (e.g. low-energy wireless protocols) without significantly impacting the user experience.

### Declarative interface

A declarative interface allows the user to specify the desired state while guaranteeing the user that the system will eventually converge towards the desired state if no additional changes are made in the meantime. One major advantage of this is that it hides the complexity of the system from its user. The user is only required to specify the desired state, whereas the system implements the algorithms to modify the current state in such a way that it will converge towards the desired state. This process is often also called **reconciliation**. All reconciliation logic is hidden from the user and while this adds additional complexity for the implementation of the system, it makes the system safer for the user. Some declarative systems naturally possess self-healing features, such as the ability to automatically recover from failures, a property which provides additional safety and reliability.

<!-- Glossary -->

[^1]: [Principles of Eventual Consistency - Sebastian Burckhardt (2014)](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/final-printversion-10-5-14.pdf)
