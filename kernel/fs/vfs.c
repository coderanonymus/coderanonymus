#include "aether/vfs.h"
#include "aether/log.h"

static mount_t mounts[8];
static int mount_count;
static vnode_t nodes[32];
static int node_count;

static void str_copy(char *dst, const char *src) {
    int i = 0;
    while (src && src[i] && i < (VFS_NAME_MAX - 1)) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

void vfs_init(void) {
    mount_count = 0;
    node_count = 0;
    vfs_mount("/", "tmpfs");
    vfs_mknod("/dev", VNODE_DEV, 0755);
    vfs_mknod("/proc", VNODE_PROC, 0555);
    log_info("vfs: root, /dev and /proc ready");
}

aether_status_t vfs_mount(const char *path, const char *fs_name) {
    if (mount_count >= (int)(sizeof(mounts)/sizeof(mounts[0]))) {
        return AETHER_ENOMEM;
    }
    mount_t *m = &mounts[mount_count++];
    str_copy(m->path, path);
    m->fs_name = fs_name;
    m->root = 0;
    m->next = 0;
    return AETHER_OK;
}

aether_status_t vfs_mknod(const char *path, vnode_type_t type, uint16_t mode) {
    if (node_count >= (int)(sizeof(nodes)/sizeof(nodes[0]))) {
        return AETHER_ENOMEM;
    }
    vnode_t *n = &nodes[node_count++];
    str_copy(n->name, path);
    n->type = type;
    n->mode = mode;
    n->uid = 0;
    n->gid = 0;
    n->next = 0;
    return AETHER_OK;
}

void vfs_dump_mounts(void) {
    for (int i = 0; i < mount_count; i++) {
        log_info(mounts[i].path);
    }
}
