#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    class FirestoreBatch : public Object
    {
        GDCLASS(FirestoreBatch, Object);

    protected:
        static void _bind_methods();
        Firestore *firestore;
        WriteBatch batch;

    public:
        FirestoreBatch();
        ~FirestoreBatch();

        void init(Firestore *firestore);

        void addDelete(String docPath);
        void addSet(String docPath, Dictionary fields, godot::SetOptions setOptions, Array mergefields);
        void addUpdate(String docPath, Dictionary fields);
        bool isValid();
        int commit();
    };
}