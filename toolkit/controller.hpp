#pragma once

#define ACTION_FUNC void (*action)(void);

namespace controller {
    typedef struct {
        const char* name;
        ACTION_FUNC;
    } ControllerAction;

    class Controller {
        private:
            ControllerAction* *actions;
            int actions_count;
        public:
            void add_action(const char* name, void (*action)(void));
            void trigger_action(const char* name);
    };
}
