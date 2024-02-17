namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class PutBytesTask : IDisposable
{
    private GodotObject taskObject;

    public PutBytesTask(GodotObject godotObject) => Set(godotObject);
    public PutBytesTask(Action<Metadata> onCompleted, Action<Storage.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public PutBytesTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_file_completed",
            Callable.From<Dictionary>((metadata) => Completed?.Invoke(new(metadata))));
        godotObject.Connect("get_file_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Storage.Error)error, errorMessage)));
        godotObject.Connect("get_file_paused",
            Callable.From(() => Paused?.Invoke()));
        godotObject.Connect("get_file_progress",
            Callable.From(() => Progress?.Invoke()));
        return this;
    }

    public ulong BytesTransferred() => (ulong)taskObject.Call("bytes_transferred");
    public ulong TotalByteCount() => (ulong)taskObject.Call("total_byte_count");
    public bool Cancel() => (bool)taskObject.Call("cancel");
    public bool Pause() => (bool)taskObject.Call("pause");
    public bool Resume() => (bool)taskObject.Call("resume");

    public void Dispose() => taskObject.Free();


    public event Action<Metadata> Completed;
    public event Action Paused;
    public event Action Progress;
    public event Action<Storage.Error, string> Error;
}
