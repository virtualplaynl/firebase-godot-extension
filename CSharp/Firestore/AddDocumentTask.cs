namespace Firebase;

using System;
using Godot;

public sealed class AddDocumentTask : IDisposable
{
    private GodotObject taskObject;

    public AddDocumentTask(GodotObject godotObject) => Set(godotObject);
    public AddDocumentTask(Action onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public AddDocumentTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("add_document_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("add_document_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Firestore.Error, string> Error;
}
