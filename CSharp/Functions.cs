namespace Firebase;

using System;
using System.Threading.Tasks;
using Godot;
using Godot.Collections;

#nullable enable
public class FirebaseFunctionsException : Exception
{
    private readonly Functions.Error errorCode;

    public FirebaseFunctionsException(Functions.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firebase Functions Error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Functions
{
#pragma warning disable CA1716
    /// <summary>Error code returned by Cloud Functions C++ functions.
    /// Standard gRPC error codes, as defined in:
    /// https://github.com/googleapis/googleapis/blob/master/google/rpc/code.proto</summary>
    public enum Error
#pragma warning restore CA1716
    {
        /// <summary>Not an error; returned on success
        /// <br />---<br />
        /// HTTP Mapping: <b>200 OK</b></summary>
        None = 0,
        /// <summary>The operation was cancelled, typically by the caller.
        /// <br />---<br />
        /// HTTP Mapping: <b>499 Client Closed Request</b></summary>
        Cancelled = 1,
        /// <summary>Unknown error.  For example, this error may be returned when
        /// a `Status` value received from another address space belongs to
        /// an error space that is not known in this address space.  Also
        /// errors raised by APIs that do not return enough error information
        /// may be converted to this error.
        /// <br />---<br />
        /// HTTP Mapping: <b>500 Internal Server Error</b></summary>
        Unknown = 2,
        /// <summary>The client specified an invalid argument.  Note that this differs
        /// from `FAILED_PRECONDITION`.  `INVALID_ARGUMENT` indicates arguments
        /// that are problematic regardless of the state of the system
        /// (e.g., a malformed file name).
        /// <br />---<br />
        /// HTTP Mapping: <b>400 Bad Request</b></summary>
        InvalidArgument = 3,
        /// <summary>The deadline expired before the operation could complete. For operations
        /// that change the state of the system, this error may be returned
        /// even if the operation has completed successfully.  For example, a
        /// successful response from a server could have been delayed long
        /// enough for the deadline to expire.
        /// <br />---<br />
        /// HTTP Mapping: <b>504 Gateway Timeout</b></summary>
        DeadlineExceeded = 4,
        /// <summary>Some requested entity (e.g., file or directory) was not found.
        /// <br />---<br />
        /// Note to server developers: if a request is denied for an entire class
        /// of users, such as gradual feature rollout or undocumented whitelist,
        /// `NOT_FOUND` may be used. If a request is denied for some users within
        /// a class of users, such as user-based access control, `PERMISSION_DENIED`
        /// must be used.
        /// <br />---<br />
        /// HTTP Mapping: <b>404 Not Found</b></summary>
        NotFound = 5,
        /// <summary>The entity that a client attempted to create (e.g., file or directory)
        /// already exists.
        /// <br />---<br />
        /// HTTP Mapping: <b>409 Conflict</b></summary>
        AlreadyExists = 6,
        /// <summary>The caller does not have permission to execute the specified
        /// operation. `PERMISSION_DENIED` must not be used for rejections
        /// caused by exhausting some resource (use `RESOURCE_EXHAUSTED`
        /// instead for those errors). `PERMISSION_DENIED` must not be
        /// used if the caller can not be identified (use `UNAUTHENTICATED`
        /// instead for those errors). This error code does not imply the
        /// request is valid or the requested entity exists or satisfies
        /// other pre-conditions.
        /// <br />---<br />
        /// HTTP Mapping: <b>403 Forbidden</b></summary>
        PermissionDenied = 7,
        /// <summary>The request does not have valid authentication credentials for the
        /// operation.
        /// <br />---<br />
        /// HTTP Mapping: <b>401 Unauthorized</b></summary>
        Unauthenticated = 16,
        /// <summary>Some resource has been exhausted, perhaps a per-user quota, or
        /// perhaps the entire file system is out of space.
        /// <br />---<br />
        /// HTTP Mapping: <b>429 Too Many Requests</b></summary>
        ResourceExhausted = 8,
        /// <summary>The operation was rejected because the system is not in a state
        /// required for the operation's execution.  For example, the directory
        /// to be deleted is non-empty, an rmdir operation is applied to
        /// a non-directory, etc.
        /// <br />---<br />
        /// Service implementors can use the following guidelines to decide
        /// between `FAILED_PRECONDITION`, `ABORTED`, and `UNAVAILABLE`:
        ///  (a) Use `UNAVAILABLE` if the client can retry just the failing call.
        ///  (b) Use `ABORTED` if the client should retry at a higher level
        ///      (e.g., when a client-specified test-and-set fails, indicating the
        ///      client should restart a read-modify-write sequence).
        ///  (c) Use `FAILED_PRECONDITION` if the client should not retry until
        ///      the system state has been explicitly fixed.  E.g., if an "rmdir"
        ///      fails because the directory is non-empty, `FAILED_PRECONDITION`
        ///      should be returned since the client should not retry unless
        ///      the files are deleted from the directory.
        /// <br />---<br />
        /// HTTP Mapping: <b>400 Bad Request</b></summary>
        FailedPrecondition = 9,
        /// <summary>The operation was aborted, typically due to a concurrency issue such as
        /// a sequencer check failure or transaction abort.
        /// <br />---<br />
        /// See the guidelines above for deciding between `FAILED_PRECONDITION`,
        /// `ABORTED`, and `UNAVAILABLE`.
        /// <br />---<br />
        /// HTTP Mapping: <b>409 Conflict</b></summary>
        Aborted = 10,
        /// <summary>The operation was attempted past the valid range.  E.g., seeking or
        /// reading past end-of-file.
        /// <br />---<br />
        /// Unlike `INVALID_ARGUMENT`, this error indicates a problem that may
        /// be fixed if the system state changes. For example, a 32-bit file
        /// system will generate `INVALID_ARGUMENT` if asked to read at an
        /// offset that is not in the range [0,2^32-1], but it will generate
        /// `OUT_OF_RANGE` if asked to read from an offset past the current
        /// file size.
        /// <br />---<br />
        /// There is a fair bit of overlap between `FAILED_PRECONDITION` and
        /// `OUT_OF_RANGE`.  We recommend using `OUT_OF_RANGE` (the more specific
        /// error) when it applies so that callers who are iterating through
        /// a space can easily look for an `OUT_OF_RANGE` error to detect when
        /// they are done.
        /// <br />---<br />
        /// HTTP Mapping: <b>400 Bad Request</b></summary>
        OutOfRange = 11,
        /// <summary>The operation is not implemented or is not supported/enabled in this
        /// service.
        /// <br />---<br />
        /// HTTP Mapping: <b>501 Not Implemented</b></summary>
        Unimplemented = 12,
        /// <summary>Internal errors.  This means that some invariants expected by the
        /// underlying system have been broken.  This error code is reserved
        /// for serious errors.
        /// <br />---<br />
        /// HTTP Mapping: <b>500 Internal Server Error</b></summary>
        Internal = 13,
        /// <summary>The service is currently unavailable.  This is most likely a
        /// transient condition, which can be corrected by retrying with
        /// a backoff.
        /// <br />---<br />
        /// See the guidelines above for deciding between `FAILED_PRECONDITION`,
        /// `ABORTED`, and `UNAVAILABLE`.
        /// <br />---<br />
        /// HTTP Mapping: <b>503 Service Unavailable</b></summary>
        Unavailable = 14,
        /// <summary>Unrecoverable data loss or corruption.
        /// <br />---<br />
        /// HTTP Mapping: <b>500 Internal Server Error</b></summary>
        DataLoss = 15,
    }

    private static bool initialized, unavailable;

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.FunctionsObject == null)
        {
            GD.PushError("Firebase.Functions not found in engine!");
            unavailable = true;
            return false;
        }

        if (Firebase.LogDebug) GD.Print("Firebase Functions initialized.");

        initialized = true;
        return initialized;
    }

    public static FirebaseFunction Call(SceneTree tree, string name, Dictionary args, Action<string, Dictionary> onCompleted = null, Action<Error, string> onError = null)
    {
        return new FirebaseFunction(tree, name, onCompleted, onError).Call(args);
    }
    public static Task<Dictionary> CallAsync(SceneTree tree, string name, Dictionary args)
    {
        TaskCompletionSource<Dictionary> source = new();
        new FirebaseFunction(
            tree, name,
            (name, result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseFunctionsException(error, message)))
            .Call(args);
        return source.Task;
    }
}
