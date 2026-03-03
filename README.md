# AetherOS Phase 4

## 1. Phase 4 Architecture Overview
AetherOS now evolves from a modular kernel baseline into a **hybrid kernel + integrated type-1 hypervisor** trajectory. Existing boot path, MM, scheduler, VFS, network, GUI, and tooling remain in place and are extended (not restarted). The architecture splits into:
- **microkernel core plane** (scheduling, IPC, MM, interrupt core)
- **isolated service plane** (drivers, storage, network services, graphics services)
- **hypervisor plane** (VMX/SVM virtualization substrate)
- **policy plane** (capabilities, sandboxing, namespaces, resource controls)

## 2. Hybrid Kernel Design
Hybrid direction introduced by scaffolding and interfaces:
- core runtime still in kernel space for bring-up stability
- driver/service separation prepared through dedicated service modules and explicit APIs
- message-oriented subsystem interaction enabled via IPC queues (`sched/ipc.c` + service endpoints)
- restartable service intent represented by service restart hooks (`kernel/services/restart.c`)

Isolation model evolution path:
1. Move each driver into a service task.
2. Route all privileged operations via capability-checked IPC.
3. Add crash monitor to restart failed services.

## 3. Hypervisor Implementation Plan
Added hypervisor subsystem (`kernel/hypervisor/*`):
- Intel VT-x detection via CPUID VMX bit
- AMD-V SVM detection via extended CPUID
- backend chooser (`HV_BACKEND_VMX`/`HV_BACKEND_SVM`)
- guest context structure and launch entry (`hv_launch_guest`)
- VM-exit dispatch entry (`hv_vmexit_dispatch`)

Next concrete steps:
- VMXON region and VMCS allocation/alignment
- host/guest state fields load
- EPT/NPT page-table builder
- VMEXIT decoding: CPUID, HLT, IO, EPT faults
- virtual interrupt injection via posted interrupts or emulated LAPIC path

## 4. Advanced Scheduler Design
Phase 4 adds `sched2` (CFS-style scaffold):
- virtual runtime tracking (`vruntime`)
- affinity hooks (`cpu_affinity`)
- RT/deadline metadata (`rt_class`, `deadline_ns`)
- multi-policy engine path to coexist with legacy scheduler during migration

Planned upgrades:
- per-CPU rb-tree run queues
- NUMA-aware load balancing
- RT priority classes + EDF deadline queue

## 5. Memory System Upgrade
Phase 4 introduces `mm2` framework for:
- transparent huge pages policy point
- reclaim daemon hooks
- compression and memory-pressure counters
- swap and accounting extension points

Existing MM keeps COW/demand/guard logic while mm2 layers advanced policies.

## 6. Capability Security Model
New security subsystem (`kernel/security/capability.c`):
- capability tokens
- syscall filter objects (seccomp-like allow masks)
- per-process capability checks

Upgrade path:
- enforce capability checks in syscall dispatch
- privilege bounding sets per process/container
- policy loading from signed security manifests

## 7. Container System
New container subsystem (`kernel/container/container.c`):
- namespace fields: PID, NET, MNT
- resource limits: memory + CPU quota
- container launch API scaffold

Next:
- namespace ID allocators
- virtual procfs view per namespace
- cgroups-like hierarchical accounting and throttling

## 8. Graphics 2.0 Architecture
New `gfx2` subsystem:
- graphics HAL-facing init path
- scene composition function
- display server poll loop entry

Roadmap:
- GPU driver interface contracts
- scene graph with damage tracking
- multi-monitor scanout targets
- client/server display protocol IPC endpoints

## 9. Networking 2.0 Architecture
New `net2` subsystem:
- packet filter API (netfilter-style rule insertion)
- route table insertion API
- DHCP discovery entrypoint
- DNS resolver API

Roadmap:
- socket layer separation from protocol engines
- stateful firewall tables
- routing policy database and multipath

## 10. Storage Stack
New `storage` subsystem:
- block layer submission API
- NVMe initialization hook
- LVM volume API scaffold
- placement for journaling/snapshot managers

Roadmap:
- pluggable I/O schedulers
- writeback + journal replay engine
- snapshot COW metadata tree

## 11. Developer Ecosystem
New SDK and service foundations:
- kernel/user SDK init hooks
- lightweight service manager foundation
- package manager foundation

Planned:
- module ABI + symbol versioning
- userspace build sysroot packaging
- doc generation tooling from headers + manifests

## 12. Updated Folder Structure
```text
AetherOS/
├── boot/bios/
├── include/aether/
│   ├── hypervisor.h security.h container.h storage.h
│   ├── net2.h gfx2.h sdk.h sched2.h mm2.h
│   └── (existing core headers)
├── kernel/
│   ├── hypervisor/{core.c,vmx.c,svm.c}
│   ├── sched2/cfs.c
│   ├── mm/mm2.c
│   ├── security/capability.c
│   ├── container/container.c
│   ├── net2/stack.c
│   ├── gfx2/compositor.c
│   ├── storage/block.c
│   ├── sdk/sdk.c
│   ├── services/restart.c
│   └── (existing arch/mm/sched/fs/net/gui/debug/user files)
├── docs/design.md
├── Makefile
└── linker.ld
```

## 13. Boot Flow with Hypervisor Layer
1. BIOS stage1/stage2 load kernel and enter long mode.
2. Kernel early init starts debug console.
3. Hypervisor probe (`hv_init`) selects VMX/SVM backend.
4. Core kernel services initialize (MM, scheduler, process/security).
5. Service-plane subsystems initialize (VFS/storage/net/gui/drivers).
6. SDK/service manager/package manager initialize.
7. Shell/user bootstrap continues as before.

Future insertion point: optional “self-hosted guest mode” where AetherOS kernel can run as L1 guest with thin L0 host runtime.

## 14. Integration Strategy
- Keep old subsystems operational while introducing phase-4 modules in parallel.
- Migrate feature-by-feature behind stable interfaces (`*_init`, policy APIs).
- Gate high-risk changes (VMX/SVM launch, EPT/NPT) with runtime feature detection.
- Keep `check-host` as baseline compile gate and extend to emulation tests when cross toolchain is available.

## 15. Long-Term Evolution Path
- Phase 4.1: real VMXON/VMCS + first guest launch.
- Phase 4.2: service isolation with restart supervisor.
- Phase 4.3: capability-enforced syscall path + namespaces fully active.
- Phase 4.4: full socket/filter/routing and NVMe-backed journaling FS.
- Phase 4.5: display server protocol and hardware-accelerated compositor.

## Build / Test
```bash
make check-host
make
make iso
make run
make run-iso
```

## QEMU SMP + debug examples
```bash
qemu-system-x86_64 -drive format=raw,file=build/aetheros.img -smp 4 -m 1024 -serial stdio
qemu-system-x86_64 -drive format=raw,file=build/aetheros.img -s -S -serial stdio
gdb build/kernel.elf
```
