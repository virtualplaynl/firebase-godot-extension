namespace Firebase;

using System;
using Godot;

public sealed class SendPasswordResetTask : IDisposable
{
    private GodotObject taskObject;

    public SendPasswordResetTask(GodotObject godotObject) => Set(godotObject);
    public SendPasswordResetTask(Action onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public SendPasswordResetTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("send_password_reset_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("send_password_reset_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Auth.Error, string> Error;
}
