namespace Firebase;

using System;
using Godot;

public sealed class DocumentExistsTask : IDisposable
{
    private GodotObject taskObject;

    public DocumentExistsTask(GodotObject godotObject) => Set(godotObject);
    public DocumentExistsTask(Action<bool> onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public DocumentExistsTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("document_exists_completed",
            Callable.From<bool>((exists) => Completed?.Invoke(exists)));
        godotObject.Connect("document_exists_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<bool> Completed;
    public event Action<Firestore.Error, string> Error;
}
