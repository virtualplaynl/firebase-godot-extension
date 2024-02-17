#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/analytics.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::analytics;

namespace godot
{
    GFB_CALLBACK1_H(GetAnalyticsInstanceIdTask, String, instance_id);
    GFB_CALLBACK1_H(GetSessionIdTask, int64_t, session_id);

    class GodotFirebaseAnalytics : public Object
    {
        GDCLASS(GodotFirebaseAnalytics, Object);

    protected:
        static void _bind_methods();

    public:
        GodotFirebaseAnalytics();
        ~GodotFirebaseAnalytics();

        void init(App *app);

        GetAnalyticsInstanceIdTask *getAnalyticsInstanceId();
        GetSessionIdTask *getSessionId();
        void logEvent(String name, Dictionary parameters);
        void resetAnalyticsData();
        void setAnalyticsCollectionEnabled(bool enabled);
        void setAdStorageConsent(bool granted);
        void setAnalyticsStorageConsent(bool granted);
        void setSessionTimeoutDuration(int64_t milliseconds);
        void setUserId(String userId);
        void setUserProperty(String name, String property);
        void terminate();
    };
}