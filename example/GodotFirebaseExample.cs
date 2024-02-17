using Godot;
using Firebase;
using Godot.Collections;

[Icon("res://Firebase.svg")]
public partial class GodotFirebaseExample : Node
{
    private static GodotFirebaseExample instance;
    [Export] private Label logLabel;
    [Export] private LineEdit username, password;
    private User user;
    private string fcmToken;
    public static string GetToken => instance.fcmToken;

    public GodotFirebaseExample() => instance = this;

    private void AddLog(string line)
    {
        CallDeferred("DoAddLog", line);
    }

    private void DoAddLog(string line)
    {
        GD.Print(line);
        logLabel.Text += $"{line}\n";
    }

    public override void _Ready()
    {
        AddLog("Initializing Firebase...");

        if (OS.GetName() is "Android" or "iOS")
            Firebase.Firebase.Init();
        else
            Firebase.Firebase.InitWith(FileAccess.GetFileAsString("res://google-services.json"));

        AddLog("Initializing Firebase Authentication...");

        Auth.Init();

        FirestoreFilter filter = FirestoreFilter.GreaterThan("trees", 50);
        GD.Print(filter.ToString());
    }

    public void OnLoginPressed()
    {
        AddLog("Signing in...");

        SignInTask task = Auth.SignInWithEmailAndPassword(username.Text, password.Text,
            (userResult) => { user = userResult; CallDeferred("OnSignin"); },
            (error, errorMessage) => AddLog($"Error {error} signing in: {errorMessage}"));
    }

    FirestoreListener docListener;
    private void OnSignin()
    {
        AddLog($"Signed in as {user.DisplayName}.");

        Database.Init();

        Firestore.Init();
        GetDocumentTask getTask = Firestore.GetDocument($"/users/{user.Uid}",
            document => AddLog($"Retrieved document:\n{document}"),
            (error, errorMessage) => AddLog($"Error {error} retrieving document: {errorMessage}"));

        string deviceid = "JwkkxPkc84";
        docListener = Firestore.ListenToDocument($"/users/{user.Uid}/d/{deviceid}",
            (docPath, document) => AddLog($"Got updated document {docPath}:\n{document}"),
            (docPath, error, errorMessage) => AddLog($"Error {error} listening to document {docPath}: {errorMessage}"));

        Functions.Init();
        FirebaseFunction call = Functions.Call(GetTree(), "friendSearch", new() { { "s", "John" } },
            (name, data)
                => AddLog($"Result of function {name}: {data}"),
            (error, error_message)
                => AddLog($"Error {error} calling function 'friendSearch': {error_message}")
            );

        Messaging.TokenReceived += OnToken;
        Messaging.MessageReceived += OnMessage;
        Messaging.Init();
        AddLog($"FirebaseCloudMessaging inited");

        Storage.Init();
        // Wait for signin process to finish registering
        CreateTween().TweenCallback(Callable.From(() => CallDeferred("GoStorage"))).SetDelay(1.0f);
    }

    private void GoStorage()
    {
        GetDownloadUrlTask urlTask = Storage.GetDownloadUrl("/avatars/DemoP/avatar.jpg",
            url => AddLog($"Download url for DemoP.jpg: {url}"),
            (error, errorMessage) => AddLog($"Error {error} getting download url: {errorMessage}"));
    }

    private void OnToken(string token)
    {
        if (token.Length > 0)
        {
            fcmToken = token;
            AddLog($"FCM token: {token}");
        }
    }

    private void OnMessage(Dictionary message)
    {
        AddLog($"FCM message: {message}");
    }

    public override void _ExitTree()
    {
        docListener.Dispose();
    }
}