namespace Firebase;

using System;
using Godot;

public sealed class GetAuthTokenTask : IDisposable
{
    private GodotObject taskObject;

    public GetAuthTokenTask(GodotObject godotObject) => Set(godotObject);
    public GetAuthTokenTask(Action<string> onCompleted, Action<Auth.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetAuthTokenTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_auth_token_completed",
            Callable.From<string>((token) => Completed?.Invoke(token)));
        godotObject.Connect("get_auth_token_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Auth.Error)error, errorMessage)));

        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Auth.Error, string> Error;
}
