#ifndef AETHER_VFS_H
#define AETHER_VFS_H

#include <stdint.h>
#include "aether/types.h"

#define VFS_NAME_MAX 32

typedef enum {
    VNODE_FILE,
    VNODE_DIR,
    VNODE_DEV,
    VNODE_PROC,
} vnode_type_t;

typedef struct vnode {
    char name[VFS_NAME_MAX];
    vnode_type_t type;
    uint16_t mode;
    uint32_t uid;
    uint32_t gid;
    struct vnode *next;
} vnode_t;

typedef struct mount {
    char path[VFS_NAME_MAX];
    const char *fs_name;
    vnode_t *root;
    struct mount *next;
} mount_t;

void vfs_init(void);
aether_status_t vfs_mount(const char *path, const char *fs_name);
aether_status_t vfs_mknod(const char *path, vnode_type_t type, uint16_t mode);
void vfs_dump_mounts(void);

#endif
