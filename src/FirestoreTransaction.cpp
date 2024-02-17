#include "FirestoreTransaction.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

FirestoreTransaction::FirestoreTransaction()
{
    operations = Array();
}

FirestoreTransaction::~FirestoreTransaction()
{
}

firestore::Error FirestoreTransaction::run(Firestore *firestore, Transaction &transaction, std::string &errorMessage)
{
    firestore::Error error;
    // TODO: add snapshot referencing
    Dictionary snapshots = Dictionary();
    Array op;
    for (size_t i = 0; i < operations.size(); i++)
    {
        op = (Array)operations[i];
        firestore::SetOptions setOptions;

        Operation operation = (Operation)(int)op[0];
        if(operation == Operation::kDELETE)
        {
            transaction.Delete(firestore->Document(((String)op[1]).utf8().get_data()));
        }
        else if(operation ==  Operation::kGET)
        {
            std::string *error_message;
            snapshots[(String)op[1]] = mapToDict(transaction.Get(firestore->Document(((String)op[1]).utf8().get_data()), &error, error_message).GetData());
        }
        else if(operation ==  Operation::kSET)
        {
            godot::SetOptions options = (godot::SetOptions)(int)op[2];
            if (options == godot::SetOptions::kOVERWRITE)
                setOptions = firestore::SetOptions();
            else if (options == godot::SetOptions::kMERGE_SPECIFIC)
                setOptions = firestore::SetOptions::MergeFields(arrayToStringVector((Array)op[3]));
            else
                setOptions = firestore::SetOptions::Merge();

            transaction.Set(firestore->Document(((String)op[1]).utf8().get_data()), dictToMap((Dictionary)op[1]), setOptions);
        }
        else if(operation ==  Operation::kUPDATE)
        {
            transaction.Update(firestore->Document(((String)op[1]).utf8().get_data()), dictToMap(op[2]));
        }
    }

    return error;
}

void FirestoreTransaction::addDelete(String docPath)
{
    Array op = Array();
    op.append(Operation::kDELETE);
    op.append(docPath);
    operations.append(op);
}

void FirestoreTransaction::addGet(String docPath)
{
    Array op = Array();
    op.append(Operation::kGET);
    op.append(docPath);
    operations.append(op);
}

void FirestoreTransaction::addSet(String docPath, Dictionary fields, godot::SetOptions setOptions, Array mergefields)
{
    Array op = Array();
    op.append(Operation::kSET);
    op.append(docPath);
    op.append(fields);
    op.append(setOptions);
    op.append(mergefields);
    operations.append(op);
}

void FirestoreTransaction::addUpdate(String docPath, Dictionary fields)
{
    Array op = Array();
    op.append(Operation::kUPDATE);
    op.append(docPath);
    op.append(fields);
    operations.append(op);
}

void FirestoreTransaction::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("add_delete", "doc_path"), &FirestoreTransaction::addDelete);
    ClassDB::bind_method(D_METHOD("add_get", "doc_path"), &FirestoreTransaction::addGet);
    ClassDB::bind_method(D_METHOD("add_set", "doc_path", "fields", "options", "mergefields"), &FirestoreTransaction::addSet);
    ClassDB::bind_method(D_METHOD("add_update", "doc_path", "fields"), &FirestoreTransaction::addUpdate);
}
