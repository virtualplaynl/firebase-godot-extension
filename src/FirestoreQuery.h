#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"
#include "FirestoreFilter.h"

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    class FirestoreQuery : public Object
    {
        GDCLASS(FirestoreQuery, Object);

    protected:
        static void _bind_methods();
        firestore::Query query;
        String queryString;

    public:
        FirestoreQuery();
        ~FirestoreQuery();
        String toString();

        void setQuery(firestore::Query query, String string);

        void execute();

        void count();
        void endAt(Array values);
        void endBefore(Array values);
        void limit(int limit);
        void limitToLast(int limit);
        void orderBy(Array fieldPath, Direction direction);
        void startAfter(Array values);
        void startAt(Array values);
        void where(FirestoreFilter *filter);
        void whereArrayContains(String field, Variant value);
        void whereArrayContainsAny(String field, Array values);
        void whereEqualTo(String field, Variant value);
        void whereGreaterThan(String field, Variant value);
        void whereGreaterThanOrEqualTo(String field, Variant value);
        void whereIn(String field, Array values);
        void whereLessThan(String field, Variant value);
        void whereLessThanOrEqualTo(String field, Variant value);
        void whereNotEqualTo(String field, Variant value);
        void whereNotIn(String field, Array values);
    };
}
