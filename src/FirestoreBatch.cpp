#include "FirestoreBatch.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

FirestoreBatch::FirestoreBatch()
{
    firestore = nullptr;
}

FirestoreBatch::~FirestoreBatch()
{
}

void FirestoreBatch::init(Firestore *firestore)
{
    firestore = firestore;
    batch = firestore->batch();
}

void FirestoreBatch::addDelete(String docPath)
{
    if(firestore == nullptr)
    {
        UtilityFunctions::push_error("Error: FirestoreBatch not initialized! Create FirestoreBatch using Firebase.Firestore.batch() instead of making a new FirestoreBatch object.");
        return;
    }

    batch.Delete(firestore->Document(docPath.utf8().get_data()));
}

void FirestoreBatch::addSet(String docPath, Dictionary fields, godot::SetOptions options, Array mergefields)
{
    if (firestore == nullptr)
    {
        UtilityFunctions::push_error("Error: FirestoreBatch not initialized! Create FirestoreBatch using Firebase.Firestore.batch() instead of making a new FirestoreBatch object.");
        return;
    }

    firestore::SetOptions setOptions;
    if (options == godot::SetOptions::kOVERWRITE)
        setOptions = firestore::SetOptions();
    else if (options == godot::SetOptions::kMERGE_SPECIFIC)
        setOptions = firestore::SetOptions::MergeFields(arrayToStringVector(mergefields));
    else
        setOptions = firestore::SetOptions::Merge();

    batch.Set(firestore->Document(docPath.utf8().get_data()), dictToMap(fields), setOptions);
}

void FirestoreBatch::addUpdate(String docPath, Dictionary fields)
{
    if (firestore == nullptr)
    {
        UtilityFunctions::push_error("Error: FirestoreBatch not initialized! Create FirestoreBatch using Firebase.Firestore.batch() instead of making a new FirestoreBatch object.");
        return;
    }

    batch.Update(firestore->Document(docPath.utf8().get_data()), dictToMap(fields));
}

bool FirestoreBatch::isValid()
{
    if (firestore == nullptr)
    {
        UtilityFunctions::push_error("Error: FirestoreBatch not initialized! Create FirestoreBatch using Firebase.Firestore.batch() instead of making a new FirestoreBatch object.");
        return false;
    }

    return batch.is_valid();
}

int FirestoreBatch::commit()
{
    if (firestore == nullptr)
    {
        UtilityFunctions::push_error("Error: FirestoreBatch not initialized! Create FirestoreBatch using Firebase.Firestore.batch() instead of making a new FirestoreBatch object.");
        return -1;
    }
    
    batch.Commit().OnCompletion([this](Future<void> completedTask)
                                {
        if (completedTask.error() == 0)
        {
            emit_signal("commit_completed");
        }
        else
        {
            emit_signal("commit_error",
                        completedTask.error(),
                        completedTask.error_message());
        } });
    return -1;
}

void FirestoreBatch::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("add_delete", "doc_path"), &FirestoreBatch::addDelete);
    ClassDB::bind_method(D_METHOD("add_set", "doc_path", "fields", "set_options", "mergefields"), &FirestoreBatch::addSet);
    ClassDB::bind_method(D_METHOD("add_update", "doc_path", "fields"), &FirestoreBatch::addUpdate);
    ClassDB::bind_method(D_METHOD("is_valid"), &FirestoreBatch::isValid);
    ClassDB::bind_method(D_METHOD("commit"), &FirestoreBatch::commit);

    ADD_SIGNAL(MethodInfo("commit_completed"));
    ADD_SIGNAL(MethodInfo("commit_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));
}
