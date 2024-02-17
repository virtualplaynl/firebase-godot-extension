#include "conversion.h"

godot::Variant fieldToVariant(FieldValue value)
{
    if (!value.is_valid() || value.is_null())
    {
        return godot::Variant(nullptr);
    }
    else 
    switch (value.type())
    {
        case FieldValue::Type::kArray:
        {
            Array result;
            for (auto &element : value.array_value())
            {
                result.append(fieldToVariant(element));
            }
            return result;
        }
        case FieldValue::Type::kBlob:
            return godot::Variant(value.blob_value());
        case FieldValue::Type::kBoolean:
            return godot::Variant(value.boolean_value());
        case FieldValue::Type::kDouble:
            return godot::Variant(value.double_value());
        case FieldValue::Type::kGeoPoint:
            return godot::Variant(value.geo_point_value().ToString().c_str());
        case FieldValue::Type::kInteger:
            return godot::Variant(value.integer_value());
        case FieldValue::Type::kMap:
            return mapToDict(value.map_value());
        case FieldValue::Type::kReference:
            return godot::Variant(value.reference_value().path().c_str());
        case FieldValue::Type::kString:
            return godot::Variant(value.string_value().c_str());
        case FieldValue::Type::kTimestamp:
        case FieldValue::Type::kServerTimestamp:
            return godot::Variant(value.timestamp_value().seconds());
        default:
            return godot::Variant(nullptr);
    }
}

Dictionary mapToDict(std::string id, MapFieldValue map)
{
    Dictionary result = mapToDict(map);
    result["id"] = String(id.c_str());
    
    return result;
}

Dictionary mapToDict(MapFieldValue map)
{
    Dictionary result = Dictionary();

    for (auto &element : map)
    {
        result[element.first.c_str()] = fieldToVariant(element.second);
    }
    return result;
}

FieldValue variantToField(godot::Variant variant)
{
    switch (variant.get_type())
    {
    case godot::Variant::Type::ARRAY:
    {
        Array array = (Array)variant;
        std::vector<FieldValue> values = std::vector<FieldValue>();
        for (size_t i = 0; i < array.size(); i++)
        {
            values.push_back(variantToField(array[i]));
        }
        return FieldValue::Array(values);
    }
    case godot::Variant::Type::BOOL:
        return FieldValue::Boolean((bool)variant);
    case godot::Variant::Type::DICTIONARY:
        if ((String)((Dictionary)variant).keys()[0] == "__INCREMENT__")
        {
            if (((Dictionary)variant)["__INCREMENT__"].get_type() == godot::Variant::Type::INT)
                return FieldValue::Increment((int64_t)((Dictionary)variant)["__INCREMENT__"]);
            else
                return FieldValue::Increment((double)((Dictionary)variant)["__INCREMENT__"]);
        }

        if ((String)((Dictionary)variant).keys()[0] == "__ARRAY_REMOVE__")
            return FieldValue::ArrayRemove(variantToField((Array)((Dictionary)variant)["__ARRAY_REMOVE__"]).array_value());
        if ((String)((Dictionary)variant).keys()[0] == "__ARRAY_UNION__")
            return FieldValue::ArrayRemove(variantToField((Array)((Dictionary)variant)["__ARRAY_UNION__"]).array_value());

        return FieldValue::Map(dictToMap((Dictionary)variant));
    case godot::Variant::Type::FLOAT:
        return FieldValue::Double((double)variant);
    case godot::Variant::Type::INT:
        return FieldValue::Integer((int64_t)variant);
    case godot::Variant::Type::NODE_PATH:
        return FieldValue::String(((String)((NodePath)variant)).utf8().get_data());
    case godot::Variant::Type::PACKED_BYTE_ARRAY:
        return FieldValue::Blob(((PackedByteArray)variant).ptr(), ((PackedByteArray)variant).size());
    case godot::Variant::Type::STRING:
        if ((String)variant == "__DELETE_FIELD__")
            return FieldValue::Delete();
        if ((String)variant == "__SERVER_TIMESTAMP__")
            return FieldValue::ServerTimestamp();
            
        return FieldValue::String(((String)variant).utf8().get_data());
    default:
        return FieldValue::Null();
    }
}

std::string variantToString(godot::Variant variant)
{
    switch (variant.get_type())
    {
    case godot::Variant::Type::NODE_PATH:
        return std::string(((String)((NodePath)variant)).utf8().get_data());
    case godot::Variant::Type::STRING:
        return std::string(((String)variant).utf8().get_data());
    default:
        return nullptr;
    }
}

MapFieldValue dictToMap(Dictionary dict)
{
    MapFieldValue map = MapFieldValue();

    for (size_t i = 0; i < dict.keys().size(); i++)
    {
        map.insert_or_assign(((String)dict.keys()[i]).utf8().get_data(), variantToField(dict[dict.keys()[i]]));
    }

    return map;
}

firebase::Variant variantToFBVariant(godot::Variant variant)
{
    switch (variant.get_type())
    {
    case godot::Variant::Type::ARRAY:
    {
        Array array = (Array)variant;
        std::vector<firebase::Variant> values = std::vector<firebase::Variant>();
        for (size_t i = 0; i < array.size(); i++)
        {
            values.push_back(variantToFBVariant(array[i]));
        }
        return firebase::Variant(values);
    }
    case godot::Variant::Type::BOOL:
        return firebase::Variant((bool)variant);
    case godot::Variant::Type::DICTIONARY:
        return firebase::Variant(dictToVariantMap((Dictionary)variant));
    case godot::Variant::Type::FLOAT:
        return firebase::Variant((double)variant);
    case godot::Variant::Type::INT:
        return firebase::Variant((int64_t)variant);
    case godot::Variant::Type::NODE_PATH:
        return firebase::Variant(std::string(((String)((NodePath)variant)).utf8().get_data()));
    case godot::Variant::Type::PACKED_BYTE_ARRAY:
        return firebase::Variant::FromMutableBlob(((PackedByteArray)variant).ptr(), ((PackedByteArray)variant).size());
    case godot::Variant::Type::STRING:
        return firebase::Variant(std::string(((String)variant).utf8().get_data()));
    default:
        return 0;
    }
}

std::map<std::string, firebase::Variant> dictToVariantMap(Dictionary dict)
{
    std::map<std::string, firebase::Variant> map = std::map<std::string, firebase::Variant>();

    for (size_t i = 0; i < dict.keys().size(); i++)
    {
        map.insert_or_assign(((String)dict.keys()[i]).utf8().get_data(), variantToFBVariant(dict[dict.keys()[i]]));
    }

    return map;
}

std::vector<FieldValue> arrayToFieldVector(Array array)
{
    std::vector<FieldValue> vector = std::vector<FieldValue>();

    for (size_t i = 0; i < array.size(); i++)
    {
        vector.push_back(variantToField(array[i]));
    }

    return vector;
}

std::vector<std::string> arrayToStringVector(Array array)
{
    std::vector<std::string> vector = std::vector<std::string>();

    for (size_t i = 0; i < array.size(); i++)
    {
        vector.push_back(variantToString(array[i]));
    }

    return vector;
}

godot::Variant fbVariantToVariant(firebase::Variant variant)
{
    if (variant.is_null())
    {
        return godot::Variant(nullptr);
    }
    else
        switch (variant.type())
        {
        case firebase::Variant::Type::kTypeVector:
        {
            Array result;
            for (auto &element : variant.vector())
            {
                result.append(fbVariantToVariant(element));
            }
            return result;
        }
        // case firebase::Variant::Type::kTypeStaticBlob:
        // case firebase::Variant::Type::kTypeMutableBlob:
        case firebase::Variant::Type::kTypeBool:
            return godot::Variant(variant.bool_value());
        case firebase::Variant::Type::kTypeDouble:
            return godot::Variant(variant.double_value());
        case firebase::Variant::Type::kTypeInt64:
            return godot::Variant(variant.int64_value());
        case firebase::Variant::Type::kTypeMap:
            return variantMaptoDict(variant.map());
        case firebase::Variant::Type::kTypeStaticString:
        case firebase::Variant::Type::kTypeMutableString:
            return godot::Variant(variant.string_value());
        default:
            return godot::Variant(nullptr);
        }
}

godot::Variant stringToVariant(std::string value)
{
    return godot::Variant(value.c_str());
}

Dictionary variantMaptoDict(std::map<firebase::Variant, firebase::Variant> map)
{
    Dictionary result = Dictionary();

    for (auto &element : map)
    {
        result[fbVariantToVariant(element.first)] = fbVariantToVariant(element.second);
    }
    return result;
}

Dictionary stringMapToDict(std::map<std::string, std::string> map)
{
    Dictionary result = Dictionary();

    for (auto &element : map)
    {
        result[element.first.c_str()] = element.second.c_str();
    }
    return result;
}

Array vectorToStringArray(std::vector<std::string> vector)
{
    Array result = Array();

    for (auto &element : vector)
    {
        result.append(stringToVariant(element));
    }
    return result;
}

std::pair<Array, Array> querySnapshotToArrays(QuerySnapshot snapshot)
{
    std::vector<DocumentChange> changes_vec = snapshot.DocumentChanges();
    std::vector<DocumentSnapshot> documents_vec = snapshot.documents();
    Array changes = Array();
    for (auto &change : changes_vec)
    {
        Dictionary changeDict = Dictionary();
        changeDict["document"] = mapToDict(change.document().GetData());
        changeDict["type"] = change.type() == DocumentChange::Type::kAdded     ? "added"
                             : change.type() == DocumentChange::Type::kRemoved ? "removed"
                                                                               : "modified";
        changeDict["old_index"] = (int)change.old_index();
        changeDict["new_index"] = (int)change.new_index();
        changes.append(changeDict);
    }
    Array documents = Array();
    for (auto &document : documents_vec)
    {
        documents.append(mapToDict(document.GetData()));
    }

    return std::pair<Array, Array>(changes, documents);
}

Dictionary metadataToDict(const Metadata *metadata)
{
    Dictionary result = Dictionary();

    result["bucket"] = metadata->bucket();
    result["cache_control"] = metadata->cache_control();
    result["content_disposition"] = metadata->content_disposition();
    result["content_encoding"] = metadata->content_encoding();
    result["content_language"] = metadata->content_language();
    result["content_type"] = metadata->content_type();
    result["creation_time"] = metadata->creation_time();
    result["custom_metadata"] = stringMapToDict(*metadata->custom_metadata());
    result["generation"] = metadata->generation();
    result["metadata_generation"] = metadata->metadata_generation();
    result["name"] = metadata->name();
    result["path"] = metadata->path();
    result["size_bytes"] = metadata->size_bytes();
    result["updated_time"] = metadata->updated_time();

    return result;
}
Metadata *dictToMetadata(Dictionary metadata)
{
    Metadata *result = new Metadata();
    result->set_cache_control(((String)metadata["cache_control"]).utf8().get_data());
    result->set_content_disposition(((String)metadata["content_disposition"]).utf8().get_data());
    result->set_content_encoding(((String)metadata["content_encoding"]).utf8().get_data());
    result->set_content_language(((String)metadata["content_language"]).utf8().get_data());
    result->set_content_type(((String)metadata["content_type"]).utf8().get_data());

    return result;
}
