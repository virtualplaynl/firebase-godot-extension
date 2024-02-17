#pragma once

#include <godot_cpp/variant/variant.hpp>
#include <firebase/auth.h>
#include <firebase/firestore.h>
#include <firebase/storage.h>
#include <firebase/variant.h>

using namespace godot;
using namespace firebase::firestore;
using namespace firebase::storage;
using namespace firebase::auth;

godot::Variant fieldToVariant(FieldValue value);
Dictionary mapToDict(std::string id, MapFieldValue map);
Dictionary mapToDict(MapFieldValue map);

FieldValue variantToField(godot::Variant variant);
MapFieldValue dictToMap(Dictionary dict);

firebase::Variant variantToFBVariant(godot::Variant variant);
std::map<std::string, firebase::Variant> dictToVariantMap(Dictionary dict);


std::string variantToString(godot::Variant variant);
std::map<std::string, std::string> dictToStringMap(Dictionary dict);
std::vector<FieldValue> arrayToFieldVector(Array array);
std::vector<std::string> arrayToStringVector(Array array);

godot::Variant fbVariantToVariant(firebase::Variant variant);
Dictionary variantMaptoDict(std::map<firebase::Variant, firebase::Variant> map);

godot::Variant stringToVariant(std::string value);
Dictionary stringMapToDict(std::map<std::string, std::string> map);
Array stringVectorToArray(std::vector<std::string> vector);

std::pair<Array, Array> querySnapshotToArrays(QuerySnapshot snapshot);
Dictionary credentialToDict(const Credential *credential);
Dictionary metadataToDict(const Metadata *metadata);
Metadata *dictToMetadata(Dictionary metadata);