#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/messaging.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::messaging;

namespace godot
{
    GFB_CALLBACK1_H(GetFCMTokenTask, String, token);
    GFB_CALLBACK0_H(DeleteTokenTask);
    GFB_CALLBACK1_H(SubscribeTask, String, topic);
    GFB_CALLBACK1_H(UnsubscribeTask, String, topic);

    class GodotFirebaseMessaging : public Object, firebase::messaging::Listener
    {
        GDCLASS(GodotFirebaseMessaging, Object);

    protected:
        static void _bind_methods();

    public:
        GodotFirebaseMessaging();
        ~GodotFirebaseMessaging();

        void init(App *app);
        void requestPermission();

        GetFCMTokenTask *getToken();
        DeleteTokenTask *deleteToken();
        SubscribeTask *subscribe(String topic);
        UnsubscribeTask *unsubscribe(String topic);
        void terminate();

        void OnMessage(const Message &message) override;
        void OnTokenReceived(const char *token) override;
    };
}