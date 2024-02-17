namespace Firebase;

using System;
using Godot;

public sealed class CollectionExistsTask : IDisposable
{
    private GodotObject taskObject;

    public CollectionExistsTask(GodotObject godotObject) => Set(godotObject);
    public CollectionExistsTask(Action<int> onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public CollectionExistsTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("collection_exists_completed",
            Callable.From<int>((documentCount) => Completed?.Invoke(documentCount)));
        godotObject.Connect("collection_exists_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<int> Completed;
    public event Action<Firestore.Error, string> Error;
}
