#include "FirestoreListener.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

FirestoreListener::FirestoreListener()
{
}

FirestoreListener::~FirestoreListener()
{
    registration.Remove();
}

void FirestoreListener::setToDocument(String _docPath, ListenerRegistration _registration)
{
    docPath = _docPath;
    registration = _registration;
}
void FirestoreListener::setToCollection(String _collectionPath, ListenerRegistration _registration)
{
    collectionPath = _collectionPath;
    registration = _registration;
}
void FirestoreListener::setToCollectionGroup(String _collectionId, ListenerRegistration _registration)
{
    collectionId = _collectionId;
    registration = _registration;
}
void FirestoreListener::setToInSync(ListenerRegistration _registration)
{
    registration = _registration;
}

void FirestoreListener::inSyncCallback()
{
    emit_signal("listeners_in_sync");
}

void FirestoreListener::docUpdated(const DocumentSnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage)
{
    if (error == 0)
    {
        if (snapshot.exists())
        {
            MapFieldValue data = snapshot.GetData();
            Dictionary dataDict = mapToDict(snapshot.id(), data);

            emit_signal("document_update", docPath, dataDict);
        }
        else
        {
            emit_signal("document_update", docPath, Dictionary());
        }
    }
    else
    {
        emit_signal("document_listen_error", docPath, error, errorMessage.c_str());
    }
}
void FirestoreListener::collectionUpdated(const QuerySnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage)
{
    if (error == 0)
    {
        if (snapshot.empty())
        {
            emit_signal("collection_update", collectionPath, Array(), Array());
        }
        else
        {
            std::pair<Array, Array> snapshotElements = querySnapshotToArrays(snapshot);
            emit_signal("collection_update", collectionPath, snapshotElements.first, snapshotElements.second);
        }
    }
    else
    {
        emit_signal("collection_listen_error", collectionPath, error, errorMessage.c_str());
    }
}
void FirestoreListener::collectionGroupUpdated(const QuerySnapshot &snapshot, firebase::firestore::Error error, const std::string errorMessage)
{
    if (error == 0)
    {
        if (snapshot.empty())
        {
            emit_signal("collection_group_update", collectionId, Array(), Array());
        }
        else
        {
            std::pair<Array, Array> snapshotElements = querySnapshotToArrays(snapshot);
            emit_signal("collection_group_update", collectionId, snapshotElements.first, snapshotElements.second);
        }
    }
    else
    {
        emit_signal("collection_group_listen_error", collectionId, error, errorMessage.c_str());
    }
}

void FirestoreListener::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("listeners_in_sync"));
    ADD_SIGNAL(MethodInfo("document_update", PropertyInfo(Variant::STRING, "doc_path"), PropertyInfo(Variant::DICTIONARY, "document")));
    ADD_SIGNAL(MethodInfo("document_listen_error", PropertyInfo(Variant::STRING, "doc_path"), PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
    ADD_SIGNAL(MethodInfo("collection_update", PropertyInfo(Variant::STRING, "collection_path"), PropertyInfo(Variant::DICTIONARY, "changes"), PropertyInfo(Variant::DICTIONARY, "documents")));
    ADD_SIGNAL(MethodInfo("collection_listen_error", PropertyInfo(Variant::STRING, "collection_path"), PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
    ADD_SIGNAL(MethodInfo("collection_group_update", PropertyInfo(Variant::STRING, "collection_id"), PropertyInfo(Variant::DICTIONARY, "changes"), PropertyInfo(Variant::DICTIONARY, "documents")));
    ADD_SIGNAL(MethodInfo("collection_group_listen_error", PropertyInfo(Variant::STRING, "collection_id"), PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
}
