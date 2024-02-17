#include "GodotFirebaseRemoteConfig.h"

using namespace godot;
using namespace firebase;
using namespace firebase::remote_config;

// GFB_CALLBACK1_CPP(GetFCMTokenTask, get_fcm_token, String, STRING, token);

GodotFirebaseRemoteConfig::GodotFirebaseRemoteConfig()
{
}

GodotFirebaseRemoteConfig::~GodotFirebaseRemoteConfig()
{
}

void GodotFirebaseRemoteConfig::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Remote Config...");
    remoteConfig = RemoteConfig::GetInstance(app);
}

void GodotFirebaseRemoteConfig::_bind_methods()
{
    // ClassDB::bind_method(D_METHOD("request_permission"), &GodotFirebaseRemoteConfig::requestPermission);

    // ADD_SIGNAL(MethodInfo("message_received", PropertyInfo(Variant::DICTIONARY, "message")));
}
