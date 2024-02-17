namespace Firebase;

using System;
using System.Threading.Tasks;
using Godot;
using Godot.Collections;

#nullable enable
public class FirestoreException : Exception
{
    private readonly Firestore.Error errorCode;

    public FirestoreException(Firestore.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firestore Error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Firestore
{
#pragma warning disable CA1716
    /// <summary>
    /// Error codes used by Cloud Firestore.
    /// <br />
    /// The codes are in sync across Firestore SDKs on various platforms.
    /// </summary>
    public enum Error
#pragma warning restore CA1716
    {
        /// <summary>
        /// The operation completed successfully.
        /// <br />
        /// Note: NSError objects will never have a code with this value.
        /// </summary>
        Ok = 0,
        /// <summary>
        /// The operation was cancelled (typically by the caller).
        /// </summary>
        Cancelled = 1,
        /// <summary>
        /// Unknown error or an error from a different error domain.
        /// </summary>
        Unknown = 2,
        /// <summary>
        /// Client specified an invalid argument. Note that this differs from
        /// FailedPrecondition. InvalidArgument indicates arguments that are
        /// problematic regardless of the state of the system (e.g., an invalid field
        /// name).
        /// </summary>
        InvalidArgument = 3,
        /// <summary>
        /// Deadline expired before operation could complete. For operations that
        /// change the state of the system, this error may be returned even if the
        /// operation has completed successfully. For example, a successful response
        /// from a server could have been delayed long enough for the deadline to
        /// expire.
        /// </summary>
        DeadlineExceeded = 4,
        /// <summary>
        /// Some requested document was not found.
        /// </summary>
        NotFound = 5,
        /// <summary>
        /// Some document that we attempted to create already exists.
        /// </summary>
        AlreadyExists = 6,
        /// <summary>
        /// The caller does not have permission to execute the specified operation.
        /// </summary>
        PermissionDenied = 7,
        /// <summary>
        /// Some resource has been exhausted, perhaps a per-user quota, or perhaps the
        /// entire file system is out of space.
        /// </summary>
        ResourceExhausted = 8,
        /// <summary>
        /// Operation was rejected because the system is not in a state required for
        /// the operation's execution.
        /// </summary>
        FailedPrecondition = 9,
        /// <summary>
        /// The operation was aborted, typically due to a concurrency issue like
        /// transaction aborts, etc.
        /// </summary>
        Aborted = 10,
        /// <summary>
        /// Operation was attempted past the valid range.
        /// </summary>
        OutOfRange = 11,
        /// <summary>
        /// Operation is not implemented or not supported/enabled.
        /// </summary>
        Unimplemented = 12,
        /// <summary>
        /// Internal errors. Means some invariants expected by underlying system has
        /// been broken. If you see one of these errors, something is very broken.
        /// </summary>
        Internal = 13,
        /// <summary>
        /// The service is currently unavailable. This is a most likely a transient
        /// condition and may be corrected by retrying with a backoff.
        /// </summary>
        Unavailable = 14,
        /// <summary>
        /// Unrecoverable data loss or corruption.
        /// </summary>
        DataLoss = 15,
        /// <summary>
        /// The request does not have valid authentication credentials for the
        /// operation.
        /// </summary>
        Unauthenticated = 16
    }

    private static bool initialized, unavailable;

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.FirestoreObject == null)
        {
            GD.PushError("Firebase.Firestore not found in engine!");
            unavailable = true;
            return false;
        }

        if (Firebase.LogDebug) GD.Print("Firestore initialized.");

        initialized = true;
        return initialized;
    }


    public static FirestoreListener ListenToSnapshotsInSync(Action<string, Dictionary> onCompleted = null, Action<string, int, string> onError = null)
        => new FirestoreListener(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("listen_to_snapshots_in_sync"));
    public static FirestoreBatch Batch()
        => new((GodotObject)Firebase.FirestoreObject.Call("batch"));
    public static void ClearPersistence()
        => Firebase.FirestoreObject.Call("clear_persistence");
    public static void DisableNetwork()
        => Firebase.FirestoreObject.Call("disable_network");
    public static void EnableNetwork()
        => Firebase.FirestoreObject.Call("enable_network");
    /// <summary>
    /// <b>Result:</b> bytesLoaded, documentsLoaded, totalBytes, totalDocuments
    /// </summary>
    public static LoadBundleTask LoadBundle(string bundle, Action<int, int, int, int> onCompleted = null, Action<Error, string> onError = null)
        => new LoadBundleTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("load_bundle", bundle));
    /// <summary>
    /// <b>Result:</b> bytesLoaded, documentsLoaded, totalBytes, totalDocuments
    /// </summary>
    public static Task<(int, int, int, int)> LoadBundleAsync(string bundle)
    {
        TaskCompletionSource<(int, int, int, int)> source = new();
        LoadBundle(bundle,
            (bytesLoaded, documentsLoaded, totalBytes, totalDocuments) => source.TrySetResult((bytesLoaded, documentsLoaded, totalBytes, totalDocuments)),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static GetNamedQueryTask NamedQuery(string queryName, Action<FirestoreQuery> onCompleted = null, Action<Error, string> onError = null)
        => new GetNamedQueryTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("named_query", queryName));
    public static Task<FirestoreQuery> NamedQueryAsync(string queryName)
    {
        TaskCompletionSource<FirestoreQuery> source = new();
        NamedQuery(queryName,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static void RunTransaction(FirestoreTransaction transactionRunner, int maxAttempts = 5)
        => Firebase.FirestoreObject.Call("run_transaction", transactionRunner.Transaction, maxAttempts);
    public static void SetLogLevel(int level)
        => Firebase.FirestoreObject.Call("set_log_level", level);
    public static void SetSettings(Dictionary settings)
        => Firebase.FirestoreObject.Call("set_settings", settings);
    public static Dictionary GetSettings()
        => (Dictionary)Firebase.FirestoreObject.Call("get_settings");
    public static void Terminate()
        => Firebase.FirestoreObject.Call("terminate");
    public static WaitForPendingWritesTask WaitForPendingWrites(Action onCompleted = null, Action<Error, string> onError = null)
        => new WaitForPendingWritesTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("wait_for_pending_writes"));
    public static Task WaitForPendingWritesAsync()
    {
        TaskCompletionSource source = new();
        WaitForPendingWrites(
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }

    public static FirestoreListener ListenToCollection(string collectionPath, Action<string, Dictionary, Dictionary> onCompleted = null, Action<string, int, string> onError = null)
        => new FirestoreListener(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("listen_to_collection", collectionPath));
    public static AddDocumentTask AddDocument(string docPath, Dictionary fields, Action onCompleted = null, Action<Error, string> onError = null)
        => new AddDocumentTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("add_document", docPath, fields));
    public static Task AddDocumentAsync(string docPath, Dictionary fields)
    {
        TaskCompletionSource source = new();
        AddDocument(docPath, fields,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static string NewDocumentIn(string collectionPath)
        => (string)Firebase.FirestoreObject.Call("new_document_in", collectionPath);
    public static FirestoreQuery CollectionQuery(string collectionPath)
        => new((GodotObject)Firebase.FirestoreObject.Call("collection_query", collectionPath));
    /// <summary>
    /// <b>Result:</b> changes, documents
    /// </summary>
    public static Task<(Godot.Collections.Array, Godot.Collections.Array)> CollectionQueryAsync(string collectionPath)
    {
        TaskCompletionSource<(Godot.Collections.Array, Godot.Collections.Array)> source = new();
        FirestoreQuery query = CollectionQuery(collectionPath);
        query.QueryCompleted += (changes, documents) => source.TrySetResult((changes, documents));
        query.QueryError += (error, message) => source.TrySetException(new FirestoreException(error, message));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> number of documents in collection
    /// </summary>
    public static CollectionExistsTask CollectionExists(string collectionPath, Action<int> onCompleted = null, Action<Error, string> onError = null)
        => new CollectionExistsTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("collection_exists", collectionPath));
    /// <summary>
    /// <b>Result:</b> number of documents in collection
    /// </summary>
    public static Task<int> CollectionExistsAsync(string collectionPath)
    {
        TaskCompletionSource<int> source = new();
        CollectionExists(collectionPath,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static string CollectionParent(string collectionPath)
        => (string)Firebase.FirestoreObject.Call("collection_parent", collectionPath);

    public static FirestoreListener ListenToCollectionGroup(string collectionId, Action<string, Dictionary, Dictionary> onCompleted = null, Action<string, int, string> onError = null)
        => new FirestoreListener(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("listen_to_collection_group", collectionId));
    public static FirestoreQuery CollectionGroupQuery(string collectionId)
        => new((GodotObject)Firebase.FirestoreObject.Call("collection_group_query", collectionId));

    public static FirestoreListener ListenToDocument(string docPath, Action<string, Dictionary> onCompleted = null, Action<string, int, string> onError = null)
        => new FirestoreListener(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("listen_to_document", docPath));
    public static DeleteDocumentTask DeleteDocument(string docPath, Action onCompleted = null, Action<Error, string> onError = null)
        => new DeleteDocumentTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("delete_document", docPath));
    public static Task DeleteDocumentAsync(string docPath)
    {
        TaskCompletionSource source = new();
        DeleteDocument(docPath,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> document fields
    /// </summary>
    public static GetDocumentTask GetDocument(string docPath, Action<Dictionary> onCompleted = null, Action<Error, string> onError = null)
        => new GetDocumentTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("get_document", docPath));
    /// <summary>
    /// <b>Result:</b> document fields
    /// </summary>
    public static Task<Dictionary> GetDocumentAsync(string docPath)
    {
        TaskCompletionSource<Dictionary> source = new();
        GetDocument(docPath,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static DocumentExistsTask DocumentExists(string docPath, Action<bool> onCompleted = null, Action<Error, string> onError = null)
        => new DocumentExistsTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("document_exists", docPath));
    public static Task<bool> DocumentExistsAsync(string docPath)
    {
        TaskCompletionSource<bool> source = new();
        DocumentExists(docPath,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static string DocumentParent(string docPath)
        => (string)Firebase.FirestoreObject.Call("document_parent", docPath);
    public static SetDocumentTask SetDocument(string docPath, Dictionary fields, SetOptions setOptions, Godot.Collections.Array mergefields, Action onCompleted = null, Action<Error, string> onError = null)
        => new SetDocumentTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("set_document", docPath, fields, (int)setOptions, mergefields));
    public static Task SetDocumentAsync(string docPath, Dictionary fields, SetOptions setOptions, Godot.Collections.Array mergefields)
    {
        TaskCompletionSource source = new();
        SetDocument(docPath, fields, setOptions, mergefields,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
    public static string GetDocumentString(string docPath)
        => (string)Firebase.FirestoreObject.Call("get_document_string", docPath);

    public static void AddDeleteField(Dictionary toAddTo, string fieldName)
        => toAddTo.Add(fieldName, "__DELETE_FIELD__");
    public static void AddServerTimestamp(Dictionary toAddTo, string fieldName)
        => toAddTo.Add(fieldName, "__SERVER_TIMESTAMP__");
    public static void AddFieldIncrement(Dictionary toAddTo, string fieldName, double change)
        => toAddTo.Add(fieldName, new Dictionary() { { "__INCREMENT__", change } });
    public static void AddFieldIncrement(Dictionary toAddTo, string fieldName, int change)
        => toAddTo.Add(fieldName, new Dictionary() { { "__INCREMENT__", change } });
    public static void AddArrayRemove(Dictionary toAddTo, string fieldName, Godot.Collections.Array array)
        => toAddTo.Add(fieldName, new Dictionary() { { "__ARRAY_REMOVE__", array } });
    public static void AddArrayUnion(Dictionary toAddTo, string fieldName, Godot.Collections.Array array)
        => toAddTo.Add(fieldName, new Dictionary() { { "__ARRAY_UNION__", array } });
    public static UpdateDocumentTask UpdateDocument(string docPath, Dictionary changes, Action onCompleted = null, Action<Error, string> onError = null)
        => new UpdateDocumentTask(onCompleted, onError).Set((GodotObject)Firebase.FirestoreObject.Call("update_document", docPath, changes));
    public static Task UpdateDocumentAsync(string docPath, Dictionary changes)
    {
        TaskCompletionSource source = new();
        UpdateDocument(docPath, changes,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirestoreException(error, message)));
        return source.Task;
    }
}
