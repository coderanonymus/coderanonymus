#ifndef AETHER_GUI_H
#define AETHER_GUI_H

#include <stdint.h>

typedef enum {
    GUI_EVT_NONE,
    GUI_EVT_KEY,
    GUI_EVT_MOUSE,
    GUI_EVT_REDRAW,
} gui_event_type_t;

typedef struct {
    gui_event_type_t type;
    uint32_t a;
    uint32_t b;
} gui_event_t;

void gui_init(void);
void gui_post_event(gui_event_t evt);
void gui_pump_events(void);

#endif
