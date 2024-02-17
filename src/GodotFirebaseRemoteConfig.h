#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/remote_config.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::remote_config;

namespace godot
{
    // GFB_CALLBACK1_H(GetFCMTokenTask, String, token);

    class GodotFirebaseRemoteConfig : public Object
    {
        GDCLASS(GodotFirebaseRemoteConfig, Object);

    protected:
        static void _bind_methods();
        RemoteConfig *remoteConfig;

    public:
        GodotFirebaseRemoteConfig();
        ~GodotFirebaseRemoteConfig();

        void init(App *app);
    };
}