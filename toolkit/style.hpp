#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

namespace component {
    struct StyleValue {
        char* name;
        union {
            char* s;
            int i;
            float f;
            bool b;
            bool isUndefined;
        };
    };

    class Style {
        StyleValue* values;
        int length;

        public:
            void set(char* name, char* value) {

                StyleValue *v = new StyleValue();
                v->name = name;
                v->s = value;

                this->values = (StyleValue*)realloc(this->values, sizeof(StyleValue) * (this->length + 1));

                v->isUndefined = false;

                this->values[this->length] = *v;

                this->length++;
            }

            StyleValue* get(char* name) {
                for (int i = 0; i < this->length; i++) {
                    if (strcmp(this->values[i].name, name) == 0) {
                        return &this->values[i];
                    }
                }

                StyleValue *v;
                v->name = name;
                v->isUndefined = true;

                return v;
            }
    };
};
