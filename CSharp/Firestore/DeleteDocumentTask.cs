namespace Firebase;

using System;
using Godot;

public sealed class DeleteDocumentTask : IDisposable
{
    private GodotObject taskObject;

    public DeleteDocumentTask(GodotObject godotObject) => Set(godotObject);
    public DeleteDocumentTask(Action onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public DeleteDocumentTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("delete_document_completed",
            Callable.From(() => Completed?.Invoke()));
        godotObject.Connect("delete_document_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action Completed;
    public event Action<Firestore.Error, string> Error;
}
