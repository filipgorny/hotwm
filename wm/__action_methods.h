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
