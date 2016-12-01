
#include "DatabaseStructure.h"
#include "TypedQueryGenerator.h"
#include "Database.h"

int main()
{
    Database db;

    TYPED_QUERY(
        /* query name */ pcia_query,
        /* table name */ playercreateinfo_action,
        /* field list */ race, class_, action
    );

    // typed query
    if (pcia_query result = db.TQuery<pcia_query>())
    {
        do
        {
            // access result fields!
            uint32_t action = result.get_action();
            uint8_t race = result.get_race();
            uint8_t class_ = result.get_class_();
        } while (result.NextRow());
    }
}
