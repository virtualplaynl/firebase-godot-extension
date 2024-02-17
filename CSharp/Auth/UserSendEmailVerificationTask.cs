namespace Firebase;

using System;
using Godot;

public sealed class UserSendEmailVerificationTask : IDisposable
{
    private GodotObject taskObject;

    public UserSendEmailVerificationTask(GodotObject godotObject) => Set(godotObject);
    public UserSendEmailVerificationTask(Action onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public UserSendEmailVerificationTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("user_send_email_verification_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("user_send_email_verification_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Auth.Error, string> Error;
}
