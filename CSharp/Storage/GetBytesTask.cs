namespace Firebase;

using System;
using Godot;

public sealed class GetBytesTask : IDisposable
{
    private GodotObject taskObject;

    public GetBytesTask(GodotObject godotObject) => Set(godotObject);
    public GetBytesTask(Action<long> onCompleted, Action<Storage.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetBytesTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_bytes_completed",
            Callable.From<long>((bytesRead) => Completed?.Invoke(bytesRead)));
        godotObject.Connect("get_bytes_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Storage.Error)error, errorMessage)));
        godotObject.Connect("get_bytes_paused",
            Callable.From(() => Paused?.Invoke()));
        godotObject.Connect("get_bytes_progress",
            Callable.From(() => Progress?.Invoke()));
        return this;
    }

    public ulong BytesTransferred() => (ulong)taskObject.Call("bytes_transferred");
    public ulong TotalByteCount() => (ulong)taskObject.Call("total_byte_count");
    public bool Cancel() => (bool)taskObject.Call("cancel");
    public bool Pause() => (bool)taskObject.Call("pause");
    public bool Resume() => (bool)taskObject.Call("resume");

    public void Dispose() => taskObject.Free();


    public event Action<long> Completed;
    public event Action Paused;
    public event Action Progress;
    public event Action<Storage.Error, string> Error;
}
