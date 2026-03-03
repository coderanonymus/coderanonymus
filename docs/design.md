# AetherOS Design Progress

This document tracks current implementation status as the kernel evolves from minimal boot baseline to subsystem-oriented architecture.

## Implemented subsystem layout
- `kernel/mm`: virtual memory areas, COW and demand-paging fault path framework, guard pages.
- `kernel/sched`: priority scheduler, sleep queue, block/wakeup mechanisms.
- `kernel/sched/ipc.c`: message queue IPC primitives.
- `kernel/fs`: VFS core with mount table, `/dev`, `/proc`, and permission fields.
- `kernel/net`: ARP/ICMP/TCP entry flows and HTTP demo hook.
- `kernel/user`: process API (`fork/exec/wait/kill`), minimal signal handling, libc subset, ELF relocations.
- `kernel/gui`: event queue, input-driven pump, double-buffer model.
- `kernel/debug`: debug console, panic stack trace, symbol-range wiring.
- `kernel/arch/x86_64`: HPET, IOAPIC, MSI, USB initialization hooks.

## Next integration milestones
1. Wire real page table manipulation and physical frame allocator into MM APIs.
2. Connect timer IRQs to `sched_tick()` for preemption.
3. Add process table locking and per-task kernel/user stacks.
4. Back VFS with FAT32/EXT2 drivers and file descriptor layer.
5. Attach networking layer to real NIC driver with RX/TX rings.
6. Transition GUI from RAM buffers to GOP framebuffer backend.
7. Add symbol lookup table and address-to-name panic output.
