#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>
#include "GodotFirebaseObjects.h"
#include "FirestoreQuery.h"
#include "FirestoreBatch.h"
#include "FirestoreListener.h"
#include "FirestoreTransaction.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    GFB_CALLBACK4_H(LoadBundleTask, int, bytes_loaded, int, documents_loaded, int, total_bytes, int, total_documents);
    GFB_CALLBACK1_H(GetNamedQueryTask, FirestoreQuery *, query);
    GFB_CALLBACK0_H(WaitForPendingWritesTask);
    GFB_CALLBACK0_H(AddDocumentTask);
    GFB_CALLBACK1_H(CollectionExistsTask, int, document_count);
    GFB_CALLBACK0_H(DeleteDocumentTask);
    GFB_CALLBACK1_H(GetDocumentTask, Dictionary, document);
    GFB_CALLBACK1_H(DocumentExistsTask, bool, exists);
    GFB_CALLBACK0_H(SetDocumentTask);
    GFB_CALLBACK0_H(UpdateDocumentTask);

    class GodotFirestore : public Object
    {
        GDCLASS(GodotFirestore, Object);

    protected:
        static void _bind_methods();
        Firestore *firestore;

    public:
        GodotFirestore();
        ~GodotFirestore();

        void init(App *app);

        Firestore *getFirestore();

        FirestoreListener *listenToSnapshotsInSync();
        FirestoreBatch *batch();
        void clearPersistence();
        void disableNetwork();
        void enableNetwork();
        LoadBundleTask *loadBundle(String bundle);
        GetNamedQueryTask *namedQuery(String queryName);
        void runTransaction(FirestoreTransaction *transactionRunner, int maxAttempts = 5);
        void setLogLevel(int level);
        void setSettings(Dictionary settings);
        Dictionary getSettings();
        void terminate();
        WaitForPendingWritesTask *waitForPendingWrites();

        FirestoreListener *listenToCollection(String collectionPath);
        AddDocumentTask *addDocument(String docPath, Dictionary fields);
        String newDocumentIn(String collectionPath); 
        FirestoreQuery *collectionQuery(String collectionPath);
        CollectionExistsTask *collectionExists(String collectionPath);
        String collectionParent(String collectionPath);

        FirestoreListener *listenToCollectionGroup(String collectionId);
        FirestoreQuery *collectionGroupQuery(String collectionId);

        FirestoreListener *listenToDocument(String docPath);
        DeleteDocumentTask *deleteDocument(String docPath);
        GetDocumentTask *getDocument(String docPath);
        DocumentExistsTask *documentExists(String docPath);
        String documentParent(String docPath);
        SetDocumentTask *setDocument(String docPath, Dictionary fields, SetOptions setOptions, Array mergefields);
        String getDocumentString(String docPath);
        UpdateDocumentTask *updateDocument(String docPath, Dictionary changes);
    };
}
