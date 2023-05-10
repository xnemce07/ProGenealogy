/*!
 *  \file   databaserow.h
 *  \author Leopold Nemcek
 */
#ifndef DATABASEROW_H
#define DATABASEROW_H

#include <QMap>
#include <QVariant>

/**
 * @brief Class representing row returned from database or the DEMoS API.
 */
class DatabaseRow
{
private:
    QVariantMap _row;

public:
    DatabaseRow(QVariantMap row);

    QVariant value(QString name) const;
};

#endif // DATABASEROW_H
