#include "aether/drivers.h"
#include "aether/gui.h"
#include "aether/log.h"

void input_manager_poll(void) {
    gui_event_t evt;
    evt.type = GUI_EVT_REDRAW;
    evt.a = 0;
    evt.b = 0;
    gui_post_event(evt);
}

void drivers_init(void) {
    log_info("drivers: core init (kbd/mouse/usb routing)");
}
