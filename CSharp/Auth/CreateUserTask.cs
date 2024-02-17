namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class CreateUserTask : IDisposable
{
    private GodotObject taskObject;

    public CreateUserTask(GodotObject godotObject) => Set(godotObject);
    public CreateUserTask(Action<User> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public CreateUserTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("create_user_completed",
            Callable.From<Dictionary>((userData) => Completed?.Invoke(new(userData))));
        godotObject.Connect("create_user_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<User> Completed;
    public event Action<Auth.Error, string> Error;
}
