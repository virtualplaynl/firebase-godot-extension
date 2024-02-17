#include "GodotFirebaseDatabase.h"

using namespace godot;
using namespace firebase;
using namespace firebase::database;

GodotFirebaseDatabase::GodotFirebaseDatabase()
{
    database = nullptr;
}

GodotFirebaseDatabase::~GodotFirebaseDatabase()
{
}

void GodotFirebaseDatabase::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Firebase Realtime Database...");
    database = Database::GetInstance(app);
}

void GodotFirebaseDatabase::offline()
{
    database->GoOffline();
}

void GodotFirebaseDatabase::online()
{
    database->GoOnline();
}

void GodotFirebaseDatabase::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("offline"), &GodotFirebaseDatabase::offline);
    ClassDB::bind_method(D_METHOD("online"), &GodotFirebaseDatabase::online);
}
