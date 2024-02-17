namespace Firebase;

using System;
using Godot;

public sealed class UnsubscribeTask : IDisposable
{
    private GodotObject taskObject;

    public UnsubscribeTask(GodotObject godotObject) => Set(godotObject);
    public UnsubscribeTask(Action<string> onCompleted, Action<Messaging.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UnsubscribeTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("unsubscribe_completed",
            Callable.From<string>((topic) => Completed?.Invoke(topic)));
        godotObject.Connect("unsubscribe_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Messaging.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Messaging.Error, string> Error;
}
