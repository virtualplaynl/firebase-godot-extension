namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class FirestoreTransaction : IDisposable
{
    public GodotObject Transaction { get; }

    public FirestoreTransaction(GodotObject godotObject)
        => Transaction = godotObject;

    public void Dispose() => Transaction.Free();

    public void AddDelete(string docPath) => Transaction.Call("add_delete", docPath);
    public void AddGet(string docPath) => Transaction.Call("add_get", docPath);
    public void AddSet(string docPath, Dictionary fields, SetOptions setOptions, Godot.Collections.Array mergefields) => Transaction.Call("add_set", docPath, fields, (int)setOptions, mergefields);
    public void AddUpdate(string docPath, Dictionary fields) => Transaction.Call("add_update", docPath, fields);
}
