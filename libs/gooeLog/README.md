# Logging system

This project is an attempt to write a simple and performant logging system in C.
It's a learning project, so it will evolve with discoveries along the journey.

## How to use it

It's a header only file, so you only need to include it in your project.
The implementation of the lib is available with `GOOE_LOG_IMPLEMENTATION`.

```C
#define GOOE_LOG_IMPLEMENTATION
#include <gooeLog/log.h>
```

If you want to run your program and remove all the logs, you can pass `-DGOOE_LOG_NONE`.
