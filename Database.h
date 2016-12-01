#pragma once

#include <memory>

//ignore
struct Field { };
struct ResultSet { Field* Fetch() { return nullptr; } bool NextRow() { return false; } };
typedef std::shared_ptr<ResultSet> QueryResult;

class Database
{
public:
    QueryResult Query(char const* query) { return{}; }

    template<typename ResultType>
    ResultType TQuery()
    {
        return ResultType(Query(ResultType::GetQuery()));
    }
};
