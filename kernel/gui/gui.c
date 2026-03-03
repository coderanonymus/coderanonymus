#include "aether/gui.h"
#include "aether/log.h"

#define GUI_Q_MAX 64

static gui_event_t q[GUI_Q_MAX];
static unsigned head;
static unsigned tail;
static uint32_t front_buffer[320 * 200];
static uint32_t back_buffer[320 * 200];

void gui_init(void) {
    head = 0;
    tail = 0;
    for (unsigned i = 0; i < 320u * 200u; i++) {
        front_buffer[i] = 0;
        back_buffer[i] = 0x00101010;
    }
    log_info("gui: event-driven desktop core with double buffering");
}

void gui_post_event(gui_event_t evt) {
    unsigned next = (tail + 1u) % GUI_Q_MAX;
    if (next == head) {
        return;
    }
    q[tail] = evt;
    tail = next;
}

void gui_pump_events(void) {
    while (head != tail) {
        gui_event_t evt = q[head];
        head = (head + 1u) % GUI_Q_MAX;
        if (evt.type == GUI_EVT_REDRAW) {
            for (unsigned i = 0; i < 320u * 200u; i++) {
                front_buffer[i] = back_buffer[i];
            }
        }
    }
}
