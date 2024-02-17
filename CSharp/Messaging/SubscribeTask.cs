namespace Firebase;

using System;
using Godot;

public sealed class SubscribeTask : IDisposable
{
    private GodotObject taskObject;

    public SubscribeTask(GodotObject godotObject) => Set(godotObject);
    public SubscribeTask(Action<string> onCompleted, Action<Messaging.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public SubscribeTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("subscribe_completed",
            Callable.From<string>((topic) => Completed?.Invoke(topic)));
        godotObject.Connect("subscribe_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Messaging.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Messaging.Error, string> Error;
}
