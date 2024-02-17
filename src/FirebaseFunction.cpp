#include "GodotFirebaseFunctions.h"

#include "conversion.h"

using namespace godot;
using namespace firebase;
using namespace firebase::functions;

FirebaseFunction::FirebaseFunction()
{
    forward_to = nullptr;
    set_process(false);
}
FirebaseFunction::~FirebaseFunction() {}

void FirebaseFunction::completed(String name, Dictionary result)
{
    (forward_to == nullptr ? this : forward_to)->emit_signal("function" "_completed", name, result);
}

void FirebaseFunction::error(int error, String error_message)
{
    (forward_to == nullptr ? this : forward_to)->emit_signal("function" "_error", error, error_message);
}

void FirebaseFunction::forward(FirebaseFunction *to_task)
{
    forward_to = to_task;
}

void FirebaseFunction::setCallable(HttpsCallableReference function, String name)
{
    callable = function;
    functionName = name;
}

void FirebaseFunction::call(Dictionary args)
{
    waitTask = callable.Call(dictToVariantMap(args));
    set_process(true);
}

void FirebaseFunction::_process(double delta)
{
    if (waitTask.status() != firebase::kFutureStatusPending)
    {
        if (waitTask.error() == 0)
        {
            godot::Variant data = fbVariantToVariant(waitTask.result()->data());
            if (data.get_type() == Variant::Type::DICTIONARY)
                completed(functionName, (Dictionary)data);
            else
                error(-1, "Function '" + functionName + "' result is not a dictionary: " + data.stringify());
        }
        else
        {
            error(waitTask.error(), waitTask.error_message());
        }

        set_process(false);
    }
}

void FirebaseFunction::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("call", "args"), &FirebaseFunction::call);

    godot::ClassDB::add_signal(get_class_static(), MethodInfo("function_completed", PropertyInfo(Variant::STRING, "m_arg1"), PropertyInfo(Variant::DICTIONARY, "m_arg2")));
    godot::ClassDB::add_signal(get_class_static(), MethodInfo("function_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
}
