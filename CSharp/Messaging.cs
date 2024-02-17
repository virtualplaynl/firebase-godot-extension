namespace Firebase;

using System;
using System.Threading.Tasks;
using Godot;
using Godot.Collections;

#nullable enable
public class FirebaseMessagingException : Exception
{
    private readonly Messaging.Error errorCode;

    public FirebaseMessagingException(Messaging.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firebase Messaging Error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Messaging
{
#pragma warning disable CA1716
    /// <summary>Error code returned by Firebase Cloud Messaging C++ functions.</summary>
    public enum Error
    {
        /// <summary>The operation was a success, no error occurred.</summary>
        None = 0,
        /// <summary>Permission to receive notifications was not granted.</summary>
        FailedToRegisterForRemoteNotifications,
        /// <summary>Topic name is invalid for subscription/unsubscription.</summary>
        InvalidTopicName,
        /// <summary>Could not subscribe/unsubscribe because there is no registration token.</summary>
        NoRegistrationToken,
        /// <summary>Unknown error.</summary>
        Unknown
    }

    private static bool initialized, unavailable;

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.MessagingObject == null)
        {
            GD.PushError("Firebase.Messaging not found in engine!");
            unavailable = true;
            return false;
        }

        try
        {
            if (!Firebase.MessagingObject.IsConnected("message_received", Callable.From<Dictionary>(OnMessageReceived)))
                Firebase.MessagingObject.Connect("message_received", Callable.From<Dictionary>(OnMessageReceived));
            if (!Firebase.MessagingObject.IsConnected("token_received", Callable.From<string>(OnTokenReceived)))
                Firebase.MessagingObject.Connect("token_received", Callable.From<string>(OnTokenReceived));

            if (Firebase.LogDebug) GD.Print("Firebase Messaging initialized.");
        }
        catch (Exception)
        {
            return false;
        }

        initialized = true;
        return initialized;
    }

    public static void RequestPermission() => Firebase.MessagingObject.Call("request_permission");

    public static GetFCMTokenTask GetToken(Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new GetFCMTokenTask(onCompleted, onError).Set((GodotObject)Firebase.MessagingObject.Call("get_token"));
    public static Task<string> GetTokenAsync()
    {
        TaskCompletionSource<string> source = new();
        GetToken(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseMessagingException(error, message)));
        return source.Task;
    }
    public static DeleteTokenTask DeleteToken(Action onCompleted = null, Action<Error, string> onError = null)
        => new DeleteTokenTask(onCompleted, onError).Set((GodotObject)Firebase.MessagingObject.Call("delete_token"));
    public static Task DeleteTokenAsync()
    {
        TaskCompletionSource source = new();
        DeleteToken(
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseMessagingException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> subscribed topic
    /// </summary>
    public static SubscribeTask Subscribe(string topic, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new SubscribeTask(onCompleted, onError).Set((GodotObject)Firebase.MessagingObject.Call("subscribe", topic));
    /// <summary>
    /// <b>Result:</b> subscribed topic
    /// </summary>
    public static Task<string> SubscribeAsync(string topic)
    {
        TaskCompletionSource<string> source = new();
        Subscribe(topic,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseMessagingException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> unsubscribed topic
    /// </summary>
    public static UnsubscribeTask Unsubscribe(string topic, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new UnsubscribeTask(onCompleted, onError).Set((GodotObject)Firebase.MessagingObject.Call("unsubscribe", topic));
    /// <summary>
    /// <b>Result:</b> unsubscribed topic
    /// </summary>
    public static Task<string> UnsubscribeAsync(string topic)
    {
        TaskCompletionSource<string> source = new();
        Unsubscribe(topic,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseMessagingException(error, message)));
        return source.Task;
    }
    public static void Terminate() => Firebase.MessagingObject.Call("terminate");

    private static void OnMessageReceived(Dictionary message) => MessageReceived?.Invoke(message);
    public static event Action<Dictionary> MessageReceived;
    private static void OnTokenReceived(string token) => TokenReceived?.Invoke(token);
    public static event Action<string> TokenReceived;
}
