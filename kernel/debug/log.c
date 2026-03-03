#include "aether/log.h"
#include "aether/debug.h"

void log_info(const char *msg) {
    dbg_puts("[INFO] ");
    dbg_puts(msg);
}

void log_warn(const char *msg) {
    dbg_puts("[WARN] ");
    dbg_puts(msg);
}

void log_error(const char *msg) {
    dbg_puts("[ERR ] ");
    dbg_puts(msg);
}
