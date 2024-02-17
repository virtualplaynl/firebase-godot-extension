namespace Firebase;

using System;
using Godot;

public sealed class DeleteUserTask : IDisposable
{
    private GodotObject taskObject;

    public DeleteUserTask(GodotObject godotObject) => Set(godotObject);
    public DeleteUserTask(Action onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public DeleteUserTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("delete_user_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("delete_user_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Auth.Error, string> Error;
}
