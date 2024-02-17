#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/storage.h>
#include "GodotFirebaseObjects.h"

#include "conversion.h"

using namespace firebase;
using namespace firebase::storage;

namespace godot
{
    GFB_CALLBACK0_H(StorageDeleteTask);
    GFB_STORAGE_CALLBACK1_H(GetBytesTask, uint64_t, bytesRead);
    GFB_CALLBACK1_H(GetDownloadUrlTask, String, url);
    GFB_STORAGE_CALLBACK1_H(GetFileTask, uint64_t, bytesRead);
    GFB_STORAGE_CALLBACK1_H(GetMetadataTask, Dictionary, metadata);
    GFB_STORAGE_CALLBACK1_H(PutBytesTask, Dictionary, metadata);
    GFB_STORAGE_CALLBACK1_H(PutFileTask, Dictionary, metadata);
    GFB_STORAGE_CALLBACK1_H(UpdateMetadataTask, Dictionary, metadata);

    class GodotFirebaseStorage : public Object
    {
        GDCLASS(GodotFirebaseStorage, Object);

    protected:
        static void _bind_methods();
        Storage *storage;
        StorageReference getReference(String pathOrUrl);

    public:
        GodotFirebaseStorage();
        ~GodotFirebaseStorage();

        void init(App *app);
        
        String bucket(String path);
        String child(String path, String child);
        StorageDeleteTask *deleteObject(String path);
        GetBytesTask *getBytes(String path, PackedByteArray buffer);
        GetDownloadUrlTask *getDownloadUrl(String path);
        GetFileTask *getFile(String path, String storePath);
        GetMetadataTask *getMetadata(String path);
        String parent(String path);
        PutBytesTask *putBytes(String path, PackedByteArray buffer);
        PutFileTask *putFile(String path, String storePath);
        PutFileTask *putFileWithMetadata(String path, String storePath, Dictionary metadata);
        UpdateMetadataTask *updateMetadata(String path, Dictionary metadata);
        void setMaxDownloadRetryTime(double maxTransferRetrySeconds);
        void setMaxOperationRetryTime(double maxTransferRetrySeconds);
        void setMaxUploadRetryTime(double maxTransferRetrySeconds);
    };
}