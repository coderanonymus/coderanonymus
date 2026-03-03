#include "aether/arch.h"
#include "aether/container.h"
#include "aether/debug.h"
#include "aether/drivers.h"
#include "aether/fs.h"
#include "aether/gfx2.h"
#include "aether/gui.h"
#include "aether/hypervisor.h"
#include "aether/log.h"
#include "aether/mm.h"
#include "aether/mm2.h"
#include "aether/net.h"
#include "aether/net2.h"
#include "aether/process.h"
#include "aether/sched.h"
#include "aether/sched2.h"
#include "aether/sdk.h"
#include "aether/security.h"
#include "aether/storage.h"
#include "shell.h"
#include "vga.h"

void kmain(void) {
    dbg_console_init();
    vga_clear();
    vga_writeln("AetherOS kernel online");
    vga_writeln("Phase 4 hybrid/hypervisor evolution active");

    hv_init();
    arch_init();

    mm_init();
    mm2_init();

    sched_init();
    sched2_init();

    process_init();
    security_init();
    container_init();

    vfs_init();
    storage_init();

    net_init();
    net2_init();

    gui_init();
    gfx2_init();

    drivers_init();

    sdk_init();
    service_manager_init();
    package_manager_init();

    log_info("kernel: phase 4 subsystem bootstrap complete");
    shell_run();

    panic("shell returned unexpectedly");
}
