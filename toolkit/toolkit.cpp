#include "toolkit.hpp"

#include "button.hpp"
#include "component.hpp"
#include "container.hpp"
#include "definition.hpp"

template <class TClass>
class Factory {
public:
    static TClass* Create(){return new TClass();}
};

ComponentAlias* toolkit_components;
int toolkit_components_count = 0;

component::Button* toolkit_create_button(const definition::ComponentDefinition* def) {
    return new component::Button();
}

component::Container* toolkit_create_container(const definition::ComponentDefinition* def) {
    return new component::Container();
}

View* toolkit_generate_view(ViewDef *def) {
   View *view = (View*)malloc(sizeof(View));
 
   return view;
}
