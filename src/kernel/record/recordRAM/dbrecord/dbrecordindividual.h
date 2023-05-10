/*!
 *  \file   dbrecordindividual.h
 *  \author Leopold Nemcek
 */
#ifndef DBRECORDINDIVIDUAL_H
#define DBRECORDINDIVIDUAL_H

#include "dbrecordrole.h"
#include "../identifier.h"

/**
 * @brief The DbRecordIndividual class Structure representing person in dbRecord. Structure is saving only information from dbRecord - primary key, role and imported flag
 */
class DbRecordIndividual
{
    friend class DbRecord;
private:
    bool _imported = false;
    DbRecordRole _role;
    Identifier _identifier; /**< Identifier of person structure from temporary memory of RecordRAM. This person structure stores all other information about person */
    const unsigned int _primaryKey;
protected:
    DbRecordIndividual(DbRecordRole role, Identifier identifier, unsigned int _primaryKey);
public:


    void setRole(DbRecordRole role);
    /**
     * @brief setImported Mark indiviudal as imported or not imported
     * @param imported True = imported, False = not imported
     */
    void setImported(bool imported);

    DbRecordRole getRole() const;
    bool getImported() const;
    Identifier getIdentifier() const;
    unsigned int getPrimaryKey() const;
};

#endif // DBRECORDINDIVIDUAL_H
