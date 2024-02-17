namespace Firebase;

using System;
using System.Threading.Tasks;
using Godot;
using Godot.Collections;

public enum LoginProvider : long
{
    Anonymous,
    EmailAndPassword,
    Phone,
    Apple,
    Facebook,
    GameCenter,
    Google,
    GooglePlayGames,
    Microsoft,
    X,
    Yahoo,
    GitHub,
    Snapchat,
    TikTok,
    Steam,
    EpicGames,
    Other = 9999999L
}

#nullable enable
public class FirebaseAuthException : Exception
{
    private readonly Auth.Error errorCode;

    public FirebaseAuthException(Auth.Error error, string? message, Exception? innerException = null)
        : base(message, innerException)
        => errorCode = error;

    public override string ToString()
    {
        return $"Firebase Authentication Error: {errorCode}\n{base.ToString()}";
    }
}
#nullable disable

public static class Auth
{
#pragma warning disable CA1716
    /// <summary>All possible error codes from asynchronous Firebase Authentication calls.</summary>
    public enum Error
#pragma warning restore CA1716
    {
        /// <summary>Success.</summary>
        None = 0,
        /// <summary>Function will be implemented in a later revision of the API.</summary>
        Unimplemented = -1,
        /// <summary>This indicates an internal error.
        /// Common error code for all API Methods.</summary>
        Failure = 1,
        /// <summary>Indicates a validation error with the custom token.
        /// This error originates from "bring your own auth" methods.</summary>
        InvalidCustomToken,
        /// <summary>Indicates the service account and the API key belong to different
        /// projects.
        /// Caused by "Bring your own auth" methods.</summary>
        CustomTokenMismatch,
        /// <summary>Indicates the IDP token or requestUri is invalid.
        /// Caused by "Sign in with credential" methods.</summary>
        InvalidCredential,
        /// <summary>Indicates the user’s account is disabled on the server.
        /// Caused by "Sign in with credential" methods.</summary>
        UserDisabled,
        /// <summary>Indicates an account already exists with the same email address but using
        /// different sign-in credentials. Account linking is required.
        /// Caused by "Sign in with credential" methods.</summary>
        AccountExistsWithDifferentCredentials,
        /// <summary>Indicates the administrator disabled sign in with the specified identity
        /// provider.
        /// Caused by "Set account info" methods.</summary>
        OperationNotAllowed,
        /// <summary>Indicates the email used to attempt a sign up is already in use.
        /// Caused by "Set account info" methods.</summary>
        EmailAlreadyInUse,
        /// <summary>Indicates the user has attemped to change email or password more than 5
        /// minutes after signing in, and will need to refresh the credentials.
        /// Caused by "Set account info" methods.</summary>
        RequiresRecentLogin,
        /// <summary>Indicates an attempt to link with a credential that has already been
        /// linked with a different Firebase account.
        /// Caused by "Set account info" methods.</summary>
        CredentialAlreadyInUse,
        /// <summary>Indicates an invalid email address.
        /// Caused by "Sign in with password" methods.</summary>
        InvalidEmail,
        /// <summary>Indicates the user attempted sign in with a wrong password.
        /// Caused by "Sign in with password" methods.</summary>
        WrongPassword,
        /// <summary>Indicates that too many requests were made to a server method.
        /// Common error code for all API methods.</summary>
        TooManyRequests,
        /// <summary>Indicates the user account was not found.
        /// Send password request email error code.
        /// Common error code for all API methods.</summary>
        UserNotFound,
        /// <summary>Indicates an attempt to link a provider to which the account is already
        /// linked.
        /// Caused by "Link credential" methods.</summary>
        ProviderAlreadyLinked,
        /// <summary>Indicates an attempt to unlink a provider that is not linked.
        /// Caused by "Link credential" methods.</summary>
        NoSuchProvider,
        /// <summary>Indicates user's saved auth credential is invalid, the user needs to sign
        /// in again.
        /// Caused by requests with an STS id token.</summary>
        InvalidUserToken,
        /// <summary>Indicates the saved token has expired.
        /// For example, the user may have changed account password on another device.
        /// The user needs to sign in again on the device that made this request.
        /// Caused by requests with an STS id token.</summary>
        UserTokenExpired,
        /// <summary>Indicates a network error occurred (such as a timeout, interrupted
        /// connection, or unreachable host). These types of errors are often
        /// recoverable with a retry.
        /// Common error code for all API Methods.</summary>
        NetworkRequestFailed,
        /// <summary>Indicates an invalid API key was supplied in the request.
        /// For Android these should no longer occur (as of 2016 v3).
        /// Common error code for all API Methods.</summary>
        InvalidApiKey,
        /// <summary>Indicates the App is not authorized to use Firebase Authentication with
        /// the provided API Key.
        /// Common error code for all API Methods.
        /// On Android this error should no longer occur (as of 2016 v3).
        /// Common error code for all API Methods.</summary>
        AppNotAuthorized,
        /// <summary>Indicates that an attempt was made to reauthenticate with a user which is
        /// not the current user.</summary>
        UserMismatch,
        /// <summary>Indicates an attempt to set a password that is considered too weak.</summary>
        WeakPassword,
        /// <summary>Internal api usage error code when there is no signed-in user
        /// and getAccessToken is called.
        ///
        /// @note This error is only reported on Android.</summary>
        NoSignedInUser,
        /// <summary>This can happen when certain methods on App are performed, when the auth
        /// API is not loaded.
        ///
        /// @note This error is only reported on Android.</summary>
        ApiNotAvailable,
        /// <summary>Indicates the out-of-band authentication code is expired.</summary>
        ExpiredActionCode,
        /// <summary>Indicates the out-of-band authentication code is invalid.</summary>
        InvalidActionCode,
        /// <summary>Indicates that there are invalid parameters in the payload during a
        /// "send password reset email" attempt.</summary>
        InvalidMessagePayload,
        /// <summary>Indicates that an invalid phone number was provided.
        /// This is caused when the user is entering a phone number for verification.</summary>
        InvalidPhoneNumber,
        /// <summary>Indicates that a phone number was not provided during phone number
        /// verification.
        ///
        /// @note This error is iOS-specific.</summary>
        MissingPhoneNumber,
        /// <summary>Indicates that the recipient email is invalid.</summary>
        InvalidRecipientEmail,
        /// <summary>Indicates that the sender email is invalid during a "send password reset
        /// email" attempt.</summary>
        InvalidSender,
        /// <summary>Indicates that an invalid verification code was used in the
        /// verifyPhoneNumber request.</summary>
        InvalidVerificationCode,
        /// <summary>Indicates that an invalid verification ID was used in the
        /// verifyPhoneNumber request.</summary>
        InvalidVerificationId,
        /// <summary>Indicates that the phone auth credential was created with an empty
        /// verification code.</summary>
        MissingVerificationCode,
        /// <summary>Indicates that the phone auth credential was created with an empty
        /// verification ID.</summary>
        MissingVerificationId,
        /// <summary>Indicates that an email address was expected but one was not provided.</summary>
        MissingEmail,
        /// <summary>Represents the error code for when an application attempts to create an
        /// email/password account with an empty/null password field.
        ///
        /// @note This error is only reported on Android.</summary>
        MissingPassword,
        /// <summary>Indicates that the project's quota for this operation (SMS messages,
        /// sign-ins, account creation) has been exceeded. Try again later.</summary>
        QuotaExceeded,
        /// <summary>Thrown when one or more of the credentials passed to a method fail to
        /// identify and/or authenticate the user subject of that operation. Inspect
        /// the error message to find out the specific cause.
        /// @note This error is only reported on Android.</summary>
        RetryPhoneAuth,
        /// <summary>Indicates that the SMS code has expired.</summary>
        SessionExpired,
        /// <summary>Indicates that the app could not be verified by Firebase during phone
        /// number authentication.
        ///
        /// @note This error is iOS-specific.</summary>
        AppNotVerified,
        /// <summary>Indicates a general failure during the app verification flow.
        ///
        /// @note This error is iOS-specific.</summary>
        AppVerificationFailed,
        /// <summary>Indicates that the reCAPTCHA token is not valid.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        CaptchaCheckFailed,
        /// <summary>Indicates that an invalid APNS device token was used in the verifyClient
        /// request.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        InvalidAppCredential,
        /// <summary>Indicates that the APNS device token is missing in the verifyClient
        /// request.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        MissingAppCredential,
        /// <summary>Indicates that the clientID used to invoke a web flow is invalid.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        InvalidClientId,
        /// <summary>Indicates that the domain specified in the continue URI is not valid.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        InvalidContinueUri,
        /// <summary>Indicates that a continue URI was not provided in a request to the backend
        /// which requires one.</summary>
        MissingContinueUri,
        /// <summary>Indicates an error occurred while attempting to access the keychain.
        /// Common error code for all API Methods.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        KeychainError,
        /// <summary>Indicates that the APNs device token could not be obtained. The app may
        /// not have set up remote notification correctly, or may have failed to
        /// forward the APNs device token to FIRAuth if app delegate swizzling is
        /// disabled.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        MissingAppToken,
        /// <summary>Indicates that the iOS bundle ID is missing when an iOS App Store ID is
        /// provided.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        MissingIosBundleId,
        /// <summary>Indicates that the app fails to forward remote notification to FIRAuth.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        NotificationNotForwarded,
        /// <summary>Indicates that the domain specified in the continue URL is not white-
        /// listed in the Firebase console.
        ///
        /// @note This error is iOS and tvOS-specific.</summary>
        UnauthorizedDomain,
        /// <summary>Indicates that an attempt was made to present a new web context while one
        /// was already being presented.</summary>
        WebContextAlreadyPresented,
        /// <summary>Indicates that the URL presentation was cancelled prematurely by the user.</summary>
        WebContextCancelled,
        /// <summary>Indicates that Dynamic Links in the Firebase Console is not activated.</summary>
        DynamicLinkNotActivated,
        /// <summary>Indicates that the operation was cancelled.</summary>
        Cancelled,
        /// <summary>Indicates that the provider id given for the web operation is invalid.</summary>
        InvalidProviderId,
        /// <summary>Indicates that an internal error occurred during a web operation.</summary>
        WebInternalError,
        /// <summary>Indicates that 3rd party cookies or data are disabled, or that there was
        /// a problem with the browser.</summary>
        WebStorateUnsupported,
        /// <summary>Indicates that the provided tenant ID does not match the Auth instance's
        /// tenant ID.</summary>
        TenantIdMismatch,
        /// <summary>Indicates that a request was made to the backend with an associated tenant
        /// ID for an operation that does not support multi-tenancy.</summary>
        UnsupportedTenantOperation,
        /// <summary>Indicates that an FDL domain used for an out of band code flow is either
        /// not configured or is unauthorized for the current project.</summary>
        InvalidLinkDomain,
        /// <summary>Indicates that credential related request data is invalid. This can occur
        /// when there is a project number mismatch (sessionInfo, spatula header,
        /// temporary proof),
        /// an incorrect temporary proof phone number, or during game center sign in
        /// when the user is
        /// already signed into a different game center account.</summary>
        RejectedCredential,
        /// <summary>Indicates that the phone number provided in the MFA sign in flow to be
        /// verified does not correspond to a phone second factor for the user.</summary>
        PhoneNumberNotFound,
        /// <summary>Indicates that a request was made to the backend with an invalid tenant
        /// ID.</summary>
        InvalidTenantId,
        /// <summary>Indicates that a request was made to the backend without a valid client
        /// identifier.</summary>
        MissingClientIdentifier,
        /// <summary>Indicates that a second factor challenge request was made without proof of
        /// a successful first factor sign-in.</summary>
        MissingMultiFactorSession,
        /// <summary>Indicates that a second factor challenge request was made where a second
        /// factor identifier was not provided.</summary>
        MissingMultiFactorInfo,
        /// <summary>Indicates that a second factor challenge request was made containing an
        /// invalid proof of first factor sign-in.</summary>
        InvalidMultiFactorSession,
        /// <summary>Indicates that the user does not have a second factor matching the
        /// provided identifier.</summary>
        MultiFactorInfoNotFound,
        /// <summary>Indicates that a request was made that is restricted to administrators
        /// only.</summary>
        AdminRestrictedOperation,
        /// <summary>Indicates that the user's email must be verified to perform that request.</summary>
        UnverifiedEmail,
        /// <summary>Indicates that the user is trying to enroll a second factor that already
        /// exists on their account.</summary>
        SecondFactorAlreadyEnrolled,
        /// <summary>Indicates that the user has reached the maximum number of allowed second
        /// factors and is attempting to enroll another one.</summary>
        MaximumSecondFactorCountExceeded,
        /// <summary>Indicates that a user either attempted to enroll in 2FA with an
        /// unsupported first factor or is enrolled and attempts a first factor sign
        /// in that is not supported for 2FA users.</summary>
        UnsupportedFirstFactor,
        /// <summary>Indicates that a second factor users attempted to change their email with
        /// updateEmail instead of verifyBeforeUpdateEmail.</summary>
        EmailChangeNeedsVerification,
        /// <summary>Indicates that the provided event handler is null or invalid.</summary>
        InvalidEventHandler,
        /// <summary>Indicates that the federated provider is busy with a previous
        /// authorization request. Try again when the previous authorization request
        /// completes.</summary>
        FederatedProviderAreadyInUse,
        /// <summary>Indicates that one or more fields of the provided AuthenticatedUserData
        /// are invalid.</summary>
        InvalidAuthenticatedUserData,
        /// <summary>Indicates that an error occurred during a Federated Auth UI Flow when the
        /// user was prompted to enter their credentials.</summary>
        FederatedSignInUserInteractionFailure,
        /// <summary>Indicates that a request was made with a missing or invalid nonce.
        /// This can happen if the hash of the provided raw nonce did not match the
        /// hashed nonce in the OIDC ID token payload.</summary>
        MissingOrInvalidNonce,
        /// <summary>Indicates that the user did not authorize the application during Generic
        /// IDP sign-in.</summary>
        UserCancelled,
        /// <summary>Indicates that a request was made to an unsupported backend endpoint in
        /// passthrough mode.</summary>
        UnsupportedPassthroughOperation,
        /// <summary>Indicates that a token refresh was requested, but neither a refresh token
        /// nor a custom token provider is available.</summary>
        TokenRefreshUnavailable
    }

    private static bool initialized, unavailable;

    public static string AnonymousProviderId => "anonymous";
    public static string EmailAndPasswordProviderId => (string)Firebase.AuthObject.Call("get_email_provider_id");
    public static string PhoneProviderId => (string)Firebase.AuthObject.Call("get_phone_provider_id");
    public static string AppleProviderId => (string)Firebase.AuthObject.Call("get_apple_provider_id");
    public static string FacebookProviderId => (string)Firebase.AuthObject.Call("get_facebook_provider_id");
    public static string GameCenterProviderId => (string)Firebase.AuthObject.Call("get_gamecenter_provider_id");
    public static string GoogleProviderId => (string)Firebase.AuthObject.Call("get_google_provider_id");
    public static string GooglePlayGamesProviderId => (string)Firebase.AuthObject.Call("get_playgames_provider_id");
    public static string MicrosoftProviderId => (string)Firebase.AuthObject.Call("get_microsoft_provider_id");
    public static string TwitterProviderId => (string)Firebase.AuthObject.Call("get_twitter_provider_id");
    public static string XProviderId => (string)Firebase.AuthObject.Call("get_x_provider_id");
    public static string YahooProviderId => (string)Firebase.AuthObject.Call("get_yahoo_provider_id");
    public static string GitHubProviderId => (string)Firebase.AuthObject.Call("get_github_provider_id");
    public static string SnapchatProviderId => "snapchat.com";
    public static string TikTokProviderId => "tiktok.com";
    public static string SteamProviderId => "steampowered.com";
    public static string EpicGamesProviderId => "epicgames.com";

    public static string Token { get; private set; }

    public static bool Init()
    {
        if (initialized) return true;
        if (unavailable) return false;

        if (Firebase.AuthObject == null)
        {
            GD.PushError("Firebase.Auth not found in engine!");
            unavailable = true;
            return false;
        }

        try
        {
            if (!Firebase.AuthObject.IsConnected("auth_state_changed", Callable.From<Dictionary, Dictionary>(OnAuthStateChanged)))
                Firebase.AuthObject.Connect("auth_state_changed", Callable.From<Dictionary, Dictionary>(OnAuthStateChanged));
            if (!Firebase.AuthObject.IsConnected("id_token_changed", Callable.From<string>(OnIdTokenChanged)))
                Firebase.AuthObject.Connect("id_token_changed", Callable.From<string>(OnIdTokenChanged));
            if (!Firebase.AuthObject.IsConnected("phone_verification_completed", Callable.From(OnPhoneVerificationCompleted)))
                Firebase.AuthObject.Connect("phone_verification_completed", Callable.From(OnPhoneVerificationCompleted));
            if (!Firebase.AuthObject.IsConnected("phone_verification_failed", Callable.From<string>(OnPhoneVerificationFailed)))
                Firebase.AuthObject.Connect("phone_verification_failed", Callable.From<string>(OnPhoneVerificationFailed));
            if (!Firebase.AuthObject.IsConnected("verification_sms_sent", Callable.From(OnVerificationSmsSent)))
                Firebase.AuthObject.Connect("verification_sms_sent", Callable.From(OnVerificationSmsSent));
            if (!Firebase.AuthObject.IsConnected("auto_sms_failed", Callable.From(OnAutoSmsFailed)))
                Firebase.AuthObject.Connect("auto_sms_failed", Callable.From(OnAutoSmsFailed));

            if (Firebase.LogDebug) GD.Print("FirebaseAuth initialized.");
        }
        catch (Exception)
        {
            return false;
        }

        initialized = true;
        return initialized;
    }

    private static void OnAuthStateChanged(Dictionary userData, Dictionary credentials) => AuthStateChanged?.Invoke(new(userData), credentials);
    public static event Action<User, Dictionary> AuthStateChanged;
    private static void OnIdTokenChanged(string token) { Token = token; IdTokenChanged?.Invoke(token); }
    public static event Action<string> IdTokenChanged;
    private static void OnPhoneVerificationCompleted() => PhoneVerificationCompleted?.Invoke();
    public static event Action PhoneVerificationCompleted;
    private static void OnPhoneVerificationFailed(string error) => PhoneVerificationFailed?.Invoke(error);
    public static event Action<string> PhoneVerificationFailed;
    private static void OnVerificationSmsSent() => VerificationSmsSent?.Invoke();
    public static event Action VerificationSmsSent;
    private static void OnAutoSmsFailed() => AutoSmsFailed?.Invoke();
    public static event Action AutoSmsFailed;

    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithEmailAndPassword(string email, string password, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_email_and_password", email, password));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithEmailAndPasswordAsync(string email, string password)
    {
        TaskCompletionSource<User> source = new();
        SignInWithEmailAndPassword(email, password,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInAnonymously(Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_anonymously"));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInAnonymouslyAsync()
    {
        TaskCompletionSource<User> source = new();
        SignInAnonymously(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithCredentialFrom(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_credential_from", providerId));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithCredentialFromAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        SignInWithCredentialFrom(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithCustomToken(string token, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_custom_token", token));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithCustomTokenAsync(string token)
    {
        TaskCompletionSource<User> source = new();
        SignInWithCustomToken(token,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithProvider(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_provider", providerId));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithProviderAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        SignInWithProvider(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static void SetLanguageCode(string languageCode) => Firebase.AuthObject.Call("set_language_code", languageCode);
    /// <summary>
    /// <b>Result:</b> signed in new user data
    /// </summary>
    public static CreateUserTask CreateUserWithEmailAndPassword(string email, string password, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new CreateUserTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("create_user_with_email_and_password", email, password));
    /// <summary>
    /// <b>Result:</b> signed in new user data
    /// </summary>
    public static Task<User> CreateUserWithEmailAndPasswordAsync(string email, string password)
    {
        TaskCompletionSource<User> source = new();
        CreateUserWithEmailAndPassword(email, password,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static Dictionary GetUser() => (Dictionary)Firebase.AuthObject.Call("get_user");
    public static DeleteUserTask UserDelete(Action onCompleted = null, Action<Error, string> onError = null)
        => new DeleteUserTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_delete"));
    public static Task UserDeleteAsync()
    {
        TaskCompletionSource source = new();
        UserDelete(
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static GetAuthTokenTask UserGetToken(bool forceRefresh, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new GetAuthTokenTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_get_token", forceRefresh));
    public static Task<string> UserGetTokenAsync(bool forceRefresh)
    {
        TaskCompletionSource<string> source = new();
        UserGetToken(forceRefresh,
            (result) => { Token = result; source.TrySetResult(result); },
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithCredentialFrom(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_credential_from", providerId));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithCredentialFromAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithCredentialFrom(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithProvider(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_provider", providerId));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithProviderAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithProvider(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask UserReauthenticateWithCredentialFrom(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_reauthenticate_with_credential_from", providerId));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> UserReauthenticateWithCredentialFromAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        UserReauthenticateWithCredentialFrom(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask UserReauthenticateWithProvider(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_reauthenticate_with_provider", providerId));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> UserReauthenticateWithProviderAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        UserReauthenticateWithProvider(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static UserReloadTask UserReload(Action onCompleted = null, Action<Error, string> onError = null)
        => new UserReloadTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_reload"));
    public static Task UserReloadAsync()
    {
        TaskCompletionSource source = new();
        UserReload(
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static UserSendEmailVerificationTask UserSendEmailVerification(Action onCompleted = null, Action<Error, string> onError = null)
        => new UserSendEmailVerificationTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_send_email_verification"));
    public static Task UserSendEmailVerificationAsync()
    {
        TaskCompletionSource source = new();
        UserSendEmailVerification(
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> remaining user data
    /// </summary>
    public static UserUnlinkTask UserUnlink(string providerId, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserUnlinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_unlink", providerId));
    /// <summary>
    /// <b>Result:</b> remaining user data
    /// </summary>
    public static Task<User> UserUnlinkAsync(string providerId)
    {
        TaskCompletionSource<User> source = new();
        UserUnlink(providerId,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> updated email
    /// </summary>
    public static UserUpdateEmailTask UserUpdateEmail(string email, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new UserUpdateEmailTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_update_email", email));
    /// <summary>
    /// <b>Result:</b> updated email
    /// </summary>
    public static Task<string> UserUpdateEmailAsync(string email)
    {
        TaskCompletionSource<string> source = new();
        UserUpdateEmail(email,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> updated password
    /// </summary>
    public static UserUpdatePasswordTask UserUpdatePassword(string password, Action<string> onCompleted = null, Action<Error, string> onError = null)
        => new UserUpdatePasswordTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_update_password", password));
    /// <summary>
    /// <b>Result:</b> updated password
    /// </summary>
    public static Task<string> UserUpdatePasswordAsync(string password)
    {
        TaskCompletionSource<string> source = new();
        UserUpdatePassword(password,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> updated user data
    /// </summary>
    public static UserUpdatePhoneNumberTask UserUpdatePhoneNumberCredential(Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserUpdatePhoneNumberTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_update_phone_number_credential"));
    /// <summary>
    /// <b>Result:</b> updated user data
    /// </summary>
    public static Task<User> UserUpdatePhoneNumberCredentialAsync()
    {
        TaskCompletionSource<User> source = new();
        UserUpdatePhoneNumberCredential(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> displayName, photoUrl
    /// </summary>
    public static UserUpdateProfileTask UserUpdateProfile(string displayName, string photoUrl, Action<string, string> onCompleted = null, Action<Error, string> onError = null)
        => new UserUpdateProfileTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_update_profile", displayName, photoUrl));
    /// <summary>
    /// <b>Result:</b> displayName, photoUrl
    /// </summary>
    public static Task<(string, string)> UserUpdateProfileAsync(string displayName, string photoUrl)
    {
        TaskCompletionSource<(string, string)> source = new();
        UserUpdateProfile(displayName, photoUrl,
            (name, url) => source.TrySetResult((name, url)),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static string GetLanguageCode() => (string)Firebase.AuthObject.Call("get_language_code");
    public static SendPasswordResetTask SendPasswordResetEmail(string email, Action onCompleted = null, Action<Error, string> onError = null)
        => new SendPasswordResetTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("send_password_reset_email", email));
    public static Task SendPasswordResetEmailAsync(string email)
    {
        TaskCompletionSource source = new();
        SendPasswordResetEmail(email,
            () => source.TrySetResult(),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static void SignOut() => Firebase.AuthObject.Call("sign_out");
    public static void UseEmulator(string host, int port) => Firebase.AuthObject.Call("use_emulator", host, port);

    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithApple(string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_apple", token, nonce));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithAppleAsync(string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        SignInWithApple(token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithFacebook(string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_facebook", accessToken));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithFacebookAsync(string accessToken)
    {
        TaskCompletionSource<User> source = new();
        SignInWithFacebook(accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static bool IsGamecenterSignedIn() => (bool)Firebase.AuthObject.Call("is_gamecenter_signed_in");
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithGameCenter(Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_gamecenter"));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithGameCenterAsync()
    {
        TaskCompletionSource<User> source = new();
        SignInWithGameCenter(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithGitHub(string token, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_github", token));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithGitHubAsync(string token)
    {
        TaskCompletionSource<User> source = new();
        SignInWithGitHub(token,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithGoogle(string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_google", idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithGoogleAsync(string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        SignInWithGoogle(idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithMicrosoft(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_microsoft", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithMicrosoftAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        SignInWithMicrosoft(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithPlayGames(string serverAuthCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_playgames", serverAuthCode));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithPlayGamesAsync(string serverAuthCode)
    {
        TaskCompletionSource<User> source = new();
        SignInWithPlayGames(serverAuthCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithTwitter(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_twitter", token, secret));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithTwitterAsync(string token, string secret)
    {
        TaskCompletionSource<User> source = new();
        SignInWithTwitter(token, secret,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithX(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_x", token, secret));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithXAsync(string token, string secret)
    {
        TaskCompletionSource<User> source = new();
        SignInWithX(token, secret,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithYahoo(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_yahoo", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithYahooAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        SignInWithYahoo(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithOAuth(string providerId, string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_oauth", providerId, idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithOAuthAsync(string providerId, string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        SignInWithOAuth(providerId, idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithOAuthNonce(string providerId, string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_oauth_nonce", providerId, token, nonce));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithOAuthNonceAsync(string providerId, string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        SignInWithOAuthNonce(providerId, token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    public static void VerifyPhoneNumber(bool resend, string phoneNumber) => Firebase.AuthObject.Call("verify_phone_number", resend, phoneNumber);
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static SignInTask SignInWithPhoneNumber(string smsCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new SignInTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("sign_in_with_phone_number", smsCode));
    /// <summary>
    /// <b>Result:</b> signed in user data
    /// </summary>
    public static Task<User> SignInWithPhoneNumberAsync(string smsCode)
    {
        TaskCompletionSource<User> source = new();
        SignInWithPhoneNumber(smsCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }

    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithApple(string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_apple", token, nonce));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithAppleAsync(string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithApple(token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithFacebook(string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_facebook", accessToken));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithFacebookAsync(string accessToken)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithFacebook(accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithGamecenter(Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_gamecenter"));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithGamecenterAsync()
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithGamecenter(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithGithub(string token, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_github", token));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithGithubAsync(string token)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithGithub(token,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithGoogle(string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_google", idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithGoogleAsync(string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithGoogle(idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithMicrosoft(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_microsoft", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithMicrosoftAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithMicrosoft(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithPlaygames(string serverAuthCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_playgames", serverAuthCode));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithPlaygamesAsync(string serverAuthCode)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithPlaygames(serverAuthCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithTwitter(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_twitter", token, secret));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithTwitterAsync(string token, string secret)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithTwitter(token, secret,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithX(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_x", token, secret));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithXAsync(string token, string secret)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithX(token, secret,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithYahoo(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_yahoo", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithYahooAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithYahoo(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithOauth(string providerId, string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_oauth", providerId, idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithOauthAsync(string providerId, string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithOauth(providerId, idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithOauthNonce(string providerId, string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_oauth_nonce", providerId, token, nonce));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithOauthNonceAsync(string providerId, string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithOauthNonce(providerId, token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithEmail(string email, string password, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_email", email, password));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithEmailAsync(string email, string password)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithEmail(email, password,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static UserLinkTask UserLinkWithPhoneNumber(string smsCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserLinkTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("user_link_with_phone_number", smsCode));
    /// <summary>
    /// <b>Result:</b> linked user data
    /// </summary>
    public static Task<User> UserLinkWithPhoneNumberAsync(string smsCode)
    {
        TaskCompletionSource<User> source = new();
        UserLinkWithPhoneNumber(smsCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }

    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithApple(string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_apple", token, nonce));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithAppleAsync(string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithApple(token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithFacebook(string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_facebook", accessToken));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithFacebookAsync(string accessToken)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithFacebook(accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithGamecenter(Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_gamecenter"));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithGamecenterAsync()
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithGamecenter(
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithGithub(string token, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_github", token));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithGithubAsync(string token)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithGithub(token,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithGoogle(string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_google", idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithGoogleAsync(string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithGoogle(idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithMicrosoft(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_microsoft", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithMicrosoftAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithMicrosoft(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithPlaygames(string serverAuthCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_playgames", serverAuthCode));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithPlaygamesAsync(string serverAuthCode)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithPlaygames(serverAuthCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithTwitter(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => ReauthenticateWithX(token, secret, onCompleted, onError);
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithTwitterAsync(string token, string secret)
        => ReauthenticateWithXAsync(token, secret);
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithX(string token, string secret, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_x", token, secret));
    public static Task<User> ReauthenticateWithXAsync(string token, string secret)
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithX(token, secret,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithYahoo(Godot.Collections.Array scopes, Dictionary customParameters, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_yahoo", scopes, customParameters));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithYahooAsync(Godot.Collections.Array scopes, Dictionary customParameters)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithYahoo(scopes, customParameters,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithOauth(string providerId, string idToken, string accessToken, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_oauth", providerId, idToken, accessToken));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithOauthAsync(string providerId, string idToken, string accessToken)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithOauth(providerId, idToken, accessToken,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithOauthNonce(string providerId, string token, string nonce, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_oauth_nonce", providerId, token, nonce));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithOauthNonceAsync(string providerId, string token, string nonce)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithOauthNonce(providerId, token, nonce,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithEmail(string email, string password, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_email", email, password));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithEmailAsync(string email, string password)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithEmail(email, password,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static UserReauthenticateTask ReauthenticateWithPhoneNumber(string smsCode, Action<User> onCompleted = null, Action<Error, string> onError = null)
        => new UserReauthenticateTask(onCompleted, onError).Set((GodotObject)Firebase.AuthObject.Call("reauthenticate_with_phone_number", smsCode));
    /// <summary>
    /// <b>Result:</b> reauthenticated user data
    /// </summary>
    public static Task<User> ReauthenticateWithPhoneNumberAsync(string smsCode)
    {
        TaskCompletionSource<User> source = new();
        ReauthenticateWithPhoneNumber(smsCode,
            (result) => source.TrySetResult(result),
            (error, message) => source.TrySetException(new FirebaseAuthException(error, message)));
        return source.Task;
    }

    public static LoginProvider ProviderFromString(string providerId)
    {
        if (providerId == AnonymousProviderId) return LoginProvider.Anonymous;
        else if (providerId == EmailAndPasswordProviderId) return LoginProvider.EmailAndPassword;
        else if (providerId == PhoneProviderId) return LoginProvider.Phone;
        else if (providerId == AppleProviderId) return LoginProvider.Apple;
        else if (providerId == FacebookProviderId) return LoginProvider.Facebook;
        else if (providerId == GameCenterProviderId) return LoginProvider.GameCenter;
        else if (providerId == GoogleProviderId) return LoginProvider.Google;
        else if (providerId == GooglePlayGamesProviderId) return LoginProvider.GooglePlayGames;
        else if (providerId == MicrosoftProviderId) return LoginProvider.Microsoft;
        else if (providerId == XProviderId || providerId == TwitterProviderId) return LoginProvider.X;
        else if (providerId == YahooProviderId) return LoginProvider.Yahoo;
        else if (providerId == GitHubProviderId) return LoginProvider.GitHub;
        else if (providerId == SnapchatProviderId) return LoginProvider.Snapchat;
        else if (providerId == TikTokProviderId) return LoginProvider.TikTok;
        else if (providerId == SteamProviderId) return LoginProvider.Steam;
        else if (providerId == EpicGamesProviderId) return LoginProvider.EpicGames;
        else return LoginProvider.Other;
    }
}
