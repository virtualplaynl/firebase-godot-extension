namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class SignInTask : IDisposable
{
    private GodotObject taskObject;

    public SignInTask(GodotObject godotObject) => Set(godotObject);
    public SignInTask(Action<User> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public SignInTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("sign_in_completed",
            Callable.From<Dictionary>((userData) => Completed?.Invoke(new(userData))));
        godotObject.Connect("sign_in_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<User> Completed;
    public event Action<Auth.Error, string> Error;
}
