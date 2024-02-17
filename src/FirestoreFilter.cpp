#include "FirestoreFilter.h"

using namespace godot;
using namespace firebase;
using namespace firebase::firestore;

FirestoreFilter::FirestoreFilter()
{
}

FirestoreFilter::~FirestoreFilter()
{
}

Filter FirestoreFilter::getFilter()
{
    return *filter;
}

void FirestoreFilter::setFilter(Filter _filter, String string)
{
    filter = &_filter;
    filterString = string;
}

String FirestoreFilter::getString()
{
    return filterString;
}

FirestoreFilter *FirestoreFilter::andAll(Array _filters)
{
    std::vector<Filter> filters = std::vector<Filter>();
    String string = "and(";
    for (size_t i = 0; i < _filters.size(); i++)
    {
        filters.push_back(((FirestoreFilter *)((Object *)_filters[i]))->getFilter());
        string = string + ((FirestoreFilter *)((Object *)_filters[i]))->getString();
        if (i < _filters.size() - 1)
            string = string + ", ";
    }
    string = string + ")";

    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::And(filters), string);
    return newFilter;
}
FirestoreFilter *FirestoreFilter::orAny(Array _filters)
{
    std::vector<Filter> filters = std::vector<Filter>();
    String string = "or(";
    for (size_t i = 0; i < _filters.size(); i++)
    {
        filters.push_back(((FirestoreFilter *)((Object *)_filters[i]))->getFilter());
        string = string + ((FirestoreFilter *)((Object *)_filters[i]))->getString();
        if (i < _filters.size() - 1)
            string = string + ", ";
    }
    string = string + ")";

    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::Or(filters), string);
    return newFilter;
}
FirestoreFilter *FirestoreFilter::arrayContains(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::ArrayContains(field.utf8().get_data(), variantToField(value)),
                         "array_contains(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::arrayContainsAny(String field, Array values)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::ArrayContainsAny(field.utf8().get_data(), arrayToFieldVector(values)),
                         "array_contains_any(" + field + ", #" + values.size() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::equalTo(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::EqualTo(field.utf8().get_data(), variantToField(value)),
                         "equal_to(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::greaterThan(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::GreaterThan(field.utf8().get_data(), variantToField(value)),
                         "greater_than(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::greaterThanOrEqualTo(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::GreaterThanOrEqualTo(field.utf8().get_data(), variantToField(value)),
                         "greater_than_or_equal_to(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::in(String field, Array values)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::In(field.utf8().get_data(), arrayToFieldVector(values)),
                         "in(" + field + ", #" + values.size() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::lessThan(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::LessThan(field.utf8().get_data(), variantToField(value)),
                         "less_than(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::lessThanOrEqualTo(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::LessThanOrEqualTo(field.utf8().get_data(), variantToField(value)),
                         "less_than_or_equal_to(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::notEqualTo(String field, godot::Variant value)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::NotEqualTo(field.utf8().get_data(), variantToField(value)),
                         "not_equal_to(" + field + ", " + value.stringify() + ")");
    return newFilter;
}
FirestoreFilter *FirestoreFilter::notIn(String field, Array values)
{
    FirestoreFilter *newFilter = memnew(FirestoreFilter);
    newFilter->setFilter(Filter::NotIn(field.utf8().get_data(), arrayToFieldVector(values)),
                         "not_in(" + field + ", #" + values.size() + ")");
    return newFilter;
}

void FirestoreFilter::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_string"), &FirestoreFilter::getString);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("and", "filters"), &FirestoreFilter::andAll);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("or", "filters"), &FirestoreFilter::orAny);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("array_contains", "field", "value"), &FirestoreFilter::arrayContains);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("array_contains_any", "field", "values"), &FirestoreFilter::arrayContainsAny);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("equal_to", "field", "value"), &FirestoreFilter::equalTo);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("greater_than", "field", "value"), &FirestoreFilter::greaterThan);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("greater_than_or_equal_to", "field", "value"), &FirestoreFilter::greaterThanOrEqualTo);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("in", "field", "values"), &FirestoreFilter::in);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("less_than", "field", "value"), &FirestoreFilter::lessThan);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("less_than_or_equal_to", "field", "value"), &FirestoreFilter::lessThanOrEqualTo);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("not_equal_to", "field", "value"), &FirestoreFilter::notEqualTo);
    ClassDB::bind_static_method("FirestoreFilter", D_METHOD("not_in", "field", "values"), &FirestoreFilter::notIn);
}
