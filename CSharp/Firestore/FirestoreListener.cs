namespace Firebase;

using System;
using Godot;
using Godot.Collections;

public sealed class FirestoreListener : IDisposable
{
    private GodotObject listenerObject;

    public FirestoreListener(GodotObject godotObject) => Set(godotObject);
    public FirestoreListener(Action<string, Dictionary> onDocumentUpdate, Action<string, int, string> onDocumentListenError)
    {
        if (onDocumentUpdate != null) DocumentUpdate += onDocumentUpdate;
        if (onDocumentListenError != null) DocumentListenError += onDocumentListenError;
    }
    public FirestoreListener(Action<string, Dictionary, Dictionary> onCollectionUpdate, Action<string, int, string> onCollectionListenError)
    {
        if (onCollectionUpdate != null)
        {
            CollectionUpdate += onCollectionUpdate;
            CollectionGroupUpdate += onCollectionUpdate;
        }
        if (onCollectionListenError != null)
        {
            CollectionListenError += onCollectionListenError;
            CollectionGroupListenError += onCollectionListenError;
        }
    }
    public FirestoreListener Set(GodotObject godotObject)
    {
        listenerObject = godotObject;

        godotObject.Connect("listeners_in_sync",
            Callable.From(() => ListenersInSync?.Invoke()));
        godotObject.Connect("document_update",
            Callable.From<string, Dictionary>((docPath, document) => DocumentUpdate?.Invoke(docPath, document)));
        godotObject.Connect("document_listen_error",
            Callable.From<string, int, string>((docPath, error, errorMessage) => DocumentListenError?.Invoke(docPath, error, errorMessage)));
        godotObject.Connect("collection_update",
            Callable.From<string, Dictionary, Dictionary>((collectionPath, changes, documents) => CollectionUpdate?.Invoke(collectionPath, changes, documents)));
        godotObject.Connect("collection_listen_error",
            Callable.From<string, int, string>((collectionPath, error, errorMessage) => CollectionListenError?.Invoke(collectionPath, error, errorMessage)));
        godotObject.Connect("collection_group_update",
            Callable.From<string, Dictionary, Dictionary>((collectionId, changes, documents) => CollectionGroupUpdate?.Invoke(collectionId, changes, documents)));
        godotObject.Connect("collection_group_listen_error",
            Callable.From<string, int, string>((collectionId, error, errorMessage) => CollectionGroupListenError?.Invoke(collectionId, error, errorMessage)));

        return this;
    }

    public void Dispose() => listenerObject.Free();

    public event Action ListenersInSync;
    public event Action<string, Dictionary> DocumentUpdate;
    public event Action<string, int, string> DocumentListenError;
    public event Action<string, Dictionary, Dictionary> CollectionUpdate;
    public event Action<string, int, string> CollectionListenError;
    public event Action<string, Dictionary, Dictionary> CollectionGroupUpdate;
    public event Action<string, int, string> CollectionGroupListenError;
}
