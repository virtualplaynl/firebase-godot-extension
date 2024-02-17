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
    class FirestoreTransaction : public Object
    {
        GDCLASS(FirestoreTransaction, Object);

    protected:
        static void _bind_methods();
        Array operations;

    public:
        FirestoreTransaction();
        ~FirestoreTransaction();

        firestore::Error run(Firestore *firestore, Transaction &transaction, std::string &errorMessage);

        void addDelete(String docPath);
        void addGet(String docPath);
        void addSet(String docPath, Dictionary fields, godot::SetOptions setOptions, Array mergefields);
        void addUpdate(String docPath, Dictionary fields);
    };
}
