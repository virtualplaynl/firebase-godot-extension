namespace Firebase;

using System;
using System.Threading.Tasks;
using Godot;

#nullable enable
public class FirebaseStorageException : Exception
{
    private readonly Storage.Error errorCode;

    public FirebaseStorageException(Storage.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firebase Storage Error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Storage
{
#pragma warning disable CA1716
    /// <summary>Error code returned by Cloud Storage C++ functions.</summary>
    public enum Error
#pragma warning restore CA1716
    {
        /// <summary>The operation was a success, no error occurred.</summary>
        None = 0,
        /// <summary>An unknown error occurred.</summary>
        Unknown,
        /// <summary>No object exists at the desired reference.</summary>
        ObjectNotFound,
        /// <summary>No bucket is configured for Cloud Storage.</summary>
        BucketNotFound,
        /// <summary>No project is configured for Cloud Storage.</summary>
        ProjectNotFound,
        /// <summary>Quota on your Cloud Storage bucket has been exceeded.</summary>
        QuotaExceeded,
        /// <summary>User is unauthenticated.</summary>
        Unauthenticated,
        /// <summary>User is not authorized to perform the desired action.</summary>
        Unauthorized,
        /// <summary>The maximum time limit on an operation (upload, download, delete, etc.) has been exceeded.</summary>
        RetryLimitExceeded,
        /// <summary>File on the client does not match the checksum of the file received by the server.</summary>
        NonMatchingChecksum,
        /// <summary>Size of the downloaded file exceeds the amount of memory allocated for the download.</summary>
        DownloadSizeExceeded,
        /// <summary>User cancelled the operation.</summary>
        Cancelled
    }

    private static bool initialized, unavailable;

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.StorageObject == null)
        {
            GD.PushError("Firebase.Storage not found in engine!");
            unavailable = true;
            return false;
        }

        if (Firebase.LogDebug) GD.Print("Firebase Storage initialized.");

        initialized = true;
        return initialized;
    }

    public static string Bucket(string path)
        => Firebase.StorageObject.Call("bucket", path).AsString();
    public static string Child(string path, string child)
        => Firebase.StorageObject.Call("child", path, child).AsString();
    public static StorageDeleteTask DeleteObject(string path, Action onCompleted = null, Action<Error, string> onError = null)
        => new StorageDeleteTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("delete_object", path));
    public static Task DeleteObjectAsync(string path)
    {
        TaskCompletionSource source = new();
        DeleteObject(path,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> number of bytes read
    /// </summary>
    public static GetBytesTask GetBytes(string path, byte[] buffer, Action<long> onCompleted = null, Action<Error, string> onError = null)
        => new GetBytesTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("get_bytes", path, buffer));
    /// <summary>
    /// <b>Result:</b> number of bytes read
    /// </summary>
    public static Task<long> GetBytesAsync(string path, byte[] buffer)
    {
        TaskCompletionSource<long> source = new();
        GetBytes(path, buffer,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    public static GetDownloadUrlTask GetDownloadUrl(string path, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new GetDownloadUrlTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("get_download_url", path));
    public static Task<string> GetDownloadUrlAsync(string path)
    {
        TaskCompletionSource<string> source = new();
        GetDownloadUrl(path,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> number of bytes read
    /// </summary>
    public static GetFileTask GetFile(string path, string localPath, Action<long> onCompleted = null, Action<Error, string> onError = null)
        => new GetFileTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("get_file", path, localPath));
    /// <summary>
    /// <b>Result:</b> number of bytes read
    /// </summary>
    public static Task<long> GetFileAsync(string path, string localPath)
    {
        TaskCompletionSource<long> source = new();
        GetFile(path, localPath,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    public static GetMetadataTask GetMetadata(string path, Action<Metadata> onCompleted = null, Action<Error, string> onError = null)
        => new GetMetadataTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("get_metadata", path));
    public static Task<Metadata> GetMetadataAsync(string path)
    {
        TaskCompletionSource<Metadata> source = new();
        GetMetadata(path,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    public static string Parent(string path)
        => Firebase.StorageObject.Call("parent", path).AsString();
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static PutBytesTask PutBytes(string path, byte[] buffer, Action<Metadata> onCompleted = null, Action<Error, string> onError = null)
        => new PutBytesTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("put_bytes", path, buffer));
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static Task<Metadata> PutBytesAsync(string path, byte[] buffer)
    {
        TaskCompletionSource<Metadata> source = new();
        PutBytes(path, buffer,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static PutFileTask PutFile(string path, string localPath, Action<Metadata> onCompleted = null, Action<Error, string> onError = null)
        => new PutFileTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("put_file", path, localPath));
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static Task<Metadata> PutFileAsync(string path, string localPath)
    {
        TaskCompletionSource<Metadata> source = new();
        PutFile(path, localPath,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static PutFileTask PutFileWithMetadata(string path, string localPath, Metadata metadata, Action<Metadata> onCompleted = null, Action<Error, string> onError = null)
        => new PutFileTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("put_file_with_metadata", path, localPath, metadata.AsDictionary()));
    /// <summary>
    /// <b>Result:</b> metadata of new object
    /// </summary>
    public static Task<Metadata> PutFileWithMetadataAsync(string path, string localPath, Metadata metadata)
    {
        TaskCompletionSource<Metadata> source = new();
        PutFileWithMetadata(path, localPath, metadata,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> updated metadata
    /// </summary>
    public static UpdateMetadataTask UpdateMetadata(string path, Metadata metadata, Action<Metadata> onCompleted = null, Action<Error, string> onError = null)
        => new UpdateMetadataTask(onCompleted, onError).Set((GodotObject)Firebase.StorageObject.Call("update_metadata", path, metadata.AsDictionary()));
    /// <summary>
    /// <b>Result:</b> updated metadata
    /// </summary>
    public static Task<Metadata> UpdateMetadataAsync(string path, Metadata metadata)
    {
        TaskCompletionSource<Metadata> source = new();
        UpdateMetadata(path, metadata,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseStorageException(error, message)));
        return source.Task;
    }
    public static void SetMaxDownloadRetryTime(double maxTransferRetrySeconds)
        => Firebase.StorageObject.Call("set_max_download_retry_time", maxTransferRetrySeconds);
    public static void SetMaxOperationRetryTime(double maxTransferRetrySeconds)
        => Firebase.StorageObject.Call("set_max_operation_retry_time", maxTransferRetrySeconds);
    public static void SetMaxUploadRetryTime(double maxTransferRetrySeconds)
        => Firebase.StorageObject.Call("set_max_upload_retry_time", maxTransferRetrySeconds);
}
