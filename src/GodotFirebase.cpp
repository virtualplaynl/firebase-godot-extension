#include "GodotFirebase.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

GodotFirebase *GodotFirebase::instance;

GodotFirebase::GodotFirebase()
{
    if(instance != NULL) return;
    
    instance = this;

    app = nullptr;
    auth = nullptr;
    database = nullptr;
    firestore = nullptr;
    functions = nullptr;
    messaging = nullptr;
    storage = nullptr;
}

GodotFirebase::~GodotFirebase()
{
    if (instance == this) instance = NULL;
}

GodotFirebase *GodotFirebase::get_singleton()
{
    return instance;
}

void GodotFirebase::init()
{
    if (app == nullptr)
    {
        app = App::Create();
        if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initialized Firebase App.");
    }
}

void GodotFirebase::initWith(String configJson)
{
    if (app == nullptr)
    {
        AppOptions options = AppOptions();
        AppOptions::LoadFromJsonConfig(configJson.utf8().ptr(), &options);

#if defined(__ANDROID__)
        app = App::Create(options, my_jni_env, my_activity);
#else
        app = App::Create(options);
#endif // defined(__ANDROID__)
        if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initialized Firebase App.");
    }
}

GodotFirebaseAuth *GodotFirebase::getAuth()
{
    if(auth == nullptr && app != nullptr)
    {
        auth = memnew(GodotFirebaseAuth);
        auth->init(app);
    }
    return auth;
}
GodotFirebaseDatabase *GodotFirebase::getDatabase()
{
    if (database == nullptr && app != nullptr)
    {
        database = memnew(GodotFirebaseDatabase);
        database->init(app);
    }
    return database;
}
GodotFirestore *GodotFirebase::getFirestore()
{
    if (firestore == nullptr && app != nullptr)
    {
        firestore = memnew(GodotFirestore);
        firestore->init(app);
    }
    return firestore;
}
GodotFirebaseFunctions *GodotFirebase::getFunctions()
{
    if (functions == nullptr && app != nullptr)
    {
        functions = memnew(GodotFirebaseFunctions);
        functions->init(app);
    }
    return functions;
}
GodotFirebaseMessaging *GodotFirebase::getMessaging()
{
    if (messaging == nullptr && app != nullptr)
    {
        messaging = memnew(GodotFirebaseMessaging);
        messaging->init(app);
    }
    return messaging;
}
GodotFirebaseStorage *GodotFirebase::getStorage()
{
    if (storage == nullptr && app != nullptr)
    {
        storage = memnew(GodotFirebaseStorage);
        storage->init(app);
    }
    return storage;
}

void GodotFirebase::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init"), &GodotFirebase::init);
    ClassDB::bind_method(D_METHOD("init_with", "configJson"), &GodotFirebase::initWith);
    ClassDB::bind_method(D_METHOD("auth"), &GodotFirebase::getAuth);
    ClassDB::bind_method(D_METHOD("database"), &GodotFirebase::getDatabase);
    ClassDB::bind_method(D_METHOD("firestore"), &GodotFirebase::getFirestore);
    ClassDB::bind_method(D_METHOD("functions"), &GodotFirebase::getFunctions);
    ClassDB::bind_method(D_METHOD("messaging"), &GodotFirebase::getMessaging);
    ClassDB::bind_method(D_METHOD("storage"), &GodotFirebase::getStorage);
}
