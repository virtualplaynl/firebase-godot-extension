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
    class FirestoreListener : public Object
    {
        GDCLASS(FirestoreListener, Object);

    protected:
        static void _bind_methods();
        ListenerRegistration registration;
        String docPath, collectionPath, collectionId;

    public:
        FirestoreListener();
        ~FirestoreListener();

        void setToDocument(String docPath, ListenerRegistration registration);
        void setToCollection(String collectionPath, ListenerRegistration registration);
        void setToCollectionGroup(String collectionId, ListenerRegistration registration);
        void setToInSync(ListenerRegistration registration);

        void stop();

        void inSyncCallback();
        void docUpdated(const DocumentSnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage);
        void collectionUpdated(const QuerySnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage);
        void collectionGroupUpdated(const QuerySnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage);
    };
}