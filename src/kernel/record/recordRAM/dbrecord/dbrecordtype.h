/*!
 *  \file   dbrecordtype.h
 *  \author Leopold Nemcek
 */

#ifndef DBRECORDTYPE_H
#define DBRECORDTYPE_H

/**
 * @brief The DbRecordType enum Defines enumeration of database record types from the DEMoS database
 */
enum class DbRecordType
{
    RT_BIRTH,
    RT_MARRIAGE,
    RT_MARRIAGE_GROOM,
    RT_MARRIAGE_BRIDE,
    RT_BURIAL,
    RT_UNDEFINED
};

#endif // DBRECORDTYPE_H
