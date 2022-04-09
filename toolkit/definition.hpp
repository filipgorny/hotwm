#pragma once

#include <iostream>
#include <string>

#define COMPONENT_CONTAINER 100
#define COMPONENT_BUTTON 101

namespace definition {
    typedef union {
        char* c;
        int* i;
        float* f;
        bool *b;
    } PropertyDefValue;

    typedef struct {
        char* name;
        PropertyDefValue value;
    } Property;

    class ComponentDefinition {
        private:
            Property** properties;
            int property_count;
        public:
            int type;
            ComponentDefinition(int type);
            void define(std::string name, PropertyDefValue value);
            PropertyDefValue* get(std::string name);
    };
}
