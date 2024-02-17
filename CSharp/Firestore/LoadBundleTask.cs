namespace Firebase;

using System;
using Godot;

public sealed class LoadBundleTask : IDisposable
{
    private GodotObject taskObject;

    public LoadBundleTask(GodotObject godotObject) => Set(godotObject);
    public LoadBundleTask(Action<int, int, int, int> onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public LoadBundleTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("load_bundle_completed",
            Callable.From<int, int, int, int>((bytes_loaded, documents_loaded, total_bytes, total_documents) => Completed?.Invoke(bytes_loaded, documents_loaded, total_bytes, total_documents)));
        godotObject.Connect("load_bundle_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<int, int, int, int> Completed;
    public event Action<Firestore.Error, string> Error;
}
