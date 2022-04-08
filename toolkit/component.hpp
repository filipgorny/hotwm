#include <stdio.h>
#include <stdlib.h>

#include "style.hpp"
#include "tasks.h"
#include "style.h"

extern "C" {
    #include "style.h"
}

typedef void(*Callback)(void);

namespace component {
    class Component {
        protected:
            Style* style;
            Callback onClickCallback;
            Callback onHoverCallback;
            void init();
        public:
            Component();
            void onHover();
            void onClick();
            void setOnClick(Callback callback);
            void setOnHover(Callback callback);
    };
};
