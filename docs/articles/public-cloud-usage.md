---
layout: default
title: Public Cloud Cost Reduction
parent: Articles
---

# Can I do anything to reduce my already slim cloud costs?

_February 2023_

---

It might be possible, with a little work. Or, with a lot of work.

Given the questions I've been seeing and receiving lately, it appears there's a grown interest in overall cost reduction with an especial interest in public cloud resource usage. Even companies that have maintained strict controls over public cloud costs are now attempting to discover new and innovative methods to further reduce their expenses.

Given that, I've made a list (well, lists actually) of things that can be done to optimize cloud deployments and decided to make it available to everyone. It might be useful, or at least, amusing.

I'll also immediately apologize because I know there's a high probability none of the ideas will be plug and play and there's a lot of "it depends" at play.

But hopefully this can spark some ideas and open new avenues for you to explore.

Most of the costs commonly come from compute, storage, network (ingress and egress) and databases. I've divided the possible improvements for these areas into 3 parts: infrastructure, platform and application.

## Infrastructure - covering the basics

The most important thing to find areas for improvement is to measure. Then measure once more and then... you know what, don't stop measuring.

Once we have that, here's a few other things we can do just to cover the basics:

1. Use the tools offered by your cloud provider. They may not always be the most easy to use but they included in the cost and can quickly make it visible where costs are coming from.
2. Is there anything not being used? or not being used a lot? shut it down. You'd be surprised how frequent we find resources that were "temporarily" created and that are still running months later.
3. Reserve instances upfront. Most public cloud providers allow reserving compute or storage instances for a much lower price. This can be tricky since you'll pay for it even if not used. But the difference might be sufficient that even combining reserved instances with on-demand might reduce costs significantly.
4. Look for an alternative provider. This is hard one, I know but if you don't have much to move or if you can move parts of your workloads to a cheaper provider, it might mean considerable savings over time.
5. Make abundant use of auto scaling. Only turn things on when needed.


## Platform - changing how things work

Now that we've got the basics out of the way, here's a few other things we can try.

1. Go serverless, that provides a very optimal resource utilization strategy since we only pay for what's used. On a per request or task basis it might not be cheaper but for the systems where load is low or that comes in bursts, it might be a good solution.
2. Prefer object storage to block storage. It's slower but if that's not a a concern, you'd pay a lot less for it. It's also possible to mount a bucket with things like fuse and have it behave like a regular disk.
3. For network usage it's better to be where the user is. Transfers over different regions or across the globe is not only slow but also expensive.
4. Compress data before transferring. Either internally or externally, it can save a lot on network ingress and egress cost. If you offering APIs or web applications, be sure to have compression enabled (HTTP or any RPC you might be using).
5. Use spot instances. They are much cheaper, with the caveat that they can be preempted (turned off) at any time. It's possible to use a combination of fixed instances, spot instances with autoscaling groups to reduce overall cost while keeping systems durable.
6. Be sure to use the right database for the right needs. There are a number of different managed databases offered by cloud providers, some a lot more expensive than others and we may not always need all the features they offer.
7. For relational databases, use a combination of write and read replicas. Indices make querying fast though unfortunately, writing can be slow. If your application writes are being slow down by the number of indices, remove them from your write replica but keep them in the read replicas. It may require some work to get that setup properly but it works well.
8. Switch to ARM. Unless your workloads have processor specific dependencies, it might be possible for you to switch ARM. They are much cheaper and available by most cloud providers.
Naturally, one has to measure that the performance will be at least the same for the switch to be cost effective.

## Application - the things we build

These are more intrusive changes but also the ones that can have the most long term benefit.

1. For database bound applications, optimize database reads and writes. It will not only help with database costs but also potentially increase application throughput and make it possible to do more with less.
2. Profile applications and look for hotspots and bottlenecks. Pretty much every programming toolchain offers profiling tools which can be extremely helpful finding places to optimize.
3. Look for more efficient implementation of certain functions. This is very common in situations like serialization and deserialization in interpreted languages (JITted or not). Sometimes that slow json parser is worth the trouble of introducing interoperability with a C library, for example.
4. Split up the slow parts of the application. Sometimes there are features an application (service, API) provides that require a lot of resources but not used very often. By moving the resource intensive functionality into a separate application, resource balancing and allocation becomes much easier.
5. Write the critical parts of the code in a more performant ecosystem, (programming language). This is the hardest and most uncertain one. While it's possible to be a lot less resource intensive going from, say pure python to C++, development costs might also increase.
But similarly to the point above, maybe you don't need to do that for the entire code base, just for some parts and that might be enough to make a substantial difference.


All these suggestions will require investment, people's time, and that is expensive. Knowing what can be done in how much time is crucial to determine the best approach to take.

Something tells me we've entered the era of cloud austerity so the earlier we start with optimizations, and build a process for constantly doing that, the better.