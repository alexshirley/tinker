# tinker
Collection of C Abstract Data Types targeted at Embedded Systems

## Motivation
Why another C library you may ask?

* First, I'm tired with seeing fifos/ring-buffers, stacks, priority queues, etc. being implemented from scratch, over and over again, typically embedded right in other functions; I can't tell you how many hidden FIFO's I've work on in 2020, each one with bugs. I want some containers which abstract some of that non-sense away, and make C a palatable language, especially as I spend a lot of time in the embedded systems realm, where I don't always have access to a C++ compiler.

* Second, I'm  astounded by the number of developers coming out of college who don't understand things like Abstract Data Type implementation in C, Unit Testing, Benchmarking, etc. Lots of devs know how to write a for-loop, but few know how to make something readable. 