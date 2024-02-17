#include "GodotFirestore.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

GFB_CALLBACK4_CPP(LoadBundleTask, load_bundle, int, INT, bytes_loaded, int, INT, documents_loaded, int, INT, total_bytes, int, INT, total_documents);
GFB_CALLBACK1_CPP(GetNamedQueryTask, get_named_query, FirestoreQuery *, OBJECT, query);
GFB_CALLBACK0_CPP(WaitForPendingWritesTask, wait_for_pending_writes);
GFB_CALLBACK0_CPP(AddDocumentTask, add_document);
GFB_CALLBACK1_CPP(CollectionExistsTask, collection_exists, int, INT, document_count);
GFB_CALLBACK0_CPP(DeleteDocumentTask, delete_document);
GFB_CALLBACK1_CPP(GetDocumentTask, get_document, Dictionary, DICTIONARY, document);
GFB_CALLBACK1_CPP(DocumentExistsTask, document_exists, bool, BOOL, exists);
GFB_CALLBACK0_CPP(SetDocumentTask, set_document);
GFB_CALLBACK0_CPP(UpdateDocumentTask, update_document);

GodotFirestore::GodotFirestore()
{
    firestore = nullptr;
}

GodotFirestore::~GodotFirestore()
{
}

void GodotFirestore::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Firestore...");
    firestore = Firestore::GetInstance(app);
}

Firestore *GodotFirestore::getFirestore()
{
    return firestore;
}

FirestoreListener *GodotFirestore::listenToSnapshotsInSync()
{
    FirestoreListener *listener = memnew(FirestoreListener);
    listener->setToInSync(firestore->AddSnapshotsInSyncListener([listener](){
        listener->inSyncCallback();
        }));
    return listener;
}
FirestoreBatch *GodotFirestore::batch()
{
    FirestoreBatch *batch = memnew(FirestoreBatch);
    batch->init(firestore);
    return batch;
}
void GodotFirestore::clearPersistence()
{
    firestore->ClearPersistence();
}
void GodotFirestore::disableNetwork()
{
    firestore->DisableNetwork();
}
void GodotFirestore::enableNetwork()
{
    firestore->EnableNetwork();
}
LoadBundleTask *GodotFirestore::loadBundle(String bundle)
{
    LoadBundleTask *task = memnew(LoadBundleTask);
    firestore->LoadBundle(bundle.utf8().get_data())
        .OnCompletion([task](Future<LoadBundleTaskProgress> completedTask)
        {
            if(completedTask.error() == 0)
                task->completed(completedTask.result()->bytes_loaded(), completedTask.result()->documents_loaded(), completedTask.result()->total_bytes(), completedTask.result()->total_documents());
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
GetNamedQueryTask *GodotFirestore::namedQuery(String queryName)
{
    GetNamedQueryTask *task = memnew(GetNamedQueryTask);
    firestore->NamedQuery(queryName.utf8().get_data())
        .OnCompletion([task, queryName](Future<firestore::Query> completedTask)
        {
            if(completedTask.error() == 0)
            {
                FirestoreQuery *query = memnew(FirestoreQuery);
                query->setQuery(*completedTask.result(), queryName);
                task->completed(query);
            }
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
void GodotFirestore::runTransaction(FirestoreTransaction *transactionRunner, int maxAttempts)
{
    TransactionOptions options = TransactionOptions();
    options.set_max_attempts(maxAttempts);
    firestore->RunTransaction(options, [this, transactionRunner](Transaction &transaction, std::string &errorMessage)
    {
        return transactionRunner->run(firestore, transaction, errorMessage);
    });
}
void GodotFirestore::setLogLevel(int level)
{
    firestore->set_log_level((LogLevel)level);
}
void GodotFirestore::setSettings(Dictionary settings)
{
    Settings firestoreSettings = Settings();
    if (settings.has("cache_size_bytes"))
        firestoreSettings.set_cache_size_bytes(settings["cache_size_bytes"]);
    if (settings.has("host"))
        firestoreSettings.set_host(((String)settings["host"]).utf8().get_data());
    if (settings.has("persistence_enabled"))
        firestoreSettings.set_persistence_enabled(settings["persistence_enabled"]);
    if (settings.has("ssl_enabled"))
        firestoreSettings.set_ssl_enabled(settings["ssl_enabled"]);
    firestore->set_settings(firestoreSettings);
}
Dictionary GodotFirestore::getSettings()
{
    Settings firestoreSettings = firestore->settings();
    Dictionary settings = Dictionary();
    settings["cache_size_bytes"] = firestoreSettings.cache_size_bytes();
    settings["host"] = firestoreSettings.host().c_str();
    settings["persistence_enabled"] = firestoreSettings.is_persistence_enabled();
    settings["ssl_enabled"] = firestoreSettings.is_ssl_enabled();

    return settings;
}
void GodotFirestore::terminate()
{
    firestore->Terminate();
}
WaitForPendingWritesTask *GodotFirestore::waitForPendingWrites()
{
    WaitForPendingWritesTask *task = memnew(WaitForPendingWritesTask);
    firestore->WaitForPendingWrites()
        .OnCompletion([task](Future<void> completedTask)
        {
            if(completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}

FirestoreListener *GodotFirestore::listenToCollection(String collectionPath)
{
    std::string key = std::string(collectionPath.utf8().get_data());

    FirestoreListener *listener = memnew(FirestoreListener);

    ListenerRegistration registration = firestore->Collection(key).AddSnapshotListener(
        [listener](const QuerySnapshot &snapshot, firestore::Error error, const std::string &errorMessage)
        {
            listener->collectionUpdated(snapshot, error, errorMessage);
        });

    listener->setToCollection(collectionPath, registration);

    return listener;
}
AddDocumentTask *GodotFirestore::addDocument(String collectionPath, Dictionary fields)
{
    AddDocumentTask *task = memnew(AddDocumentTask);
    firestore->Collection(collectionPath.utf8().get_data()).Add(dictToMap(fields))
        .OnCompletion([task](Future<DocumentReference> completedTask)
        {
            if(completedTask.error() == 0) {
                const DocumentReference *ref = completedTask.result();

                task->completed();
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
String GodotFirestore::newDocumentIn(String collectionPath)
{
    return firestore->Collection(collectionPath.utf8().get_data()).Document().id().c_str();
}
FirestoreQuery *GodotFirestore::collectionQuery(String collectionPath)
{
    FirestoreQuery *query = memnew(FirestoreQuery);
    query->setQuery(firestore->Collection(collectionPath.utf8().get_data()), collectionPath);

    return query;
}
CollectionExistsTask *GodotFirestore::collectionExists(String collectionPath)
{
    CollectionExistsTask *task = memnew(CollectionExistsTask);
    firestore->Collection(collectionPath.utf8().ptr()).Count().Get(AggregateSource::kServer)
        .OnCompletion([task](const Future<AggregateQuerySnapshot> &completedTask)
        {
            if(completedTask.error() == 0)
            {
                task->completed(completedTask.result()->count());
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
String GodotFirestore::collectionParent(String collectionPath)
{
    DocumentReference doc = firestore->Collection(collectionPath.utf8().ptr()).Parent();

    return doc.is_valid() ? doc.path().c_str() : "/";
}

FirestoreListener *GodotFirestore::listenToCollectionGroup(String collectionId)
{
    std::string key = std::string(collectionId.utf8().get_data());

    FirestoreListener *listener = memnew(FirestoreListener);

    ListenerRegistration registration = firestore->CollectionGroup(key).AddSnapshotListener(
        [listener](const QuerySnapshot &snapshot, firestore::Error error, const std::string &errorMessage)
        {
            listener->collectionGroupUpdated(snapshot, error, errorMessage);
        });

    listener->setToCollectionGroup(collectionId, registration);

    return listener;
}
FirestoreQuery *GodotFirestore::collectionGroupQuery(String collectionId)
{
    FirestoreQuery *query = memnew(FirestoreQuery);
    query->setQuery(firestore->CollectionGroup(collectionId.utf8().get_data()), collectionId);

    return query;
}

FirestoreListener *GodotFirestore::listenToDocument(String docPath)
{
    std::string key = std::string(docPath.utf8().get_data());

    FirestoreListener *listener = memnew(FirestoreListener);

    ListenerRegistration registration = firestore->Document(key).AddSnapshotListener(
        [listener](const DocumentSnapshot &snapshot, firestore::Error error, const std::string &errorMessage)
        {
            listener->docUpdated(snapshot, error, errorMessage);
        });

    listener->setToDocument(docPath, registration);

    return listener;
}
DeleteDocumentTask *GodotFirestore::deleteDocument(String docPath)
{
    DeleteDocumentTask *task = memnew(DeleteDocumentTask);
    firestore->Document(docPath.utf8().ptr()).Delete()
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
            {
                task->completed();
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
GetDocumentTask *GodotFirestore::getDocument(String docPath)
{
    GetDocumentTask *task = memnew(GetDocumentTask);
    firestore->Document(docPath.utf8().ptr()).Get()
        .OnCompletion([task](const Future<DocumentSnapshot> &completedTask)
        {
            if(completedTask.error() == 0) {
                const DocumentSnapshot *snapshot = completedTask.result();
                MapFieldValue data = snapshot->GetData();
                Dictionary dataDict = mapToDict(data);

                task->completed(dataDict);
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
DocumentExistsTask *GodotFirestore::documentExists(String docPath)
{
    DocumentExistsTask *task = memnew(DocumentExistsTask);
    firestore->Document(docPath.utf8().ptr()).Get()
        .OnCompletion([task](const Future<DocumentSnapshot> &completedTask)
        {
            if(completedTask.error() == 0)
            {
                task->completed(completedTask.result()->exists());
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
String GodotFirestore::documentParent(String docPath)
{
    return firestore->Document(docPath.utf8().ptr()).Parent().path().c_str();
}

SetDocumentTask *GodotFirestore::setDocument(String docPath, Dictionary fields, SetOptions options, Array mergefields)
{
    firestore::SetOptions setOptions;
    if (options == kOVERWRITE)
        setOptions = firestore::SetOptions();
    else if (options == kMERGE_SPECIFIC)
        setOptions = firestore::SetOptions::MergeFields(arrayToStringVector(mergefields));
    else
        setOptions = firestore::SetOptions::Merge();

    SetDocumentTask *task = memnew(SetDocumentTask);
    firestore->Document(docPath.utf8().ptr()).Set(dictToMap(fields), setOptions)
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if (completedTask.error() == 0) {
                task->completed();
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
String GodotFirestore::getDocumentString(String docPath)
{
    return firestore->Document(docPath.utf8().ptr()).ToString().c_str();
}
UpdateDocumentTask *GodotFirestore::updateDocument(String docPath, Dictionary changes)
{
    UpdateDocumentTask *task = memnew(UpdateDocumentTask);
    firestore->Document(docPath.utf8().ptr()).Update(dictToMap(changes))
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if (completedTask.error() == 0) {
                task->completed();
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}

void GodotFirestore::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("listen_to_snapshots_in_sync"), &GodotFirestore::listenToSnapshotsInSync);
    ClassDB::bind_method(D_METHOD("batch"), &GodotFirestore::batch);
    ClassDB::bind_method(D_METHOD("clear_persistence"), &GodotFirestore::clearPersistence);
    ClassDB::bind_method(D_METHOD("disable_network"), &GodotFirestore::disableNetwork);
    ClassDB::bind_method(D_METHOD("enable_network"), &GodotFirestore::enableNetwork);
    ClassDB::bind_method(D_METHOD("load_bundle", "bundle"), &GodotFirestore::loadBundle);
    ClassDB::bind_method(D_METHOD("named_query", "query_name"), &GodotFirestore::namedQuery);
    ClassDB::bind_method(D_METHOD("run_transaction", "transaction_runner", "max_attempts"), &GodotFirestore::runTransaction);
    ClassDB::bind_method(D_METHOD("set_log_level", "level"), &GodotFirestore::setLogLevel);
    ClassDB::bind_method(D_METHOD("set_settings", "settings"), &GodotFirestore::setSettings);
    ClassDB::bind_method(D_METHOD("get_settings"), &GodotFirestore::getSettings);
    ClassDB::bind_method(D_METHOD("terminate"), &GodotFirestore::terminate);
    ClassDB::bind_method(D_METHOD("wait_for_pending_writes"), &GodotFirestore::waitForPendingWrites);

    ClassDB::bind_method(D_METHOD("listen_to_collection", "collection_path"), &GodotFirestore::listenToCollection);
    ClassDB::bind_method(D_METHOD("add_document", "doc_path", "fields"), &GodotFirestore::addDocument);
    ClassDB::bind_method(D_METHOD("new_document_in", "collection_path"), &GodotFirestore::newDocumentIn);
    ClassDB::bind_method(D_METHOD("collection_query", "collection_path"), &GodotFirestore::collectionQuery);
    ClassDB::bind_method(D_METHOD("collection_exists", "collection_path"), &GodotFirestore::collectionExists);
    ClassDB::bind_method(D_METHOD("collection_parent", "collection_path"), &GodotFirestore::collectionParent);

    ClassDB::bind_method(D_METHOD("listen_to_collection_group", "collection_id"), &GodotFirestore::listenToCollectionGroup);
    ClassDB::bind_method(D_METHOD("collection_group_query", "collection_id"), &GodotFirestore::collectionGroupQuery);

    ClassDB::bind_method(D_METHOD("listen_to_document", "doc_path"), &GodotFirestore::listenToDocument);
    ClassDB::bind_method(D_METHOD("delete_document", "doc_path"), &GodotFirestore::deleteDocument);
    ClassDB::bind_method(D_METHOD("get_document", "doc_path"), &GodotFirestore::getDocument);
    ClassDB::bind_method(D_METHOD("document_exists", "doc_path"), &GodotFirestore::documentExists);
    ClassDB::bind_method(D_METHOD("document_parent", "doc_path"), &GodotFirestore::documentParent);
    ClassDB::bind_method(D_METHOD("set_document", "doc_path", "fields", "set_options", "mergefields"), &GodotFirestore::setDocument);
    ClassDB::bind_method(D_METHOD("get_document_string", "doc_path"), &GodotFirestore::getDocumentString);
    ClassDB::bind_method(D_METHOD("update_document", "doc_path", "changes"), &GodotFirestore::updateDocument);
}
