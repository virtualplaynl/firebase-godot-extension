namespace Firebase;

using System;
using Godot;

public sealed class UserReloadTask : IDisposable
{
    private GodotObject taskObject;

    public UserReloadTask(GodotObject godotObject) => Set(godotObject);
    public UserReloadTask(Action onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserReloadTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_reload_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("user_reload_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Auth.Error, string> Error;
}
