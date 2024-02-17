#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/database.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::database;

namespace godot
{
    class GodotFirebaseDatabase : public Object
    {
        GDCLASS(GodotFirebaseDatabase, Object);

    protected:
        static void _bind_methods();
        Database *database;

    public:
        GodotFirebaseDatabase();
        ~GodotFirebaseDatabase();

        void init(App *app);

        void offline();
        void online();
    };
}