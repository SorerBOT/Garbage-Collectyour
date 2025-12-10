# Garbage-Collectyour
As the name implies, this is not a Garbage Collector, but it also isn't far from working towards the same end. The Garbage Collector _header only_ library, aims to create a wrapper around C's _malloc_ and _free_ which would keep track of memory allocations, and provide the user with a detailed rundown of memory issues when the program exits, hence Garbage CollectYOUR.

## Inspiration
Felt like it'd be interesting to learn how Valgrind's memcheck works under the hood. A short research taught me that Valgrind somehow takes the compiled code, disassembles adds its own shenanigans and recompiles it, then allocates whatever memory the user allocates a second time in order to keep track of it.

Personally, I'm not there yet, but perhaps this would be the cornerstone.
On a serious note, I use Valgrind's memcheck occasionally, and I feel as if this _petit_ library might actually have a significant advantage over Valgrind, which is the SPEED with which it runs. In Valgrind's own words:
>Your program will run much slower (eg. 20 to 30 times) than normal, and use a lot more memory. Memcheck will issue messages about memory errors and leaks that it detects.

on the other hand, my program's memory footprint is significantly smaller (at least if the application is big) and the overhead is only a fragment of Valgrind's.
