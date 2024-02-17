namespace Firebase;

using System;
using Godot;

#nullable enable
public class FirebaseDatabaseException : Exception
{
    private readonly Database.Error errorCode;

    public FirebaseDatabaseException(Database.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firebase error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Database
{
#pragma warning disable CA1716
    /// <summary>
    /// Database error codes
    /// </summary>
    public enum Error
    {
    }
#pragma warning restore CA1716

    private static bool initialized, unavailable;

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.DatabaseObject == null)
        {
            GD.PushError("Firebase.Database not found in engine!");
            unavailable = true;
            return false;
        }

        if (Firebase.LogDebug) GD.Print("Realtime Database initialized.");

        initialized = true;
        return initialized;
    }

    public static void Offline() => Firebase.DatabaseObject.Call("offline");
    public static void Online() => Firebase.DatabaseObject.Call("online");
}
