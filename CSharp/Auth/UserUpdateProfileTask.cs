namespace Firebase;

using System;
using Godot;

public sealed class UserUpdateProfileTask : IDisposable
{
    private GodotObject taskObject;

    public UserUpdateProfileTask(GodotObject godotObject) => Set(godotObject);
    public UserUpdateProfileTask(Action<string, string> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserUpdateProfileTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_update_profile_completed",
            Callable.From<string, string>((displayName, photoUrl) => Completed?.Invoke(displayName, photoUrl)));
        godotObject.Connect("user_update_profile_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string, string> Completed;
    public event Action<Auth.Error, string> Error;
}
