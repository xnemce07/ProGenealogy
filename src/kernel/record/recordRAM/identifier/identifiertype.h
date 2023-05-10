/*!
 *  \file   identifiertype.h
 *  \author Eva Navratilova, Leopold Nemcek
 *  \note This file was originally created by Eva Navratilova, Leopold Nemcek made only additions
 */

#ifndef IDENTIFIERTYPE_H
#define IDENTIFIERTYPE_H

/**
 * @brief Defines enumeration of identifier types
 *
 */
enum class IdentifierType
{
    ID_INDI,
    ID_FAM,
    ID_SUBM,
    ID_SOUR,
    ID_INVAL,
    ID_DBREC,
    ID_DBINDI,
    ID_DBSOURCE,
    ID_MERGEINDIVIDUAL,
    ID_MERGEFAM
};

#endif // IDENTIFIERTYPE_H
