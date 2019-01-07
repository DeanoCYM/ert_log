#include <stdio.h>
#include "ert_log.h"

int main(int argc, char **argv)
{
    // testing basic logging functionality
    ERT_LOG(stderr, "[LOG]", "Testing ERT_LOG()\n");
    errno = EIO;
    log_err("This is an error");
    errno = 0;
    log_warn("This is a warning");
    log_info("This is information");
    log_debug("This is debugging information");
    
    return 0;
}
