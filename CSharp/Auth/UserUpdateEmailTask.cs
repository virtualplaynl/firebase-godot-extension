namespace Firebase;

using System;
using Godot;

public sealed class UserUpdateEmailTask : IDisposable
{
    private GodotObject taskObject;

    public UserUpdateEmailTask(GodotObject godotObject) => Set(godotObject);
    public UserUpdateEmailTask(Action<string> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserUpdateEmailTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_update_email_completed",
            Callable.From<string>((email) => Completed?.Invoke(email)));
        godotObject.Connect("user_update_email_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Auth.Error, string> Error;
}
