@icon("res://Firebase.svg")
extends Node

@export
var username: LineEdit
@export
var password: LineEdit

@export
var logLabel: Label

var Auth: GodotFirebaseAuth
var Database: GodotFirebaseDatabase
var Firestore: GodotFirestore
var Functions: GodotFirebaseFunctions
var Messaging: GodotFirebaseMessaging

var auth_data: Dictionary

var _token: String

func add_log(line: String):
	call_deferred("_add_log", line)

func _add_log(line: String):
	print(line)
	logLabel.text += line + "\n"

func _ready():
	if Engine.has_singleton('Firebase'):
		add_log("Initializing Firebase...")
		if OS.get_name() in ["Android", "iOS"]:
			Firebase.init()
		else:
			Firebase.init_with(FileAccess.get_file_as_string("res://google-services.json"))
		add_log("Initializing Firebase Authentication...")
		Auth = Firebase.auth()
	else:
		push_warning('GodotFirebase module not found!')

func _on_button_pressed():
	add_log("Signing in...")
	var task: SignInTask = Auth.sign_in_with_email_and_password(username.text, password.text)
	task.sign_in_completed.connect(_on_signin)
	task.sign_in_error.connect(_on_signin_error)


func _on_signin(in_auth_data: Dictionary):
	auth_data = in_auth_data
	add_log("Signed in as %s" % auth_data["display_name"])
	add_log(str(auth_data))
	call_deferred("firestore_demo")

func firestore_demo():
	Firestore = Firebase.firestore()
	Firestore.get_document_completed.connect(_on_get_document)
	Firestore.get_document_error.connect(_on_get_document_error)
	Firestore.get_document("/users/%s" % auth_data["uid"])
	var deviceid = "JwkkxPkc84"
	var listener = Firestore.listen_to_document("/users/%s/d/%s" % [auth_data["uid"], deviceid])
	listener.document_update.connect(_on_document_update)
	listener.document_listen_error.connect(_on_listen_error)
	var query:FirestoreQuery = Firestore.collection_query("/users")
	print("Firestore:", Firestore.get_class())
	print("Query:", query)
	call_deferred("messaging_demo")

func messaging_demo():
	Messaging = Firebase.messaging();
	Messaging.token_received.connect(_on_token)
	Messaging.message_received.connect(_on_message)
	print('FirebaseCloudMessaging inited')
	Messaging.request_permission()
	call_deferred("database_demo")
	
func database_demo():
	Database = Firebase.database();
	print('FirebaseDatabase inited')
	call_deferred("functions_demo")
	
func functions_demo():
	Functions = Firebase.functions();
	print('FirebaseFunctions inited')

func _on_signin_error(error: int, error_message: String):
	add_log("Error %d signing in: %s" % [error, error_message])

func _on_get_document(id: int, document: Dictionary):
	add_log("Retrieved document %d:\n%s" % [id, document])

func _on_get_document_error(id: int, error: int, error_message: String):
	add_log("Error %d retrieving document %d: %s" % [error, id, error_message])

func _on_document_update(id: int, doc_path: String, document: Dictionary):
	add_log("Got updated document (%s) from listener %d:\n%s" % [doc_path, id, document])

func _on_listen_error(id: int, doc_path: String, error: int, error_message: String):
	add_log("Error %d listening to document (%s) from listener %d: %s" % [error, doc_path, id, error_message])

func _on_registration_completed(token: String):
	if token.length() > 0:
		_token = token
		add_log('Registration complete, token: %s' % token)

func _on_token(token: String):
	if token.length() > 0:
		_token = token
		add_log('FCM token: %s' % token)

func _on_message(message: Dictionary):
	add_log('FCM message: %s' % str(message))

func get_token():
	return _token
