#include "FirestoreQuery.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

FirestoreQuery::FirestoreQuery()
{
}

FirestoreQuery::~FirestoreQuery()
{
}

String FirestoreQuery::toString()
{
    return queryString;
}

void FirestoreQuery::setQuery(Query _query, String string)
{
    query = _query;
    queryString = string;
}

void FirestoreQuery::count()
{
    query.Count().Get(AggregateSource::kServer).OnCompletion([this](Future<AggregateQuerySnapshot> completedTask)
    {
        if(completedTask.error() == 0) {
            const AggregateQuerySnapshot *ref = completedTask.result();

            emit_signal("query_count_completed", ref->count());
        }
        else
        {
            emit_signal("query_count_error",
                        completedTask.error(),
                        completedTask.error_message());
        }
    });
}

void FirestoreQuery::endAt(Array values)
{
    query = query.EndAt(arrayToFieldVector(values));
    queryString = queryString + ".end_at(#" + values.size() + ")";
}
void FirestoreQuery::endBefore(Array values)
{
    query = query.EndBefore(arrayToFieldVector(values));
    queryString = queryString + ".end_before(#" + values.size() + ")";
}
void FirestoreQuery::limit(int limit)
{
    query = query.Limit(limit);
    queryString = queryString + ".limit(" + limit + ")";
}
void FirestoreQuery::limitToLast(int limit)
{
    query = query.LimitToLast(limit);
    queryString = queryString + ".limit_to_last(" + limit + ")";
}
void FirestoreQuery::orderBy(Array fieldPath, Direction direction)
{
    query = query.OrderBy(FieldPath(arrayToStringVector(fieldPath)), (Query::Direction)direction);
    queryString = queryString + ".order_by(" + (String)fieldPath[fieldPath.size() - 1] + ")";
}
void FirestoreQuery::startAfter(Array values)
{
    query = query.StartAfter(arrayToFieldVector(values));
    queryString = queryString + ".start_after(#" + values.size() + ")";
}
void FirestoreQuery::startAt(Array values)
{
    query = query.StartAt(arrayToFieldVector(values));
    queryString = queryString + ".start_at(#" + values.size() + ")";
}
void FirestoreQuery::where(FirestoreFilter *filter)
{
    query = query.Where(filter->getFilter());
    queryString = queryString + ".where(" + filter->getString() + ")";
}
void FirestoreQuery::whereArrayContains(String field, Variant value)
{
    query = query.WhereArrayContains(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_array_contains(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereArrayContainsAny(String field, Array values)
{
    query = query.WhereArrayContainsAny(field.utf8().get_data(), arrayToFieldVector(values));
    queryString = queryString + ".where_array_contains_any(" + field + ", #" + values.size() + ")";
}
void FirestoreQuery::whereEqualTo(String field, Variant value)
{
    query = query.WhereEqualTo(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_equal_to(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereGreaterThan(String field, Variant value)
{
    query = query.WhereGreaterThan(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_greater_than(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereGreaterThanOrEqualTo(String field, Variant value)
{
    query = query.WhereGreaterThanOrEqualTo(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_greater_than_or_equal_to(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereIn(String field, Array values)
{
    query = query.WhereIn(field.utf8().get_data(), arrayToFieldVector(values));
    queryString = queryString + ".where_in(" + field + ", #" + values.size() + ")";
}
void FirestoreQuery::whereLessThan(String field, Variant value)
{
    query = query.WhereLessThan(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_less_than(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereLessThanOrEqualTo(String field, Variant value)
{
    query = query.WhereLessThanOrEqualTo(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_less_than_or_equal_to(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereNotEqualTo(String field, Variant value)
{
    query = query.WhereNotEqualTo(field.utf8().get_data(), variantToField(value));
    queryString = queryString + ".where_not_equal_to(" + field + ", " + value.stringify() + ")";
}
void FirestoreQuery::whereNotIn(String field, Array values)
{
    query = query.WhereNotIn(field.utf8().get_data(), arrayToFieldVector(values));
    queryString = queryString + ".where_not_in(" + field + ", #" + values.size() + ")";
}

void FirestoreQuery::execute()
{
    query.Get().OnCompletion([this](Future<QuerySnapshot> completedTask)
    {
        if(completedTask.error() == 0) {
            std::pair<Array, Array> snapshotElements = querySnapshotToArrays(*completedTask.result());

            emit_signal("query_completed", snapshotElements.first, snapshotElements.second);
        }
        else
        {
            emit_signal("query_error",
                        completedTask.error(),
                        completedTask.error_message());
        }
    });
}

void FirestoreQuery::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("to_string"), &FirestoreQuery::toString);
    ClassDB::bind_method(D_METHOD("execute"), &FirestoreQuery::execute);
    ClassDB::bind_method(D_METHOD("count"), &FirestoreQuery::count);
    ClassDB::bind_method(D_METHOD("end_at", "values"), &FirestoreQuery::endAt);
    ClassDB::bind_method(D_METHOD("end_before", "values"), &FirestoreQuery::endBefore);
    ClassDB::bind_method(D_METHOD("limit", "limit"), &FirestoreQuery::limit);
    ClassDB::bind_method(D_METHOD("limit_to_last", "limit"), &FirestoreQuery::limitToLast);
    ClassDB::bind_method(D_METHOD("order_by", "fieldPath", "direction"), &FirestoreQuery::orderBy);
    ClassDB::bind_method(D_METHOD("start_after", "values"), &FirestoreQuery::startAfter);
    ClassDB::bind_method(D_METHOD("start_at", "values"), &FirestoreQuery::startAt);
    ClassDB::bind_method(D_METHOD("where", "filter"), &FirestoreQuery::where);
    ClassDB::bind_method(D_METHOD("where_array_contains", "field", "value"), &FirestoreQuery::whereArrayContains);
    ClassDB::bind_method(D_METHOD("where_array_contains_any", "field", "values"), &FirestoreQuery::whereArrayContainsAny);
    ClassDB::bind_method(D_METHOD("where_equal_to", "field", "value"), &FirestoreQuery::whereEqualTo);
    ClassDB::bind_method(D_METHOD("where_greater_Than", "field", "value"), &FirestoreQuery::whereGreaterThan);
    ClassDB::bind_method(D_METHOD("where_greater_than_or_equal_to", "field", "value"), &FirestoreQuery::whereGreaterThanOrEqualTo);
    ClassDB::bind_method(D_METHOD("where_in", "field", "values"), &FirestoreQuery::whereIn);
    ClassDB::bind_method(D_METHOD("where_less_than", "field", "value"), &FirestoreQuery::whereLessThan);
    ClassDB::bind_method(D_METHOD("where_less_than_or_equal_to", "field", "value"), &FirestoreQuery::whereLessThanOrEqualTo);
    ClassDB::bind_method(D_METHOD("where_not_equal_to", "field", "value"), &FirestoreQuery::whereNotEqualTo);
    ClassDB::bind_method(D_METHOD("where_not_in", "field", "values"), &FirestoreQuery::whereNotIn);

    ADD_SIGNAL(MethodInfo("query_count_completed", PropertyInfo(Variant::INT, "count")));
    ADD_SIGNAL(MethodInfo("query_count_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
    ADD_SIGNAL(MethodInfo("query_completed", PropertyInfo(Variant::ARRAY, "changes"), PropertyInfo(Variant::ARRAY, "documents")));
    ADD_SIGNAL(MethodInfo("query_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
}