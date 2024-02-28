#include <stdio.h>
#define GOOE_LOG_IMPLEMENTATION
#include <gooeLog/log.h>

int main(int argc, char** argv) {
    LOG_INFO("For your %s", "INFO");
    LOG_DEBUG("For your %s", "DEBUG");
    LOG_WARN("For your %s", "WARNING");
    LOG_ERR("For your %s", "ERROR");
}
