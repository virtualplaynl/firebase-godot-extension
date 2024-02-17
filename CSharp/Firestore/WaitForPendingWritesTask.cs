namespace Firebase;

using System;
using Godot;

public sealed class WaitForPendingWritesTask : IDisposable
{
    private GodotObject taskObject;

    public WaitForPendingWritesTask(GodotObject godotObject) => Set(godotObject);
    public WaitForPendingWritesTask(Action onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public WaitForPendingWritesTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("wait_for_pending_writes_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("wait_for_pending_writes_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Firestore.Error, string> Error;
}
