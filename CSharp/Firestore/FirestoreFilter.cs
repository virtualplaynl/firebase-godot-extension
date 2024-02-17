namespace Firebase;

using System;
using Godot;

public sealed class FirestoreFilter : IDisposable
{
    public GodotObject Filter { get; }
    private static readonly StringName ClassName = "FirestoreFilter";

    public FirestoreFilter(GodotObject godotObject)
        => Filter = godotObject;

    public void Dispose() => Filter.Free();

    public override string ToString() => (string)Filter.Call("get_string");

    public static Variant CallStatic(StringName method, params Variant[] args)
    {
        GodotObject hook = (GodotObject)ClassDB.Instantiate(ClassName);
        Variant result = hook.Call(method, args);
        hook.Free();
        return result;
    }

    public static FirestoreFilter And(Godot.Collections.Array filters)
        => new((GodotObject)CallStatic("and", filters));
    public static FirestoreFilter Or(Godot.Collections.Array filters)
        => new((GodotObject)CallStatic("or", filters));
    public static FirestoreFilter ArrayContains(string field, Variant value)
        => new((GodotObject)CallStatic("array_contains", field, value));
    public static FirestoreFilter ArrayContainsAny(string field, Godot.Collections.Array values)
        => new((GodotObject)CallStatic("array_contains_any", field, values));
    public static FirestoreFilter EqualTo(string field, Variant value)
        => new((GodotObject)CallStatic("equal_to", field, value));
    public static FirestoreFilter GreaterThan(string field, Variant value)
        => new((GodotObject)CallStatic("greater_than", field, value));
    public static FirestoreFilter GreaterThanOrEqualTo(string field, Variant value)
        => new((GodotObject)CallStatic("greater_than_or_equal_to", field, value));
    public static FirestoreFilter In(string field, Godot.Collections.Array values)
        => new((GodotObject)CallStatic("in", field, values));
    public static FirestoreFilter LessThan(string field, Variant value)
        => new((GodotObject)CallStatic("less_than", field, value));
    public static FirestoreFilter LessThanOrEqualTo(string field, Variant value)
        => new((GodotObject)CallStatic("less_than_or_equal_to", field, value));
    public static FirestoreFilter NotEqualTo(string field, Variant value)
        => new((GodotObject)CallStatic("not_equal_to", field, value));
    public static FirestoreFilter NotIn(string field, Godot.Collections.Array values)
        => new((GodotObject)CallStatic("not_in", field, values));
}
