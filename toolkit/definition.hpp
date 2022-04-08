#pragma once

namespace component {
    typedef union {
        char* c;
        int* i;
        float* f;
        bool *b;
    } StyleDefValue;

    typedef struct {
        char* name;
        StyleDefValue value;
    } StyleDef;

    typedef struct ComponentDef ComponentDefiStruct;
    struct ComponentDefStruct {
        char* name;
        ComponentDef *child;
        StyleDef* style;
    };

    typedef struct {
        ComponentDef *root;
    } ViewDef;
}
