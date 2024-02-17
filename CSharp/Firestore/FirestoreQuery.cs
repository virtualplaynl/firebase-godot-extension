namespace Firebase;

using System;
using Godot;

public sealed class FirestoreQuery : IDisposable
{
    private readonly GodotObject queryObject;

    public override string ToString() => (string)queryObject.Call("to_string");

    public FirestoreQuery(GodotObject godotObject)
    {
        queryObject = godotObject;

        queryObject.Connect("query_count_completed",
            Callable.From<int>((count) => QueryCountCompleted?.Invoke(count)));
        queryObject.Connect("query_count_error",
            Callable.From<int, string>((error, errorMessage) => QueryCountError?.Invoke((Firestore.Error)error, errorMessage)));
        queryObject.Connect("query_completed",
            Callable.From<Godot.Collections.Array, Godot.Collections.Array>((changes, documents) => QueryCompleted?.Invoke(changes, documents)));
        queryObject.Connect("query_error",
            Callable.From<int, string>((error, errorMessage) => QueryError?.Invoke((Firestore.Error)error, errorMessage)));
    }

    public void Dispose() => queryObject.Free();

    public void Execute() => queryObject.Call("execute");

    public void Count() => queryObject.Call("count");
    public void EndAt(Godot.Collections.Array values) => queryObject.Call("end_at", "values", values);
    public void EndBefore(Godot.Collections.Array values) => queryObject.Call("end_before", "values", values);
    public void Limit(int limit) => queryObject.Call("limit", "limit", limit);
    public void LimitToLast(int limit) => queryObject.Call("limit_to_last", "limit", limit);
    public void OrderBy(Godot.Collections.Array fieldPath, Direction direction) => queryObject.Call("order_by", fieldPath, (int)direction);
    public void StartAfter(Godot.Collections.Array values) => queryObject.Call("start_after", "values", values);
    public void StartAt(Godot.Collections.Array values) => queryObject.Call("start_at", "values", values);
    public void Where(FirestoreFilter filter) => queryObject.Call("where", "filter", filter.Filter);
    public void WhereArrayContains(string field, Variant value) => queryObject.Call("where_array_contains", field, value);
    public void WhereArrayContainsAny(string field, Godot.Collections.Array values) => queryObject.Call("where_array_contains_any", field, values);
    public void WhereEqualTo(string field, Variant value) => queryObject.Call("where_equal_to", field, value);
    public void WhereGreaterThan(string field, Variant value) => queryObject.Call("where_greater_than", field, value);
    public void WhereGreaterThanOrEqualTo(string field, Variant value) => queryObject.Call("where_greater_than_or_equal_to", field, value);
    public void WhereIn(string field, Godot.Collections.Array values) => queryObject.Call("where_in", field, values);
    public void WhereLessThan(string field, Variant value) => queryObject.Call("where_less_than", field, value);
    public void WhereLessThanOrEqualTo(string field, Variant value) => queryObject.Call("where_less_than_or_equal_to", field, value);
    public void WhereNotEqualTo(string field, Variant value) => queryObject.Call("where_not_equal_to", field, value);
    public void WhereNotIn(string field, Godot.Collections.Array values) => queryObject.Call("where_not_in", field, values);

    public event Action<int> QueryCountCompleted;
    public event Action<Firestore.Error, string> QueryCountError;
    public event Action<Godot.Collections.Array, Godot.Collections.Array> QueryCompleted;
    public event Action<Firestore.Error, string> QueryError;
}
