#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>

#include "conversion.h"

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    class FirestoreFilter : public Object
    {
        GDCLASS(FirestoreFilter, Object);

    protected:
        static void _bind_methods();
        Filter *filter;
        String filterString;

    public:
        FirestoreFilter();
        ~FirestoreFilter();

        Filter getFilter();
        void setFilter(Filter filter, String string);
        String getString();

        static FirestoreFilter *andAll(Array filters);
        static FirestoreFilter *orAny(Array filters);
        static FirestoreFilter *arrayContains(String field, Variant value);
        static FirestoreFilter *arrayContainsAny(String field, Array values);
        static FirestoreFilter *equalTo(String field, Variant value);
        static FirestoreFilter *greaterThan(String field, Variant value);
        static FirestoreFilter *greaterThanOrEqualTo(String field, Variant value);
        static FirestoreFilter *in(String field, Array values);
        static FirestoreFilter *lessThan(String field, Variant value);
        static FirestoreFilter *lessThanOrEqualTo(String field, Variant value);
        static FirestoreFilter *notEqualTo(String field, Variant value);
        static FirestoreFilter *notIn(String field, Array values);
    };
}