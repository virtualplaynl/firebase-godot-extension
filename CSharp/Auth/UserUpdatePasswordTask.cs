namespace Firebase;

using System;
using Godot;

public sealed class UserUpdatePasswordTask : IDisposable
{
    private GodotObject taskObject;

    public UserUpdatePasswordTask(GodotObject godotObject) => Set(godotObject);
    public UserUpdatePasswordTask(Action<string> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserUpdatePasswordTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_update_password_completed",
            Callable.From<string>((password) => Completed?.Invoke(password)));
        godotObject.Connect("user_update_password_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Auth.Error, string> Error;
}
