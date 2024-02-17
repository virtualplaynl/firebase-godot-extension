namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class FirebaseFunction : IDisposable
{
    private readonly GodotObject taskObject;

    public FirebaseFunction(SceneTree tree, string name, Action<string, Dictionary> onCompleted = null, Action<Functions.Error, string> onError = null)
    {
        if (onCompleted != null) Completed += onCompleted;
        if (onError != null) Error += onError;

        taskObject = (GodotObject)Firebase.FunctionsObject.Call("prepare", name, tree);

        taskObject.Connect("function_completed",
            Callable.From<string, Dictionary>((name, result) => Completed?.Invoke(name, result)));
        taskObject.Connect("function_error",
            Callable.From<int, string>((error, errorMessage) => Error?.Invoke((Functions.Error)error, errorMessage)));
    }

    public FirebaseFunction Call(Dictionary args)
    {
        taskObject.CallDeferred("call", args);
        return this;
    }

    public void Dispose() => taskObject.Free();


    public event Action<string, Dictionary> Completed;
    public event Action<Functions.Error, string> Error;
}
