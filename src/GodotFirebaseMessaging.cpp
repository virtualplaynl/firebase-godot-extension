#include "GodotFirebaseMessaging.h"

using namespace godot;
using namespace firebase;
using namespace firebase::messaging;

GFB_CALLBACK1_CPP(GetFCMTokenTask, get_fcm_token, String, STRING, token);
GFB_CALLBACK0_CPP(DeleteTokenTask, delete_token);
GFB_CALLBACK1_CPP(SubscribeTask, subscribe, String, STRING, topic);
GFB_CALLBACK1_CPP(UnsubscribeTask, unsubscribe, String, STRING, topic);

GodotFirebaseMessaging::GodotFirebaseMessaging()
{
}

GodotFirebaseMessaging::~GodotFirebaseMessaging()
{
}

void GodotFirebaseMessaging::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Cloud Messaging...");
    messaging::Initialize(*app, this);
}

void GodotFirebaseMessaging::requestPermission()
{
    messaging::RequestPermission();
}

GetFCMTokenTask *GodotFirebaseMessaging::getToken()
{
    GetFCMTokenTask *task = memnew(GetFCMTokenTask);
    messaging::GetToken()
        .OnCompletion([task](Future<std::string> completedTask)
        {
            if (completedTask.error() == 0)
            {
                task->completed(completedTask.result()->c_str());
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
DeleteTokenTask *GodotFirebaseMessaging::deleteToken()
{
    DeleteTokenTask *task = memnew(DeleteTokenTask);
    messaging::DeleteToken()
        .OnCompletion([task](Future<void> completedTask)
        {
            if (completedTask.error() == 0)
            {
                task->completed();
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
SubscribeTask *GodotFirebaseMessaging::subscribe(String topic)
{
    SubscribeTask *task = memnew(SubscribeTask);
    messaging::Subscribe(topic.utf8().get_data())
        .OnCompletion([task, topic](Future<void> completedTask)
        {
            if (completedTask.error() == 0)
            {
                task->completed(topic);
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UnsubscribeTask *GodotFirebaseMessaging::unsubscribe(String topic)
{
    UnsubscribeTask *task = memnew(UnsubscribeTask);
    messaging::Unsubscribe(topic.utf8().get_data())
        .OnCompletion([task, topic](Future<void> completedTask)
        {
            if (completedTask.error() == 0)
            {
                task->completed(topic);
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
void GodotFirebaseMessaging::terminate()
{
    messaging::Terminate();
}

void GodotFirebaseMessaging::OnMessage(const Message &message)
{
    Dictionary converted = Dictionary();
    converted["collapse_key"] = message.collapse_key.c_str();
    Dictionary data = Dictionary();
    for(auto &entry : message.data)
    {
        data[entry.first.c_str()] = entry.second.c_str();
    }
    converted["data"] = data;
    converted["error"] = message.error.c_str();
    converted["error_description"] = message.error_description.c_str();
    converted["from"] = message.from.c_str();
    converted["link"] = message.link.c_str();
    converted["message_id"] = message.message_id.c_str();
    converted["message_type"] = message.message_type.c_str();
    Dictionary notification = Dictionary();
    if (message.notification != nullptr)
    {
        if (message.notification->android != nullptr)
            notification["channel_id"] = message.notification->android->channel_id.c_str();
        notification["badge"] = message.notification->badge.c_str();
        notification["body"] = message.notification->body.c_str();
        Array body_loc_args = Array();
        for (auto &element : message.notification->body_loc_args)
        {
            body_loc_args.append(element.c_str());
        }
        notification["body_loc_args"] = body_loc_args;
        notification["body_loc_key"] = message.notification->body_loc_key.c_str();
        notification["click_action"] = message.notification->click_action.c_str();
        notification["color"] = message.notification->color.c_str();
        notification["icon"] = message.notification->icon.c_str();
        notification["sound"] = message.notification->sound.c_str();
        notification["tag"] = message.notification->tag.c_str();
        notification["title"] = message.notification->title.c_str();
        Array title_loc_args = Array();
        for (auto &element : message.notification->title_loc_args)
        {
            title_loc_args.append(element.c_str());
        }
        notification["title_loc_args"] = title_loc_args;
        notification["title_loc_key"] = message.notification->title_loc_key.c_str();
        converted["notification"] = notification;
    }
    else
    {
        converted["notification"] = nullptr;
    }
    converted["notification_opened"] = message.notification_opened;
    converted["original_priority"] = message.original_priority.c_str();
    converted["priority"] = message.priority.c_str();
    converted["sent_time"] = message.sent_time;
    converted["time_to_live"] = message.time_to_live;
    converted["to"] = message.to.c_str();

    emit_signal("message_received", converted);
}
void GodotFirebaseMessaging::OnTokenReceived(const char *token)
{
    emit_signal("token_received", token);
}

void GodotFirebaseMessaging::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("request_permission"), &GodotFirebaseMessaging::requestPermission);
    ClassDB::bind_method(D_METHOD("terminate"), &GodotFirebaseMessaging::terminate);

    ClassDB::bind_method(D_METHOD("get_token"), &GodotFirebaseMessaging::getToken);
    ClassDB::bind_method(D_METHOD("delete_token"), &GodotFirebaseMessaging::deleteToken);
    ClassDB::bind_method(D_METHOD("subscribe", "topic"), &GodotFirebaseMessaging::subscribe);
    ClassDB::bind_method(D_METHOD("unsubscribe", "topic"), &GodotFirebaseMessaging::unsubscribe);

    ADD_SIGNAL(MethodInfo("message_received", PropertyInfo(Variant::DICTIONARY, "message")));
    ADD_SIGNAL(MethodInfo("token_received", PropertyInfo(Variant::STRING, "token")));
}
