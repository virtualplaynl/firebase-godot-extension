#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#if __EMSCRIPTEN__
#define WEB_PLATFORM
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define IOS_PLATFORM
#else
#define MACOS_PLATFORM
#endif
#elif defined(_WIN32) || defined(WIN32)
#define WINDOWS_PLATFORM
#elif defined(__linux__) || defined(__unix__) || defined(__unix) || defined(unix)
#define LINUX_PLATFORM
#endif
#include "GodotFirebase.h"

static GodotFirebase *firebasePtr = NULL;

void initialize_firebase_module(ModuleInitializationLevel p_level)
{
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        ClassDB::register_class<GodotFirebase>();
        ClassDB::register_class<GodotFirebaseAnalytics>();
        ClassDB::register_class<GodotFirebaseAuth>();
        ClassDB::register_class<GodotFirebaseDatabase>();
        ClassDB::register_class<GodotFirestore>();
        ClassDB::register_class<FirestoreBatch>();
        ClassDB::register_class<FirestoreFilter>();
        ClassDB::register_class<FirestoreListener>();
        ClassDB::register_class<FirestoreQuery>();
        ClassDB::register_class<FirestoreTransaction>();
        ClassDB::register_class<GodotFirebaseFunctions>();
        ClassDB::register_class<GodotFirebaseMessaging>();
        ClassDB::register_class<GodotFirebaseRemoteConfig>();
        ClassDB::register_class<GodotFirebaseStorage>();

        ClassDB::register_class<DeleteUserTask>();
        ClassDB::register_class<SignInTask>();
        ClassDB::register_class<CreateUserTask>();
        ClassDB::register_class<GetAuthTokenTask>();
        ClassDB::register_class<UserLinkTask>();
        ClassDB::register_class<UserReauthenticateTask>();
        ClassDB::register_class<UserReloadTask>();
        ClassDB::register_class<UserSendEmailVerificationTask>();
        ClassDB::register_class<UserUnlinkTask>();
        ClassDB::register_class<UserUpdateEmailTask>();
        ClassDB::register_class<UserUpdatePasswordTask>();
        ClassDB::register_class<UserUpdatePhoneNumberTask>();
        ClassDB::register_class<UserUpdateProfileTask>();
        ClassDB::register_class<SendPasswordResetTask>();

        ClassDB::register_class<GetFCMTokenTask>();
        ClassDB::register_class<DeleteTokenTask>();
        ClassDB::register_class<SubscribeTask>();
        ClassDB::register_class<UnsubscribeTask>();

        ClassDB::register_class<LoadBundleTask>();
        ClassDB::register_class<GetNamedQueryTask>();
        ClassDB::register_class<WaitForPendingWritesTask>();
        ClassDB::register_class<AddDocumentTask>();
        ClassDB::register_class<CollectionExistsTask>();
        ClassDB::register_class<DeleteDocumentTask>();
        ClassDB::register_class<GetDocumentTask>();
        ClassDB::register_class<DocumentExistsTask>();
        ClassDB::register_class<SetDocumentTask>();
        ClassDB::register_class<UpdateDocumentTask>();

        ClassDB::register_class<FirebaseFunction>();

        ClassDB::register_class<StorageDeleteTask>();
        ClassDB::register_class<GetBytesTask>();
        ClassDB::register_class<GetDownloadUrlTask>();
        ClassDB::register_class<GetFileTask>();
        ClassDB::register_class<GetMetadataTask>();
        ClassDB::register_class<PutBytesTask>();
        ClassDB::register_class<PutFileTask>();
        ClassDB::register_class<UpdateMetadataTask>();

        firebasePtr = memnew(GodotFirebase);
        Engine::get_singleton()->register_singleton("Firebase", GodotFirebase::get_singleton());
    }
}

void uninitialize_firebase_module(ModuleInitializationLevel p_level)
{
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        memdelete(firebasePtr);
    }
}

extern "C"
{
    GDExtensionBool GDE_EXPORT firebase_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_firebase_module);
        init_obj.register_terminator(uninitialize_firebase_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
