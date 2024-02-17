namespace Firebase;

using System;
using Godot;

public sealed class StorageDeleteTask : IDisposable
{
    private GodotObject taskObject;

    public StorageDeleteTask(GodotObject godotObject) => Set(godotObject);
    public StorageDeleteTask(Action onCompleted, Action<Storage.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public StorageDeleteTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("storage_delete_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("storage_delete_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Storage.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Storage.Error, string> Error;
}
