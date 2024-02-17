#include "GodotFirebaseFunctions.h"

#include "conversion.h"

using namespace godot;
using namespace firebase;
using namespace firebase::functions;

GodotFirebaseFunctions::GodotFirebaseFunctions()
{
    cache = Dictionary();
}

GodotFirebaseFunctions::~GodotFirebaseFunctions()
{
}

void GodotFirebaseFunctions::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Firebase Functions...");
    functions = Functions::GetInstance(app);
}

FirebaseFunction *GodotFirebaseFunctions::prepare(String name, SceneTree *tree)
{
    if (cache.has(name))
        return (FirebaseFunction *)(GodotObject *)cache[name];

    FirebaseFunction *callable = memnew(FirebaseFunction);
    callable->set_name("Firebase Function '" + name + "'");
    tree->get_root()->call_deferred("add_child", callable);

    HttpsCallableReference function = functions->GetHttpsCallable(name.utf8().get_data());
    if (function.is_valid())
    {
        callable->setCallable(function, name);
        cache[name] = callable;
        return callable;
    }
    else
    {
        UtilityFunctions::push_error("Function call to '" + name + "' not valid");
        return nullptr;
    }
}

void GodotFirebaseFunctions::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("prepare", "name", "scene_tree"), &GodotFirebaseFunctions::prepare);
}
