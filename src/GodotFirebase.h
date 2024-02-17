#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>
#include <firebase/messaging.h>
#include "GodotFirebaseAnalytics.h"
#include "GodotFirebaseAuth.h"
#include "GodotFirebaseDatabase.h"
#include "GodotFirestore.h"
#include "GodotFirebaseFunctions.h"
#include "GodotFirebaseMessaging.h"
#include "GodotFirebaseRemoteConfig.h"
#include "GodotFirebaseStorage.h"

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    class GodotFirebase : public Object
    {
        GDCLASS(GodotFirebase, Object);

    protected:
        static void _bind_methods();
        static GodotFirebase *instance;
        App *app;
        GodotFirebaseAuth *auth;
        GodotFirebaseDatabase *database;
        GodotFirestore *firestore;
        GodotFirebaseFunctions *functions;
        GodotFirebaseMessaging *messaging;
        GodotFirebaseStorage *storage;

    public:
        GodotFirebase();
        ~GodotFirebase();

        void init();
        void initWith(String configJson);
        static GodotFirebase *get_singleton();

        GodotFirebaseAuth *getAuth();
        GodotFirebaseDatabase *getDatabase();
        GodotFirestore *getFirestore();
        GodotFirebaseFunctions *getFunctions();
        GodotFirebaseMessaging *getMessaging();
        GodotFirebaseStorage *getStorage();
    };
}