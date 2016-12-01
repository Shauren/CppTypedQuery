#pragma once

#include <boost/preprocessor.hpp>
#include <tuple>

struct Field;

// specialize this for all supported types
template<typename T>
class FieldValueExtractor
{
public:
    static T GetValue(Field& fields)
    {
        return 0;
    }
};

// preprocessor sorcery, beware

// create field name token for query
#define QUERY_STRING_FIELDS(s, table, fld) BOOST_PP_STRINGIZE(fld)

// create field list string
#define QUERY_FIELD_CAT(s, state, fld) BOOST_PP_CAT(state, BOOST_PP_CAT(",", fld))

#define QUERY_STRING(FIELDS_SEQ) \
    BOOST_PP_SEQ_FOLD_LEFT(QUERY_FIELD_CAT, BOOST_PP_SEQ_HEAD(FIELDS_SEQ), BOOST_PP_SEQ_TAIL(FIELDS_SEQ))

// generate all get_field() accessors
#define QUERY_FIELD_ACCESSOR(r, table, i, fld) auto BOOST_PP_CAT(get_, fld)() { return FieldValueExtractor<decltype(table::fld)>::GetValue(_result->Fetch()[i]); }

// result structure
#define TYPED_QUERY(query_name, table, ...) struct query_name \
{ \
    /* constructor */\
    query_name(QueryResult result) : _result(result) { }\
    /* returns query string */\
    static char const* GetQuery() { return "SELECT " \
    QUERY_STRING(BOOST_PP_SEQ_TRANSFORM(QUERY_STRING_FIELDS, table, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))) \
    " FROM " #table; } \
    /* reference to result set to keep it alive at least as long as this object */\
    QueryResult _result;\
    /* generated field accessors */\
    BOOST_PP_SEQ_FOR_EACH_I(QUERY_FIELD_ACCESSOR, table, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
    /* methods delegated from QueryResult for nice loop/if syntax */\
    operator bool() { return _result.operator bool(); }\
    bool NextRow() { return _result->NextRow(); }\
}
