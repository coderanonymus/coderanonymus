# AetherOS

AetherOS continues from the existing BIOS + long-mode baseline and now evolves into modular subsystems without redesigning the architecture.

## Updated Folder Structure

```text
AetherOS/
├── boot/
│   └── bios/
│       ├── boot.asm
│       └── stage2.asm
├── docs/
│   └── design.md
├── include/
│   ├── io.h
│   ├── keyboard.h
│   ├── shell.h
│   ├── vga.h
│   └── aether/
│       ├── arch.h
│       ├── debug.h
│       ├── drivers.h
│       ├── elf.h
│       ├── fs.h
│       ├── gui.h
│       ├── ipc.h
│       ├── libc.h
│       ├── log.h
│       ├── mm.h
│       ├── net.h
│       ├── process.h
│       ├── sched.h
│       ├── types.h
│       └── vfs.h
├── kernel/
│   ├── arch/x86_64/platform.c
│   ├── debug/debug.c
│   ├── debug/log.c
│   ├── drivers/drivers.c
│   ├── fs/vfs.c
│   ├── gui/gui.c
│   ├── mm/mm.c
│   ├── net/net.c
│   ├── user/elf.c
│   ├── user/libc.c
│   ├── user/process.c
│   ├── sched/ipc.c
│   ├── sched/sched.c
│   ├── entry.asm
│   ├── keyboard.c
│   ├── main.c
│   ├── shell.c
│   └── vga.c
├── scripts/
│   └── setup-cross.sh
├── linker.ld
└── Makefile
```

## Subsystem Progress

### 1) Memory management (`mm/`)
- Added address-space and VMA framework with explicit flags for:
  - copy-on-write (`VM_COW`)
  - demand paging (`VM_DEMAND`)
  - user/kernel policy markers (`VM_USER`)
  - guard pages (`VM_GUARD`)
- Added `mm_handle_page_fault()` skeleton with COW and demand resolution flow.
- Added `mm_fork_cow()` for fork-time COW mappings.

### 2) Scheduler (`sched/`)
- Replaced RR design with priority run queues (`0..31`).
- Added sleep queue and timer-based wake-up.
- Added block/wakeup primitives.
- Added basic message-queue IPC (`ipc_send`/`ipc_recv`).

### 3) Process model (`user/process.c`)
- Added syscall-level process primitives:
  - `sys_fork()`
  - `sys_exec()`
  - `sys_wait()`
  - `sys_kill()`
  - minimal signal delivery framework

### 4) VFS expansion (`fs/vfs.c`)
- Added mount table representation.
- Added node permissions fields (`mode`, `uid`, `gid`).
- Bootstraps `/`, `/dev`, and `/proc` vnode creation.

### 5) Networking (`net/net.c`)
- Added ARP resolve hook.
- Added ICMP ping flow entry.
- Added TCP connect state-entry function.
- Added HTTP GET demo routine (`http_get_demo`).

### 6) Userland support (`user/`)
- Added minimal libc subset (`strlen`, `strcmp`, `memcpy`).
- Added dynamic-linking foundation with ELF RELA processing (`R_X86_64_RELATIVE`).

### 7) GUI (`gui/gui.c`)
- Added event-driven queue.
- Added double-buffering model (`front_buffer`, `back_buffer`).
- Added redraw event path for compositor-style copy.

### 8) Debugging (`debug/`)
- Added debug console abstraction.
- Added panic path with stack-frame walk.
- Added linker symbols for symbol table range discovery.

### 9) Hardware support (`arch/x86_64/platform.c`)
- Added initialization hooks/foundations for:
  - HPET
  - IOAPIC routing
  - MSI
  - USB core

## Updated Build System

### Prerequisites
- `nasm`
- `xorriso`
- `qemu-system-x86_64`
- `x86_64-elf-gcc`, `x86_64-elf-ld`, `x86_64-elf-objcopy`

### Build targets
```bash
make          # raw image
make iso      # BIOS El Torito ISO
make run      # run raw image
make run-iso  # run ISO
make check-host # host-side syntax check with gcc
```

## Integration Notes

1. Current code is an integration baseline for subsystem APIs and control flow.
2. Existing BIOS boot path remains intact and now loads a larger modular kernel image.
3. `kmain()` now initializes architecture, MM, scheduler, process, VFS, networking, GUI, and drivers before entering shell.
4. Shell now exercises new pathways (`ps`, `mounts`, `fork`, `ping`, `http`).

## QEMU SMP test flags

Use SMP and debug-friendly options:

```bash
qemu-system-x86_64 \
  -drive format=raw,file=build/aetheros.img \
  -smp 4,sockets=1,cores=4,threads=1 \
  -m 512M \
  -serial stdio \
  -d int,cpu_reset
```

ISO variant:

```bash
qemu-system-x86_64 \
  -cdrom build/aetheros.iso \
  -smp 4,sockets=1,cores=4,threads=1 \
  -m 512M \
  -serial stdio
```

## GDB debugging

Start QEMU paused with gdb stub:

```bash
qemu-system-x86_64 \
  -drive format=raw,file=build/aetheros.img \
  -s -S -serial stdio
```

In another terminal:

```bash
gdb build/kernel.elf
(gdb) target remote :1234
(gdb) set architecture i386:x86-64
(gdb) b kmain
(gdb) c
```

For panic traces, inspect frame chain:

```gdb
(gdb) info registers rbp rsp rip
(gdb) x/16gx $rbp
```

## Real hardware notes

- BIOS/CSM path remains the current boot target.
- Keep Secure Boot disabled for this branch.
- VGA cursor is disabled at boot to avoid blinking white artifact behavior on text-mode output.

