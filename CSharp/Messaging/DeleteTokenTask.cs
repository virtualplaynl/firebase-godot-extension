namespace Firebase;

using System;
using Godot;

public sealed class DeleteTokenTask : IDisposable
{
    private GodotObject taskObject;

    public DeleteTokenTask(GodotObject godotObject) => Set(godotObject);
    public DeleteTokenTask(Action onCompleted, Action<Messaging.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public DeleteTokenTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("delete_token_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("delete_token_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Messaging.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Messaging.Error, string> Error;
}
