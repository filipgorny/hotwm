#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <functional>

#include "view.hpp"
#include "definition.hpp"

#define COMPONENT_CREATION std::function<component::Component*(definition::ComponentDef*)> 

typedef view::View View;
typedef component::Component Component;

View* toolkit_generate_view();

