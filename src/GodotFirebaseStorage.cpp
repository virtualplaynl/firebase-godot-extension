#include "GodotFirebaseStorage.h"

using namespace godot;
using namespace firebase;
using namespace firebase::storage;

GFB_CALLBACK0_CPP(StorageDeleteTask, storage_delete);
GFB_STORAGE_CALLBACK1_CPP(GetBytesTask, get_bytes, uint64_t, INT, bytes_read);
GFB_CALLBACK1_CPP(GetDownloadUrlTask, get_download_url, String, STRING, url);
GFB_STORAGE_CALLBACK1_CPP(GetFileTask, get_file, uint64_t, INT, bytes_read);
GFB_STORAGE_CALLBACK1_CPP(GetMetadataTask, get_metadata, Dictionary, DICTIONARY, metadata);
GFB_STORAGE_CALLBACK1_CPP(PutBytesTask, put_bytes, Dictionary, DICTIONARY, metadata);
GFB_STORAGE_CALLBACK1_CPP(PutFileTask, put_file, Dictionary, DICTIONARY, metadata);
GFB_STORAGE_CALLBACK1_CPP(UpdateMetadataTask, update_metadata, Dictionary, DICTIONARY, metadata);

GodotFirebaseStorage::GodotFirebaseStorage()
{
}

GodotFirebaseStorage::~GodotFirebaseStorage()
{
}

void GodotFirebaseStorage::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Cloud Storage...");
    storage = Storage::GetInstance(app);
}

StorageReference GodotFirebaseStorage::getReference(String pathOrUrl)
{
    if (pathOrUrl.begins_with("http"))
        return storage->GetReferenceFromUrl(pathOrUrl.utf8().get_data());
    else
        return storage->GetReference(pathOrUrl.utf8().get_data());
}

String GodotFirebaseStorage::bucket(String pathOrUrl)
{
    return getReference(pathOrUrl).bucket().c_str();
}
String GodotFirebaseStorage::child(String pathOrUrl, String child)
{
    return getReference(pathOrUrl).Child(child.utf8().get_data()).full_path().c_str();
}
StorageDeleteTask *GodotFirebaseStorage::deleteObject(String pathOrUrl)
{
    StorageDeleteTask *task = memnew(StorageDeleteTask);

    getReference(pathOrUrl).Delete()
        .OnCompletion([task](Future<void> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
GetBytesTask *GodotFirebaseStorage::getBytes(String pathOrUrl, PackedByteArray buffer)
{
    GetBytesTask *task = memnew(GetBytesTask);

    getReference(pathOrUrl).GetBytes(buffer.ptrw(), buffer.size(), (Listener *)task, task->getController())
        .OnCompletion([task](Future<size_t> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed((long)completedTask.result());
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
GetDownloadUrlTask *GodotFirebaseStorage::getDownloadUrl(String pathOrUrl)
{
    GetDownloadUrlTask *task = memnew(GetDownloadUrlTask);

    getReference(pathOrUrl).GetDownloadUrl()
        .OnCompletion([task](Future<std::string> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed(completedTask.result()->c_str());
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
GetFileTask *GodotFirebaseStorage::getFile(String pathOrUrl, String storePath)
{
    GetFileTask *task = memnew(GetFileTask);

    getReference(pathOrUrl).GetFile(storePath.utf8().get_data(), (Listener *)task, task->getController())
        .OnCompletion([task](Future<size_t> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed((long)completedTask.result());
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
GetMetadataTask *GodotFirebaseStorage::getMetadata(String pathOrUrl)
{
    GetMetadataTask *task = memnew(GetMetadataTask);

    getReference(pathOrUrl).GetMetadata()
        .OnCompletion([task](Future<Metadata> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed(metadataToDict(completedTask.result()));
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
String GodotFirebaseStorage::parent(String pathOrUrl)
{
    return getReference(pathOrUrl).GetParent().full_path().c_str();
}
PutBytesTask *GodotFirebaseStorage::putBytes(String pathOrUrl, PackedByteArray buffer)
{
    PutBytesTask *task = memnew(PutBytesTask);

    getReference(pathOrUrl).PutBytes(buffer.ptrw(), buffer.size(), (Listener *)task, task->getController()).OnCompletion([task](Future<Metadata> completedTask)
                                                                                                                         {
            if (completedTask.error() == 0)
                task->completed(metadataToDict(completedTask.result()));
            else
                task->error(completedTask.error(), completedTask.error_message()); });

    return task;
}
PutFileTask *GodotFirebaseStorage::putFile(String pathOrUrl, String storePath)
{
    PutFileTask *task = memnew(PutFileTask);

    getReference(pathOrUrl).PutFile(storePath.utf8().get_data(), (Listener *)task, task->getController())
        .OnCompletion([task](Future<Metadata> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed(metadataToDict(completedTask.result()));
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
PutFileTask *GodotFirebaseStorage::putFileWithMetadata(String pathOrUrl, String storePath, Dictionary metadata)
{
    PutFileTask *task = memnew(PutFileTask);

    getReference(pathOrUrl).PutFile(storePath.utf8().get_data(), *dictToMetadata(metadata), (Listener *)task, task->getController())
        .OnCompletion([task](Future<Metadata> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed(metadataToDict(completedTask.result()));
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
UpdateMetadataTask *GodotFirebaseStorage::updateMetadata(String pathOrUrl, Dictionary metadata)
{
    UpdateMetadataTask *task = memnew(UpdateMetadataTask);

    getReference(pathOrUrl).UpdateMetadata(*dictToMetadata(metadata))
        .OnCompletion([task](Future<Metadata> completedTask)
        {
            if (completedTask.error() == 0)
                task->completed(metadataToDict(completedTask.result()));
            else
                task->error(completedTask.error(), completedTask.error_message());
        });

    return task;
}
void GodotFirebaseStorage::setMaxDownloadRetryTime(double maxTransferRetrySeconds)
{
    storage->set_max_download_retry_time(maxTransferRetrySeconds);
}
void GodotFirebaseStorage::setMaxOperationRetryTime(double maxTransferRetrySeconds)
{
    storage->set_max_operation_retry_time(maxTransferRetrySeconds);
}
void GodotFirebaseStorage::setMaxUploadRetryTime(double maxTransferRetrySeconds)
{
    storage->set_max_upload_retry_time(maxTransferRetrySeconds);
}

void GodotFirebaseStorage::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("bucket", "path"), &GodotFirebaseStorage::bucket);
    ClassDB::bind_method(D_METHOD("child", "path", "child"), &GodotFirebaseStorage::child);
    ClassDB::bind_method(D_METHOD("delete_object", "path"), &GodotFirebaseStorage::deleteObject);
    ClassDB::bind_method(D_METHOD("get_bytes", "path", "buffer"), &GodotFirebaseStorage::getBytes);
    ClassDB::bind_method(D_METHOD("get_download_url", "path"), &GodotFirebaseStorage::getDownloadUrl);
    ClassDB::bind_method(D_METHOD("get_file", "path", "local_path"), &GodotFirebaseStorage::getFile);
    ClassDB::bind_method(D_METHOD("get_metadata", "path"), &GodotFirebaseStorage::getMetadata);
    ClassDB::bind_method(D_METHOD("parent", "path"), &GodotFirebaseStorage::parent);
    ClassDB::bind_method(D_METHOD("put_bytes", "path", "buffer"), &GodotFirebaseStorage::putBytes);
    ClassDB::bind_method(D_METHOD("put_file", "path", "local_path"), &GodotFirebaseStorage::putFile);
    ClassDB::bind_method(D_METHOD("put_file_with_metadata", "path", "local_path", "metadata"), &GodotFirebaseStorage::putFileWithMetadata);
    ClassDB::bind_method(D_METHOD("update_metadata", "path", "metadata"), &GodotFirebaseStorage::updateMetadata);
    ClassDB::bind_method(D_METHOD("set_max_download_retry_time", "max_transfer_retry_seconds"), &GodotFirebaseStorage::setMaxDownloadRetryTime);
    ClassDB::bind_method(D_METHOD("set_max_operation_retry_time", "max_transfer_retry_seconds"), &GodotFirebaseStorage::setMaxOperationRetryTime);
    ClassDB::bind_method(D_METHOD("set_max_upload_retry_time", "max_transfer_retry_seconds"), &GodotFirebaseStorage::setMaxUploadRetryTime);
}
