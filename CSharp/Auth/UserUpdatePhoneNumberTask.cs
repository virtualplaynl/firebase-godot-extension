namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class UserUpdatePhoneNumberTask : IDisposable
{
    private GodotObject taskObject;

    public UserUpdatePhoneNumberTask(GodotObject godotObject) => Set(godotObject);
    public UserUpdatePhoneNumberTask(Action<User> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserUpdatePhoneNumberTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_update_phone_number_completed",
            Callable.From<Dictionary>((userData) => Completed?.Invoke(new(userData))));
        godotObject.Connect("user_update_phone_number_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<User> Completed;
    public event Action<Auth.Error, string> Error;
}
