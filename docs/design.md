# AetherOS Design Progress (Phase 4)

## Hybrid kernel transition status
- Core plane retained: scheduler, IPC, memory manager, interrupt path.
- Service plane expanded: storage/net/gfx/service-manager scaffolds.
- Security plane expanded: capability tokens + syscall filter framework.
- Container plane introduced: namespace + resource limit spec.

## Hypervisor status
- VT-x/SVM CPUID probing and backend selection implemented.
- Guest context and VM-launch API shape established.
- VM exit dispatch hook in place.

## Advanced subsystems status
- Scheduler v2 scaffold (CFS-like metadata and RT/deadline fields).
- MM v2 scaffold (reclaim/compression/pressure hooks).
- Net2 scaffold (filter/routing/DHCP/DNS API).
- Storage scaffold (block/NVMe/LVM API).
- Gfx2 scaffold (HAL/compositor/display-server API).

## Next milestones
1. Implement VMXON/VMCS and SVM VMCB low-level paths.
2. Add per-CPU run queues and true preemption integration for sched2.
3. Wire MM2 reclaim and swap to physical allocator and fault path.
4. Enforce capability checks and syscall filters in syscall dispatcher.
5. Connect container namespace IDs to process table and VFS views.
6. Build real socket layer and NIC device framework in net2.
7. Add NVMe queues and persistent journal backend for storage.
