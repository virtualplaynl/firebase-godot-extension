namespace Firebase;

using Godot;

public enum SetOptions : int
{
    Overwrite = 0,
    MergeAll = 1,
    MergeSpecific = 2
};

public enum Direction : int
{
    Ascending = 0,
    Descending = 1
};

public enum Operation : int
{
    Delete = 0,
    Get = 1,
    Set = 2,
    Update = 3
};

public static class Firebase
{
    private static readonly GodotObject Singleton = Engine.HasSingleton("Firebase") ? Engine.GetSingleton("Firebase") : null;
    public static void Init() => Singleton.Call("init");
    public static void InitWith(string configJson) => Singleton.Call("init_with", configJson);

    public static bool LogDebug { get; set; }

    private static GodotObject auth, database, firestore, functions, messaging, storage;
    public static GodotObject AuthObject
    {
        get
        {
            auth ??= (GodotObject)Singleton.Call("auth");
            return auth;
        }
    }
    public static GodotObject DatabaseObject
    {
        get
        {
            database ??= (GodotObject)Singleton.Call("database");
            return database;
        }
    }
    public static GodotObject FirestoreObject
    {
        get
        {
            firestore ??= (GodotObject)Singleton.Call("firestore");
            return firestore;
        }
    }
    public static GodotObject FunctionsObject
    {
        get
        {
            functions ??= (GodotObject)Singleton.Call("functions");
            return functions;
        }
    }
    public static GodotObject MessagingObject
    {
        get
        {
            messaging ??= (GodotObject)Singleton.Call("messaging");
            return messaging;
        }
    }
    public static GodotObject StorageObject
    {
        get
        {
            storage ??= (GodotObject)Singleton.Call("storage");
            return storage;
        }
    }
}
