namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class UserReauthenticateTask : IDisposable
{
    private GodotObject taskObject;

    public UserReauthenticateTask(GodotObject godotObject) => Set(godotObject);
    public UserReauthenticateTask(Action<User> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserReauthenticateTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_reauthenticate_completed",
            Callable.From<Dictionary>((userData) => Completed?.Invoke(new(userData))));
        godotObject.Connect("user_reauthenticate_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<User> Completed;
    public event Action<Auth.Error, string> Error;
}
