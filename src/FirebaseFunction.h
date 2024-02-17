#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <firebase/app.h>
#include <firebase/functions.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::functions;

namespace godot
{
    class FirebaseFunction : public Node
    {
        GDCLASS(FirebaseFunction, Node);

    protected:
        static void _bind_methods();
        FirebaseFunction *forward_to;
        HttpsCallableReference callable;
        String functionName;
        Future<HttpsCallableResult> waitTask;

    public:
        FirebaseFunction();
        ~FirebaseFunction();

        void completed(String name, Dictionary result);
        void error(int error, String error_message);

        void forward(FirebaseFunction *to_task);
        void setCallable(HttpsCallableReference function, String name);
        void call(Dictionary args);
        void _process(double delta) override;
    };
}