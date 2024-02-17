namespace Firebase;

using System;
using Godot;

public sealed class GetDownloadUrlTask : IDisposable
{
    private GodotObject taskObject;

    public GetDownloadUrlTask(GodotObject godotObject) => Set(godotObject);
    public GetDownloadUrlTask(Action<string> onCompleted, Action<Storage.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetDownloadUrlTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_download_url_completed",
            Callable.From<string>((url) => Completed?.Invoke(url)));
        godotObject.Connect("get_download_url_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Storage.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string> Completed;
    public event Action<Storage.Error, string> Error;
}
