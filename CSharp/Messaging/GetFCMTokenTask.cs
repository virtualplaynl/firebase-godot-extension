namespace Firebase;

using System;
using Godot;

public sealed class GetFCMTokenTask : IDisposable
{
    private GodotObject taskObject;

    public GetFCMTokenTask(GodotObject godotObject) => Set(godotObject);
    public GetFCMTokenTask(Action<string> onCompleted, Action<Messaging.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetFCMTokenTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_fcm_token_completed",
            Callable.From<string>((token) => Completed?.Invoke(token)));
        godotObject.Connect("get_fcm_token_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Messaging.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Messaging.Error, string> Error;
}
