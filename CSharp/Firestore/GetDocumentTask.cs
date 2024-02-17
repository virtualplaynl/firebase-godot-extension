namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class GetDocumentTask : IDisposable
{
    private GodotObject taskObject;

    public GetDocumentTask(GodotObject godotObject) => Set(godotObject);
    public GetDocumentTask(Action<Dictionary> onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetDocumentTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_document_completed",
            Callable.From<Dictionary>((document) => Completed?.Invoke(document)));
        godotObject.Connect("get_document_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<Dictionary> Completed;
    public event Action<Firestore.Error, string> Error;
}
