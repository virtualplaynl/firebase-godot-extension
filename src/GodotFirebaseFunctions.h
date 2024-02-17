#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <firebase/app.h>
#include <firebase/functions.h>
#include "FirebaseFunction.h"
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::functions;

namespace godot
{
    class GodotFirebaseFunctions : public Object
    {
        GDCLASS(GodotFirebaseFunctions, Object);

    protected:
        static void _bind_methods();
        Functions *functions;
        Dictionary cache;

    public:
        GodotFirebaseFunctions();
        ~GodotFirebaseFunctions();

        void init(App *app);

        FirebaseFunction *prepare(String name, SceneTree *tree);
    };
}