namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class FirestoreBatch : IDisposable
{
    private readonly GodotObject batchObject;

    public FirestoreBatch(GodotObject godotObject)
    {
        batchObject = godotObject;

        batchObject.Connect("commit_completed",
            Callable.From(() => CommitCompleted?.Invoke()));
        batchObject.Connect("commit_error",
            Callable.From<int, string>((error, errorMessage) => CommitError?.Invoke((Firestore.Error)error, errorMessage)));
    }

    public void Dispose() => batchObject.Free();

    public void AddDelete(string docPath) => batchObject.Call("add_delete", docPath);
    public void AddSet(string docPath, Dictionary fields, SetOptions setOptions, Godot.Collections.Array mergefields) => batchObject.Call("add_set", docPath, fields, (int)setOptions, mergefields);
    public void AddUpdate(string docPath, Dictionary fields) => batchObject.Call("add_update", docPath, fields);
    public bool IsValid() => (bool)batchObject.Call("is_valid");
    public int Commit() => (int)batchObject.Call("commit");

    public event Action CommitCompleted;
    public event Action<Firestore.Error, string> CommitError;
}
