#include "shell.h"
#include "keyboard.h"
#include "vga.h"
#include "aether/net.h"
#include "aether/process.h"
#include "aether/vfs.h"

#define CMD_MAX 96

static int streq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == 0 && *b == 0;
}

static int starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s++ != *prefix++) return 0;
    }
    return 1;
}

static void prompt(void) {
    vga_write("aether> ");
}

void shell_run(void) {
    char cmd[CMD_MAX];
    for (;;) {
        prompt();
        int len = 0;
        while (1) {
            char c = keyboard_getchar();
            if (c == '\n') {
                vga_putc('\n');
                cmd[len] = 0;
                break;
            }
            if (c == '\b') {
                if (len > 0) {
                    len--;
                    vga_putc('\b');
                }
                continue;
            }
            if (len < CMD_MAX - 1) {
                cmd[len++] = c;
                vga_putc(c);
            }
        }

        if (streq(cmd, "help")) {
            vga_writeln("commands: help clear about echo ps mounts fork ping http");
        } else if (streq(cmd, "clear")) {
            vga_clear();
        } else if (streq(cmd, "about")) {
            vga_writeln("AetherOS evolving kernel");
        } else if (streq(cmd, "ps")) {
            vga_writeln("pid 1  kernel-init");
        } else if (streq(cmd, "mounts")) {
            vfs_dump_mounts();
        } else if (streq(cmd, "fork")) {
            pid_t child = sys_fork();
            (void)child;
            vga_writeln("fork requested");
        } else if (starts_with(cmd, "echo ")) {
            vga_writeln(&cmd[5]);
        } else if (streq(cmd, "ping")) {
            ipv4_addr_t ip = {{8, 8, 8, 8}};
            if (icmp_ping(ip) == 0) vga_writeln("ping ok");
            else vga_writeln("ping fail");
        } else if (streq(cmd, "http")) {
            if (http_get_demo("example.com", "/") == 0) vga_writeln("http ok");
            else vga_writeln("http fail");
        } else if (cmd[0] == 0) {
            continue;
        } else {
            vga_writeln("unknown command");
        }
    }
}
