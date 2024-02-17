#include "GodotFirebaseAnalytics.h"

using namespace godot;
using namespace firebase;
using namespace firebase::analytics;

GFB_CALLBACK1_CPP(GetAnalyticsInstanceIdTask, get_instance_id, String, STRING, instance_id);
GFB_CALLBACK1_CPP(GetSessionIdTask, get_session_id, int64_t, INT, session_id);

GodotFirebaseAnalytics::GodotFirebaseAnalytics()
{
}

GodotFirebaseAnalytics::~GodotFirebaseAnalytics()
{
    terminate();
}

void GodotFirebaseAnalytics::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Firebase Analytics...");
    Initialize(*app);
}

GetAnalyticsInstanceIdTask *GodotFirebaseAnalytics::getAnalyticsInstanceId()
{
    GetAnalyticsInstanceIdTask *task = memnew(GetAnalyticsInstanceIdTask);
    GetAnalyticsInstanceId()
        .OnCompletion([task](const Future<std::string> &completedTask)
                      {
            if(completedTask.error() == 0)
                task->completed(completedTask.result()->c_str());
            else
                task->error(completedTask.error(), completedTask.error_message()); });
    return task;
}
GetSessionIdTask *GodotFirebaseAnalytics::getSessionId()
{
    GetSessionIdTask *task = memnew(GetSessionIdTask);
    GetSessionId()
        .OnCompletion([task](const Future<int64_t> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed(*completedTask.result());
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
void GodotFirebaseAnalytics::logEvent(String name, Dictionary parameters)
{
    if (parameters.size() == 0)
    {
        LogEvent(name.utf8().get_data());
    }
    else
    {
        Array keys = parameters.keys();
        Parameter params[keys.size()];
        for (size_t i = 0; i < keys.size(); i++)
        {
            params[i] = Parameter(((String)keys[i]).utf8().get_data(), variantToFBVariant(parameters[keys[i]]));
        }
        LogEvent(name.utf8().get_data(), params, keys.size());
    }
}
void GodotFirebaseAnalytics::resetAnalyticsData()
{
    ResetAnalyticsData();
}
void GodotFirebaseAnalytics::setAnalyticsCollectionEnabled(bool enabled)
{
    SetAnalyticsCollectionEnabled(enabled);
}
void GodotFirebaseAnalytics::setAdStorageConsent(bool granted)
{
    std::map<ConsentType, ConsentStatus> map = std::map<ConsentType, ConsentStatus>();
    map[ConsentType::kConsentTypeAdStorage] = granted ? ConsentStatus::kConsentStatusGranted : ConsentStatus::kConsentStatusDenied;
    SetConsent(map);
}
void GodotFirebaseAnalytics::setAnalyticsStorageConsent(bool granted)
{
    std::map<ConsentType, ConsentStatus> map = std::map<ConsentType, ConsentStatus>();
    map[ConsentType::kConsentTypeAnalyticsStorage] = granted ? ConsentStatus::kConsentStatusGranted : ConsentStatus::kConsentStatusDenied;
    SetConsent(map);
}
void GodotFirebaseAnalytics::setSessionTimeoutDuration(int64_t milliseconds)
{
    SetSessionTimeoutDuration(milliseconds);
}
void GodotFirebaseAnalytics::setUserId(String userId)
{
    SetUserId(userId.utf8().get_data());
}
void GodotFirebaseAnalytics::setUserProperty(String name, String property)
{
    SetUserProperty(name.utf8().get_data(), property.utf8().get_data());
}
void GodotFirebaseAnalytics::terminate()
{
    Terminate();
}

void GodotFirebaseAnalytics::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_analytics_instance_id"), &GodotFirebaseAnalytics::getAnalyticsInstanceId);
    ClassDB::bind_method(D_METHOD("get_session_id"), &GodotFirebaseAnalytics::getSessionId);
    ClassDB::bind_method(D_METHOD("log_event", "name", "parameters"), &GodotFirebaseAnalytics::logEvent);
    ClassDB::bind_method(D_METHOD("reset_analytics_data"), &GodotFirebaseAnalytics::resetAnalyticsData);
    ClassDB::bind_method(D_METHOD("set_analytics_collection_enabled", "enabled"), &GodotFirebaseAnalytics::setAnalyticsCollectionEnabled);
    ClassDB::bind_method(D_METHOD("set_ad_storage_consent", "granted"), &GodotFirebaseAnalytics::setAdStorageConsent);
    ClassDB::bind_method(D_METHOD("set_analytics_storage_consent", "granted"), &GodotFirebaseAnalytics::setAnalyticsStorageConsent);
    ClassDB::bind_method(D_METHOD("set_session_timeout_duration", "milliseconds"), &GodotFirebaseAnalytics::setSessionTimeoutDuration);
    ClassDB::bind_method(D_METHOD("set_user_id", "user_id"), &GodotFirebaseAnalytics::setUserId);
    ClassDB::bind_method(D_METHOD("set_user_property", "name", "property"), &GodotFirebaseAnalytics::setUserProperty);
    ClassDB::bind_method(D_METHOD("terminate"), &GodotFirebaseAnalytics::terminate);
}