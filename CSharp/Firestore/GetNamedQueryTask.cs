namespace Firebase;

using System;
using Godot;

public sealed class GetNamedQueryTask : IDisposable
{
    private GodotObject taskObject;

    public GetNamedQueryTask(GodotObject godotObject) => Set(godotObject);
    public GetNamedQueryTask(Action<FirestoreQuery> onCompleted, Action<Firestore.Error, string> onError)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;
    }
    public GetNamedQueryTask Set(GodotObject godotObject)
    {
        taskObject = godotObject;

        godotObject.Connect("get_named_query_completed",
            Callable.From<GodotObject>((query) => Completed?.Invoke(new FirestoreQuery(query))));
        godotObject.Connect("get_named_query_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Firestore.Error)error, errorMessage)));
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<FirestoreQuery> Completed;
    public event Action<Firestore.Error, string> Error;
}
