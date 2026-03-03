#include "aether/arch.h"
#include "aether/debug.h"
#include "aether/drivers.h"
#include "aether/fs.h"
#include "aether/gui.h"
#include "aether/log.h"
#include "aether/mm.h"
#include "aether/net.h"
#include "aether/process.h"
#include "aether/sched.h"
#include "shell.h"
#include "vga.h"

void kmain(void) {
    dbg_console_init();
    vga_clear();
    vga_writeln("AetherOS kernel online");
    vga_writeln("64-bit long mode active");

    arch_init();
    mm_init();
    sched_init();
    process_init();
    vfs_init();
    net_init();
    gui_init();
    drivers_init();

    log_info("kernel: subsystem bootstrap complete");
    shell_run();

    panic("shell returned unexpectedly");
}
