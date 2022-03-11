#include "log.h"
#include "session.h"

void __next_client(Arg *arg) {
    session_select_next_client(session);

    refresh();
}

void __prev_client(Arg *arg) {
    session_select_previous_client(session);

    refresh();
}

void __layout_mono(Arg *arg) {
    return;
    session_select_layout(session, "mono");
    refresh();
}

void __layout_stack(Arg *arg) {
    session_select_layout(session, "stack");
    refresh();
}
