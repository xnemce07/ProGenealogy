/*!
 *  \file   databaseneo4j.cpp
 *  \author Leopold Nemcek
 */

#include "databaseneo4j.h"

DatabaseNeo4j::DatabaseNeo4j()
{
    _httpWorker = new Neo4jHttpWorker();
}


int DatabaseNeo4j::getPort() const
{
    return _httpWorker->getPort();
}

QString DatabaseNeo4j::getUsername() const
{
    return _httpWorker->getUsername();
}

QString DatabaseNeo4j::getPassword() const
{
    return _httpWorker->getPassword();
}

QString DatabaseNeo4j::getHostname() const
{
    return _httpWorker->getHostname();
}

QString DatabaseNeo4j::getDbName() const
{
    return _httpWorker->getDbName();
}

void DatabaseNeo4j::setUsername(QString username)
{
    _httpWorker->setUsername(username);
}

void DatabaseNeo4j::setPassword(QString password)
{
    _httpWorker->setPassword(password);
}

void DatabaseNeo4j::setHostname(QString hostname)
{
    _httpWorker->setHostname(hostname);
}

void DatabaseNeo4j::setPort(int port)
{
    _httpWorker->setPort(port);
}

void DatabaseNeo4j::setDbName(QString name)
{
    _httpWorker->setDbName(name);
}


bool DatabaseNeo4j::dbExport(Record &record)
{
    Q_UNUSED(record)

    for(Identifier i = record.getIdentifierIndividualFirst(); i != record.getIdentifierIndividualLast(); record.getIdentifierIndividualNext(i))
    {
        exportPerson(record, i);
    }

    for(Identifier i = record.getIdentifierFamilyFirst(); i != record.getIdentifierFamilyLast(); record.getIdentifierFamilyNext(i))
    {
        exportFamily(record, i);
    }

    for(Identifier i = record.getIdentifierIndividualFirst(); i != record.getIdentifierIndividualLast(); record.getIdentifierIndividualNext(i))
    {
        exportPersonRelatives(record, i);
    }


    if(_httpWorker->send()){
        QByteArray data = _httpWorker->getData();
        return true;
    }else{
        QString err = _httpWorker->getLastError();
        return false;
    }
}

QString DatabaseNeo4j::lastError()
{
    return _httpWorker->getLastError();
}


void DatabaseNeo4j::exportPerson(Record &record, Identifier ref)
{
    QJsonObject props;
    props.insert("id", QString::number(ref.getNumber()));
    {
        QString name = record.getIndividualNameGiven(ref);
        QJsonArray arr;
        arr.push_back(name);
        props.insert("jméno", arr);
    }

    {
        QString surname = record.getIndividualNameSurnameMarriedBirth(ref);
        QJsonArray arr;
        arr.push_back(surname);
        props.insert("příjmení", arr);
    }

    {
        QString title = record.getIndividualNamePrefix(ref) + " " + record.getIndividualNameSuffix(ref);
        QJsonArray arr;
        arr.push_back(title);
        props.insert("titul", arr);
    }

    QChar gender = gender2char(record.getIndividualGender(ref));
    props.insert("pohlaví", QString(gender));

    {
        Date birthDate = record.getIndividualBirthDate1(ref);
        QJsonArray arr;
        if(birthDate.isValid()){
            arr.push_back(birthDate.toString("yyyy-MM-dd"));
        }
        props.insert("datum_narození", arr);
    }

    {
        Date deathDate = record.getIndividualDeathDate1(ref);
        QJsonArray arr;
        if(deathDate.isValid()){
            arr.push_back(deathDate.toString("yyyy-MM-dd"));
        }
        props.insert("datum_úmrtí", arr);
    }

    {
        Date christeningDate = record.getIndividualChristeningDate1(ref);
        QJsonArray arr;
        if(christeningDate.isValid()){
            arr.push_back(christeningDate.toString("yyyy-MM-dd"));
        }
        props.insert("datum_křestu", arr);
    }

    {
        QJsonArray arr;
        unsigned int count = record.getIndividualOccuCount(ref);
        for(unsigned int i = 0; i < count; i++){
            arr.push_back(record.getIndividualOccuDescription(ref,i));
        }
        props.insert("povolání", arr);
    }

    {
        QJsonArray arr;
        unsigned int count = record.getIndividualReliCount(ref);
        for(unsigned int i = 0; i < count; i++){
            arr.push_back(record.getIndividualReliDescription(ref,i));
        }
        props.insert("vyznání", arr);
    }

    props.insert("vícerčatá","ne");
    props.insert("otec_mrtev","ne");
    props.insert("nalezenec","ne");
    props.insert("manželské","ne");

    {
        QJsonArray arr;
        props.insert("normalizované_jméno", arr);
        props.insert("normalizované_příjmení", arr);
        props.insert("normalizované_povolání", arr);
        props.insert("odhad_datumu_narození", arr);
        props.insert("odhad_datumu_úmrtí", arr);
        props.insert("datum_konfirmace", arr);
        props.insert("datum_vystoupení_z_cirkve", arr);
        props.insert("návrat_do_cirkve", arr);
    }

    _httpWorker->addStatement("CREATE (n:Osoba $props)", props);

    {
        unsigned int count = record.getIndividualResiCount(ref);
        for(unsigned int i = 0; i < count; i++){
            QString date = record.getIndividualResiDate1(ref, i).toString("yyyy-MM-dd");
            QString addr = record.getIndividualResiAddrAddr(ref, i);
            QString city = record.getIndividualResiAddrCity(ref, i);
            if(!addr.isEmpty() && city.isEmpty()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba) "
                                                 "WHERE p.id = '%1' "
                                                 "CREATE (p)-[r:BYDLÍ {datum:'%2'}]->(b:Ulice {ulice: '%3'})")
                                                     .arg(QString::number(ref.getNumber()),date, addr));
            }
            if(!addr.isEmpty() && !city.isEmpty()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba) "
                                                 "WHERE p.id = '%1' "
                                                 "CREATE (p)-[r:BYDLÍ {datum:'%2'}]->(b:Ulice {ulice: '%3'})-[s:JE_V]->(m:Město {město: '%4'})")
                                                     .arg(QString::number(ref.getNumber()),date, addr, city));
            }
            if(addr.isEmpty() && !city.isEmpty()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba) "
                                                 "WHERE p.id = '%1' "
                                                 "CREATE (p)-[r:BYDLÍ {datum:'%2'}]->(m:Město {město: '%4'})")
                                             .arg(QString::number(ref.getNumber()),date, city));
            }

        }
    }


    if(record.isIndividualBorn(ref)){
        _httpWorker->addStatement(QString("MATCH (n:Osoba) "
                                         "WHERE n.id = '%1' "
                                         "CREATE (n)-[r:JE_V]->(z:Záznam_o_křte {id:'%1'})").arg(ref.getNumber()));


        Identifier source = record.getIndividualBirthSourceRef(ref);
        if(source.isValid()){

            QJsonObject props;

            props.insert("id", QString::number(source.getNumber()));
            props.insert("signatura",record.getSourceTitle(source));
            props.insert("archiv",record.getSourceArchive(source));
            QJsonArray datesArr;
            Date from = record.getSourceCommonDateFrom(source);
            Date to = record.getSourceCommonDateTo(source);
            if(from.isValid()){
                datesArr.append(from.toString("yyyy-MM-dd"));
            }
            if(to.isValid()){
                datesArr.append(to.toString("yyyy-MM-dd"));
            }

            props.insert("časové_období", datesArr);


            _httpWorker->addStatement(QString("MATCH (z:Záznam_o_křte) "
                                              "WHERE z.id = '%1' "
                                              "CREATE (z)-[r:JE_V]->(m:Matrika_narodených $props)").arg(ref.getNumber()), props);
        }
    }

    if(record.isIndividualDeceased(ref)){
        _httpWorker->addStatement(QString("MATCH (n:Osoba) "
                                         "WHERE n.id = '%1' "
                                         "CREATE (n)-[r:JE_V]->(z:Záznam_o_úmrtí {id:'%1'})").arg(ref.getNumber()));

        Identifier source = record.getIndividualDeathSourceRef(ref);
        if(source.isValid()){

            QJsonObject props;

            props.insert("id", QString::number(source.getNumber()));
            props.insert("signatura",record.getSourceTitle(source));
            props.insert("archiv",record.getSourceArchive(source));
            QJsonArray datesArr;
            Date from = record.getSourceCommonDateFrom(source);
            Date to = record.getSourceCommonDateTo(source);
            if(from.isValid()){
                datesArr.append(from.toString("yyyy-MM-dd"));
            }
            if(to.isValid()){
                datesArr.append(to.toString("yyyy-MM-dd"));
            }

            props.insert("časové_období", datesArr);


            _httpWorker->addStatement(QString("MATCH (z:Záznam_o_úmrtí) "
                                              "WHERE z.id = '%1' "
                                              "CREATE (z)-[r:JE_V]->(m:Matrika_úmrtí $props)").arg(ref.getNumber()), props);
        }
    }




}

void DatabaseNeo4j::exportFamily(Record &record, Identifier ref)
{
    if(record.isFamilyMarried(ref)){
        QString marrDate = record.getFamilyMarriageDate1(ref).toString("yyyy-MM-dd");

        Identifier brideRef = record.getFamilyWife(ref);
        Identifier groomRef = record.getFamilyHusband(ref);
        if(brideRef.isValid() && groomRef.isValid())
        {
            _httpWorker->addStatement(QString("MATCH (w:Osoba), (h:Osoba) "
                                             "WHERE w.id = '%1' AND h.id = '%2' "
                                             "CREATE (w)-[r:JE_V]->(z:Záznam_o_oddaní {id:'%3'})<-[q:JE_V]-(h)").arg(QString::number(brideRef.getNumber()),QString::number(groomRef.getNumber()),QString::number(ref.getNumber())));

        {
            Identifier source = record.getFamilyMarriageSourceRef(ref);
            if(source.isValid()){

                QJsonObject props;

                props.insert("id", QString::number(source.getNumber()));
                props.insert("signatura",record.getSourceTitle(source));
                props.insert("archiv",record.getSourceArchive(source));
                QJsonArray datesArr;
                Date from = record.getSourceCommonDateFrom(source);
                Date to = record.getSourceCommonDateTo(source);
                if(from.isValid()){
                    datesArr.append(from.toString("yyyy-MM-dd"));
                }
                if(to.isValid()){
                    datesArr.append(to.toString("yyyy-MM-dd"));
                }

                props.insert("časové_období", datesArr);


                _httpWorker->addStatement(QString("MATCH (z:Záznam_o_oddaní) "
                                                  "WHERE z.id = '%1' "
                                                  "CREATE (z)-[r:JE_V]->(m:Matrika_oddaných $props)").arg(ref.getNumber()), props);
            }
        }


            _httpWorker->addStatement(QString("MATCH (w:Osoba), (h:Osoba) "
                                             "WHERE w.id = '%1' AND h.id = '%2' "
                                             "CREATE (w)-[r:JSOU_MANŽELÉ {datum:'%3'}]->(h)")
                                         .arg(QString::number(brideRef.getNumber()),QString::number(groomRef.getNumber()), marrDate));

            Identifier priest = record.getFamilyMarriagePriest(ref);
            if(priest.isValid()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba), (w:Osoba), (h:Osoba) "
                                                 "WHERE p.id = '%1' AND w.id = '%2' AND h.id = '%3' "
                                                 "CREATE (w)<-[s:ODDAL {datum:'%4'}]-(p)-[r:ODDAL {datum:'%4'}]->(h)")
                                             .arg(QString::number(priest.getNumber()),QString::number(brideRef.getNumber()), QString::number(groomRef.getNumber()), marrDate));

                _httpWorker->addStatement(QString("MATCH (p:Osoba),(z:Záznam_o_oddaní) "
                                                 "WHERE p.id = '%1' AND z.id = '%2' "
                                                 "CREATE (p)-[r:JE_V]->(z)")
                                             .arg(QString::number(priest.getNumber()),QString::number(ref.getNumber())));
            }

            Identifier witness1 = record.getFamilyMarriageWitness1(ref);
            if(witness1.isValid()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba), (w:Osoba), (h:Osoba) "
                                                 "WHERE p.id = '%1' AND w.id = '%2' AND h.id = '%3' "
                                                 "CREATE (w)<-[s:JE_SVĚDEK {datum:'%4'}]-(p)-[r:JE_SVĚDEK {datum:'%4'}]->(h)")
                                             .arg(QString::number(witness1.getNumber()),QString::number(brideRef.getNumber()), QString::number(groomRef.getNumber()), marrDate));

                _httpWorker->addStatement(QString("MATCH (w:Osoba),(z:Záznam_o_oddaní) "
                                                 "WHERE w.id = '%1' AND z.id = '%2' "
                                                 "CREATE (w)-[r:JE_V]->(z)").arg(QString::number(witness1.getNumber()),QString::number(ref.getNumber())));
            }

            Identifier witness2 = record.getFamilyMarriageWitness2(ref);
            if(witness2.isValid()){
                _httpWorker->addStatement(QString("MATCH (p:Osoba), (w:Osoba), (h:Osoba) "
                                                 "WHERE p.id = '%1' AND w.id = '%2' AND h.id = '%3' "
                                                 "CREATE (w)<-[s:JE_SVĚDEK {datum:'%4'}]-(p)-[r:JE_SVĚDEK {datum:'%4'}]->(h)")
                                             .arg(QString::number(witness2.getNumber()),QString::number(brideRef.getNumber()), QString::number(groomRef.getNumber()), marrDate));

                _httpWorker->addStatement(QString("MATCH (w:Osoba),(z:Záznam_o_oddaní) "
                                                 "WHERE w.id = '%1' AND z.id = '%2' "
                                                 "CREATE (w)-[r:JE_V]->(z)").arg(QString::number(witness2.getNumber()),QString::number(ref.getNumber())));
            }
        }
    }
}

void DatabaseNeo4j::exportPersonRelatives(Record &record, Identifier ref)
{
    QString birthStr = record.getIndividualBirthDate1(ref).toString("yyyy-MM-dd");
    QString burialStr = record.getIndividualBurialDate1(ref).toString("yyyy-MM-dd");

    Identifier mother = record.getIndividualMother(ref);
    if(mother.isValid()){
        _httpWorker->addStatement(QString("MATCH (p:Osoba), (m:Osoba) "
                                         "WHERE p.id = '%1' AND m.id = '%2' "
                                         "CREATE (m)-[r:JE_MATKA {datum:'%3'}]->(p)")
                                     .arg(QString::number(ref.getNumber()), QString::number(mother.getNumber()), birthStr));

        _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                         "WHERE m.id = '%1' AND z.id = '%2' "
                                         "CREATE (m)-[r:JE_V]->(z)")
                                     .arg(QString::number(mother.getNumber()), QString::number(ref.getNumber())));
    }

    Identifier father = record.getIndividualFather(ref);
    if(father.isValid()){
        _httpWorker->addStatement(QString("MATCH (p:Osoba), (f:Osoba) "
                                         "WHERE p.id = '%1' AND f.id = '%2' "
                                         "CREATE (f)-[r:JE_OTEC {datum:'%3'}]->(p)")
                                     .arg(QString::number(ref.getNumber()), QString::number(father.getNumber()), birthStr));

        _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                         "WHERE m.id = '%1' AND z.id = '%2' "
                                         "CREATE (m)-[r:JE_V]->(z)")
                                     .arg(QString::number(father.getNumber()), QString::number(ref.getNumber())));
    }

    Identifier midwife = record.getIndividualBirthObstetrix(ref);
    if(midwife.isValid()){
        _httpWorker->addStatement(QString("MATCH (p:Osoba), (m:Osoba) "
                                         "WHERE p.id = '%1' AND m.id = '%2' "
                                         "CREATE (m)-[r:ODRODILA {datum:'%3'}]->(p)")
                                     .arg(QString::number(ref.getNumber()), QString::number(midwife.getNumber()), birthStr));

        _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                         "WHERE m.id = '%1' AND z.id = '%2' "
                                         "CREATE (m)-[r:JE_V]->(z)")
                                     .arg(QString::number(midwife.getNumber()), QString::number(ref.getNumber())));
    }

    if(record.isIndividualChristened(ref)){
        Identifier priestChristening = record.getIndividualChristeningPriest(ref);
        if(priestChristening.isValid()){
            _httpWorker->addStatement(QString("MATCH (p:Osoba), (c:Osoba) "
                                             "WHERE p.id = '%1' AND c.id = '%2' "
                                             "CREATE (c)-[r:KŘTIL {datum:'%3'}]->(p)")
                                            .arg(QString::number(ref.getNumber()), QString::number(priestChristening.getNumber()), birthStr));

            _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                             "WHERE m.id = '%1' AND z.id = '%2' "
                                             "CREATE (m)-[r:JE_V]->(z)")
                                         .arg(QString::number(priestChristening.getNumber()), QString::number(ref.getNumber())));
        }

        Identifier godfather = record.getIndividualChristeningGodfather(ref);
        if(godfather.isValid()){
            _httpWorker->addStatement(QString("MATCH (p:Osoba), (c:Osoba) "
                                             "WHERE p.id = '%1' AND c.id = '%2' "
                                             "CREATE (c)-[r:JE_KMOTR {datum:'%3'}]->(p)").arg(QString::number(ref.getNumber()), QString::number(godfather.getNumber()), birthStr));

            _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                             "WHERE m.id = '%1' AND z.id = '%2' "
                                             "CREATE (m)-[r:JE_V]->(z)")
                                         .arg(QString::number(godfather.getNumber()), QString::number(ref.getNumber())));
        }

        Identifier godmother = record.getIndividualChristeningGodmother(ref);
        if(godmother.isValid()){
            _httpWorker->addStatement(QString("MATCH (p:Osoba), (c:Osoba) "
                                             "WHERE p.id = '%1' AND c.id = '%2' "
                                             "CREATE (c)-[r:JE_KMOTR {datum:'%3'}]->(p)")
                                         .arg(QString::number(ref.getNumber()), QString::number(godmother.getNumber()), birthStr));

            _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_křte) "
                                             "WHERE m.id = '%1' AND z.id = '%2' "
                                             "CREATE (m)-[r:JE_V]->(z)")
                                         .arg(QString::number(godmother.getNumber()), QString::number(ref.getNumber())));
        }

    }

    if(record.isIndividualBuried(ref))
    {
        Identifier priestBurial = record.getIndividualBurialPriest(ref);
        if(priestBurial.isValid()){
            _httpWorker->addStatement(QString("MATCH (p:Osoba), (b:Osoba) "
                                             "WHERE p.id = '%1' AND b.id = '%2' "
                                             "CREATE (b)-[r:POHŘBIL {datum:'%3'}]->(p)")
                                        .arg(QString::number(ref.getNumber()), QString::number(priestBurial.getNumber()), burialStr));

            _httpWorker->addStatement(QString("MATCH (m:Osoba), (z:Záznam_o_úmrtí) "
                                             "WHERE m.id = '%1' AND z.id = '%2' "
                                             "CREATE (m)-[r:JE_V]->(z)")
                                         .arg(QString::number(priestBurial.getNumber()), QString::number(ref.getNumber())));
        }
    }

}

QChar DatabaseNeo4j::gender2char(Gender gender)
{
    if(gender == Gender::MALE)
    {
        return 'M';
    } else if(gender == Gender::FEMALE)
    {
        return 'F';
    } else {
        return 'U';
    }
}



