/*!
 *  \file   databasedemos.cpp
 *  \author Leopold Nemcek
 */
#include "databasedemos.h"

DatabaseDemos::DatabaseDemos()
{
    initDbRecordRoleResolveTable();
    _worker.setAddress(_address);
}

QString DatabaseDemos::getUsername() const
{
    return this->_worker.getUsername();
}

QString DatabaseDemos::getPassword() const
{
    return this->_worker.getPassword();
}

void DatabaseDemos::clearPassword()
{
    this->_worker.setPassword("");
}

bool DatabaseDemos::isReady()
{
    if(!_worker.isOpen()){
        return setError(_worker.lastError());
    }
    return true;
}

QString DatabaseDemos::lastError()
{
    return _error;
}

void DatabaseDemos::setUsername(QString username)
{
    _worker.setUsername(username);
}

void DatabaseDemos::setPassword(QString password)
{
    _worker.setPassword(password);
}




/// IMPORTING
///

void DatabaseDemos::markAsImported(bool imported, unsigned int recordId, DbRecordType type)
{
    if(type == DbRecordType::RT_BIRTH){
        _worker.setBirthRecordImported(imported, recordId);
    } else if(type == DbRecordType::RT_BURIAL){
        _worker.setBurialRecordImported(imported, recordId);
    } else if(type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_GROOM || type == DbRecordType::RT_MARRIAGE_BRIDE){
        _worker.setMarriageRecordImported(imported, recordId);
    }
}


bool DatabaseDemos::synchronize(Record &record)
{
    QSettings settings;

    Identifier dbRecordFirst = record.getIdentifierDbRecordFirst();
    Identifier dbRecordLast = record.getIdentifierDbRecordLast();

    if(!_worker.isOpen()){
        return setError(_worker.lastError());;
    }

    //Update imported status of database records
    for(Identifier current = dbRecordFirst; current != dbRecordLast; record.getIdentifierDbRecordNext(current)){
        unsigned int pk = record.getDbRecordPrimaryKey(current);
        bool imported = record.getDbRecordImported(current);
        DbRecordType type = record.getDbRecordType(current);
        if(type == DbRecordType::RT_BIRTH){
            _worker.setBirthRecordImported(imported, pk);
        } else if(type == DbRecordType::RT_BURIAL){
            _worker.setBurialRecordImported(imported, pk);
        } else if(type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_GROOM || type == DbRecordType::RT_MARRIAGE_BRIDE ){
            _worker.setMarriageRecordImported(imported, pk);
        }
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
    }


    // Remove old temporary records
    record.deleteDbRecords();

    _preferNormalized = settings.value("dbimporter/dbimporterdemos/preferNormalized", false).toBool();

    auto rows = _worker.getMarkedRecords();
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }

    emit this->objectsToImport(rows.count());



    foreach(auto row, rows){
        bool importOk = false;
        bool imported = row.value("imported").toBool();
        bool conversionOk = false;
        unsigned int id;
        if((id = row.value("birth_id").toUInt(&conversionOk)) && conversionOk){
            importOk = importRecord(DbRecordType::RT_BIRTH, id, imported, record);
        } else if ((id = row.value("marriage_id").toUInt(&conversionOk)) && conversionOk) {
            importOk = importRecord(DbRecordType::RT_MARRIAGE, id, imported, record);
        } else if ((id = row.value("burial_id").toUInt(&conversionOk)) && conversionOk) {
            importOk = importRecord(DbRecordType::RT_BURIAL, id, imported, record);
        }

        if(!importOk){
            emit this->importDone();
            return false;
        }
        emit this->objectImported();
    }
    emit this->importDone();
    return true;
}


bool DatabaseDemos::importRecord(DbRecordType recordType, unsigned int recordId, bool imported, Record &record)
{
    if(recordType == DbRecordType::RT_BIRTH){
        return importBirthRecord(recordId, imported, record);
    } else if(recordType == DbRecordType::RT_BURIAL){
        return importBurialRecord(recordId, imported, record);
    } else if(recordType == DbRecordType::RT_MARRIAGE){
        return importMarriageRecord(recordId, imported, record);
    }

    return false;
}

bool DatabaseDemos::importBirthRecord(unsigned int recordId, bool imported, Record &record)
{
    auto individuals = _worker.getBirthRecordIndividuals(recordId);
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }


    if(individuals.isEmpty()){
        return true;
    }
    Identifier dbRecordRef = record.newDbRecord(DbRecordType::RT_BIRTH, recordId);
    record.setDbRecordImported(dbRecordRef, imported);

    foreach(auto row, individuals){
        DbRecordRole role = roleToEnum(row.value("rel").toString());
        Identifier individualRef = record.newDbIndividual(dbRecordRef, role, 0);
        if(!importIndividual(individualRef, row, record)){
            return false;
        }
    }

    return addBirthRecordData(recordId, dbRecordRef, record);
//    if(!addBirthRecordData(recordId, dbRecordRef, record)){
//        return setError("Uh3");
//    } else {
//        return true;
//    }
}

bool DatabaseDemos::importBurialRecord(unsigned int recordId, bool imported, Record &record)
{
    auto individuals = _worker.getBurialRecordIndividuals(recordId);
    if(individuals.isEmpty()){
        return true;
    }
    Identifier dbRecordRef = record.newDbRecord(DbRecordType::RT_BURIAL, recordId);
    record.setDbRecordImported(dbRecordRef, imported);
    foreach(auto row, individuals){
        DbRecordRole role = roleToEnum(row.value("rel").toString());
        Identifier individualRef = record.newDbIndividual(dbRecordRef, role, 0);
        if(!importIndividual(individualRef, row, record)){
            return false;
        }
    }

    return addBurialRecordData(recordId, dbRecordRef, record);
}

bool DatabaseDemos::importMarriageRecord(unsigned int recordId, bool imported, Record &record)
{
    auto individuals = _worker.getMarriageRecordIndividuals(recordId);
    if(individuals.isEmpty()){
        return true;
    }
        Identifier dbRecordRef = record.newDbRecord(DbRecordType::RT_MARRIAGE, recordId);
        record.setDbRecordImported(dbRecordRef, imported);

    foreach(auto row, individuals){
        DbRecordRole role = roleToEnum(row.value("rel").toString());
        Identifier individualRef = record.newDbIndividual(dbRecordRef, role, 0);
        if(!importIndividual(individualRef, row, record)){
            return false;
        }
    }

    return addMarriageRecordData(recordId, dbRecordRef, record);
}


bool DatabaseDemos::importIndividual(Identifier ref, DatabaseRow row, Record &record)
{
    // Set individual's gender as undetermined to prevent the value remaining empty
    record.setIndividualGender(ref, Gender::UNDET);

    // Get given names and gender from the 'person_name' and 'name' tables

    {
        QPair<QString, Gender> namesGender = getGivenNames(row.value("id").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        record.setIndividualNameGiven(ref,namesGender.first);
        if(namesGender.second != Gender::UNDET)
        {
            record.setIndividualGender(ref, namesGender.second);
        }
    }

    // Surname
    {
        QString surname = row.value("surname").toString();
        QString normSurname = row.value("normalizedSurname").toString();

        if(_preferNormalized && !normSurname.isEmpty()){
            record.setIndividualNameSurname(ref, normSurname);
        } else {
            record.setIndividualNameSurname(ref, surname);
        }

        QChar genderCh = row.value("sexSurname").toChar();
        if(genderCh == 'M'){
            record.setIndividualGender(ref, Gender::MALE);
        } else if(genderCh == 'F'){
            record.setIndividualGender(ref, Gender::FEMALE);
        }
    }

    // Get name prefix (title)
    record.setIndividualNamePrefix(ref, row.value("title").toString());


    {
        // Get Religion
        QString religionDesc = row.value("religion").toString();
        if(!religionDesc.isEmpty())
        {
            unsigned int relIndex = record.setIndividualReli(ref);
            record.setIndividualReliDescription(ref, relIndex, religionDesc);
        }
    }

    {
        // Occupations
        QVector<QString> occupations = getOccupations(row.value("id").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        for(auto o = occupations.begin(); o!=occupations.end();++o)
        {
            unsigned int occuIndex = record.setIndividualOccu(ref);
            record.setIndividualOccuDescription(ref, occuIndex, *o);
        }
    }


    {
        // Birth date
        QString bstr = row.value("birth_date").toString();
        Date birthDate = dbDateToDate(bstr);
        if(birthDate.isValid())
        {
            record.setIndividualBirthDate1(ref,birthDate);
            record.setIndividualBirthDateType(ref, DateType::SIMPLE);
        }
    }

    {
        // Death date
        Date deathDate = dbDateToDate(row.value("dead_date").toString());
        if(deathDate.isValid())
        {
            record.setIndividualDeceased(ref, true);
            record.setIndividualDeathDate1(ref,deathDate);
            record.setIndividualDeathDateType(ref, DateType::SIMPLE);
        }
    }

    {
        // Address
        QString domicile;
        QString dom = row.value("domicile").toString();
        QString normDom = row.value("normalizedDomicile").toString();

        if(_preferNormalized && !normDom.isEmpty()){
            domicile = normDom;
        } else {
            domicile = dom;
        }


        QString street = row.value("street").toString();
        QString descNum = row.value("descr_num").toString();
        if(!domicile.isEmpty() || !street.isEmpty() || !descNum.isEmpty())
        {
            unsigned int resIndex = record.setIndividualResi(ref);
            if(!domicile.isEmpty()){
                record.setIndividualResiAddrCity(ref,resIndex,domicile);
            }
            if(!street.isEmpty() || !descNum.isEmpty()){
                record.setIndividualResiAddrAddr(ref,resIndex,street + descNum);
            }
        }
    }
    return true;

}



bool DatabaseDemos::addBirthRecordData(unsigned int recordId, Identifier recordRef, Record &record)
{

    auto rows = _worker.getBirthRecord(recordId);
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }

    if(rows.length() != 1){
        return setError("Internal synchronization error: Unexpected number of recieved rows");
    }

    auto row = rows.first();

    Identifier mainRef = record.getDbRecordIndividualByRole(recordRef,DbRecordRole::BIR_MAIN);

    Date sourceDate = Date();

    {
        // Baptism date
        Date bapDate = dbDateToDate(row.value("baptism_date").toString());
        if(bapDate.isValid()){
            record.setIndividualChristeningDate1(mainRef, bapDate);
            record.setIndividualChristeningDateType(mainRef, DateType::SIMPLE);
        }
    }

    {
        // Gender
        QString gender = row.value("sex").toString();
        if(gender == 'M'){
            record.setIndividualGender(mainRef, Gender::MALE);
        } else if (gender == 'F') {
            record.setIndividualGender(mainRef, Gender::FEMALE);
        }
    }


    {
        // Birth date
        Date birthDate = dbDateToDate(row.value("birth_date").toString());
        if(birthDate.isValid()){
            sourceDate = birthDate;
            record.setIndividualBirthDate1(mainRef, birthDate);
            record.setIndividualBirthDateType(mainRef, DateType::SIMPLE);
        }
    }

    {
        // Death
        QString deathPlace = row.value("dead_where").toString();
        if(!deathPlace.isEmpty()){
            record.setIndividualDeceased(mainRef, true);
        }
        record.setIndividualDeathPlaceName(mainRef, deathPlace);
    }

    {
        // Parents marriage date
        QString marrDateStr = row.value("parents_marr_when").toString();
        Date marrDate = dbDateToDate(marrDateStr);
        if(marrDate.isValid()){
            record.setDbRecordMarriageDate1(recordRef, marrDate);
            record.setDbRecordMarriageDateType(recordRef, DateType::SIMPLE);
        }
    }


    {
        // Note
        QString note = row.value("comment").toString();
        if(!note.isEmpty()){
            record.setIndividualNote(mainRef,"Birth:" + note);
        }
    }

    {
        // Source
        if(!addSource(row,recordRef,DbRecordType::RT_BIRTH, record, "", sourceDate)){
            return false;
        }
    }

    return true;
}

bool DatabaseDemos::addBurialRecordData(unsigned int recordId, Identifier recordRef, Record &record)
{
    auto rows = _worker.getBurialRecord(recordId);
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }

    if(rows.length() != 1){
        return setError("Internal synchronization error: Unexpected number of recieved rows");
    }

    Date sourceDate = Date();
    QString sourcePlac;

    auto row = rows.first();

    Identifier mainRef = record.getDbRecordIndividualByRole(recordRef,DbRecordRole::BUR_MAIN);

    // Set individual as dead
    record.setIndividualDeceased(mainRef, true);

    {
        // Gender
        QString gender = row.value("sex").toString();
        if(gender == 'M'){
            record.setIndividualGender(mainRef, Gender::MALE);
        } else if (gender == 'F') {
            record.setIndividualGender(mainRef, Gender::FEMALE);
        }
    }

    {
        // Death date
        Date deathDate = dbDateToDate(row.value("dead_date").toString());
        if(deathDate.isValid()){
            sourceDate = deathDate;
            record.setIndividualDeathDate1(mainRef, deathDate);
            record.setIndividualDeathDateType(mainRef, DateType::SIMPLE);
        }
    }

    {
        // Death place
        QString deathAddress = getAddress(row.value("death_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }

        QString deathPlaceName = row.value("death_place").toString();
        QString deathPlaceFull = deathAddress + (!deathPlaceName.isEmpty()?" (" + deathPlaceName + ")":"");

        if(!deathPlaceFull.isEmpty()){
            sourcePlac = deathPlaceFull;
            record.setIndividualDeathPlaceName(mainRef, deathPlaceFull);
        }
    }

    {
        // Burial date
        Date burDate = dbDateToDate(row.value("burial_date").toString());
        if(burDate.isValid()){
            record.setIndividualBurialDate1(mainRef, burDate);
            record.setIndividualBurialDateType(mainRef, DateType::SIMPLE);
        }
    }

    {
        // Burial place
        QString burPlace = row.value("burial_place").toString();
        if(!burPlace.isEmpty()){
            record.setIndividualBurialPlaceName(mainRef, burPlace);
        }
    }

    {
        // Death cause
        QString deathCause;
        QString dC = row.value("death_cause").toString();
        QString normDC = row.value("normalizedDeathCause").toString();
        if(_preferNormalized && !normDC.isEmpty()){
            deathCause = normDC;
        } else {
            deathCause = dC;
        }

        record.setIndividualDeathCause(mainRef, deathCause);

    }

    {
        // Birth address
        QString birthAddress = getAddress(row.value("birth_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }

        if(!birthAddress.isEmpty()){
            record.setIndividualBirthPlaceName(mainRef, birthAddress);
        }
    }

    {
        // Marriage date
        QString marrDateStr = row.value("marriage_date").toString();
        Date marrDate = dbDateToDate(marrDateStr);
        if(marrDate.isValid()){
            record.setDbRecordMarriageDate1(recordRef, marrDate);
            record.setDbRecordMarriageDateType(recordRef, DateType::SIMPLE);
        }
    }

    QString note = row.value("comment").toString();
    if(!note.isEmpty()){
        record.setIndividualNote(mainRef,"Death:" + note);
    }

    // Source
    if(!addSource(row,recordRef,DbRecordType::RT_BURIAL, record, sourcePlac, sourceDate)){
        return false;
    }

    return true;
}

bool DatabaseDemos::addMarriageRecordData(unsigned int recordId, Identifier recordRef, Record &record)
{
    auto rows = _worker.getMarriageRecord(recordId);
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }
    if(rows.length() != 1){
        return setError("Internal synchronization error: Unexpected number of recieved rows");
    }

    auto row = rows.first();

    Date sourceDate = Date();
    QString sourcePlac;

    Identifier groomRef = record.getDbRecordIndividualByRole(recordRef,DbRecordRole::MAR_GROOM);
    Identifier brideRef = record.getDbRecordIndividualByRole(recordRef,DbRecordRole::MAR_BRIDE);

    {
        // Domicile
        QString domicile;
        QString dom = row.value("domicile").toString();
        QString normDom = row.value("normalizedDomicile").toString();

        if(_preferNormalized && !normDom.isEmpty()){
            domicile = normDom;

        } else {
            domicile = dom;
        }

        sourcePlac = domicile;
        record.setDbRecordMarriagePlace(recordRef, domicile);
//        record.setDbRecordMarriagePlace(groomRecRef, domicile);
    }

    {
        // Marriage date
        Date marrDate = dbDateToDate(row.value("marriage_date").toString());
        if(marrDate.isValid()){
            sourceDate = marrDate;
            record.setDbRecordMarriageDate1(recordRef, marrDate);
            record.setDbRecordMarriageDateType(recordRef, DateType::SIMPLE);
        } else{
            record.setDbRecordMarriageDateType(recordRef, DateType::EMPT);
        }
    }

    {
        // Divorce date
        Date divDate = dbDateToDate(row.value("divorce_date").toString());
        if(divDate.isValid()){
            record.setDbRecordDivorceDate1(recordRef, divDate);
            record.setDbRecordDivorceDateType(recordRef, DateType::SIMPLE);

        } else {
            record.setDbRecordDivorceDateType(recordRef, DateType::EMPT);
        }
    }

    // Birth addresses
    {
        QString groomBirthAdd = getAddress(row.value("groom_birth_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        QString brideBirthAdd = getAddress(row.value("bride_birth_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        if(!groomBirthAdd.isEmpty()){
            record.setIndividualBirthPlaceName(groomRef, groomBirthAdd);
        }
        if(!brideBirthAdd.isEmpty()){
            record.setIndividualBirthPlaceName(brideRef, brideBirthAdd);
        }
    }


    // Death addresses
    {
        QString groomDeathAdd = getAddress(row.value("groom_dead_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        QString brideDeathAdd = getAddress(row.value("bride_dead_address").toUInt());
        if(!_worker.lastOperationOk()){
            return setError(_worker.lastError());
        }
        if(!groomDeathAdd.isEmpty()){
            record.setIndividualDeceased(groomRef, true);
            record.setIndividualDeathPlaceName(groomRef, groomDeathAdd);
        }
        if(!brideDeathAdd.isEmpty()){
            record.setIndividualDeceased(groomRef, true);
            record.setIndividualDeathPlaceName(brideRef, brideDeathAdd);
        }

        QString note = row.value("comment").toString();
        if(!note.isEmpty()){
            record.setIndividualNote(groomRef,"Marriage:" + note);
            record.setIndividualNote(groomRef,"Marriage:" + note);
        }
    }

    // Source
    if(!addSource(row,recordRef,DbRecordType::RT_MARRIAGE, record, sourcePlac, sourceDate)){
        return false;
    }

    return true;
}

bool DatabaseDemos::addSource(DatabaseRow row, Identifier recordRef, DbRecordType type, Record &record, QString plac, Date date)
{
//    QString scanNo = row.value("scan").toString();
    bool regIdOk = false;
    unsigned int regId = row.value("register_id").toInt(&regIdOk);
    if(!regIdOk){
        return true;
    }
    auto regRows = _worker.getSourceInfo(regId);
    if(!_worker.lastOperationOk()){
        return setError(_worker.lastError());
    }

    if(regRows.isEmpty()){
        return setError("Internal error: register info empty result.");
    }

    Identifier srcRef = record.getDbRecordSourceRef(recordRef);

    DatabaseRow regRow = regRows.first();

    QString signature = regRow.value("signature").toString();
    QString archive = regRow.value("archiveName").toString();


    CommonEventType evType = CommonEventType::EMPT;
    if(type == DbRecordType::RT_BIRTH){
        evType = CommonEventType::BIRT;
    } else if(type == DbRecordType::RT_BURIAL){
        evType = CommonEventType::DEAT;
    } else if(type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_BRIDE || type == DbRecordType::RT_MARRIAGE_GROOM){
        evType = CommonEventType::MARR;
    }

    unsigned int index = record.setSourceData(srcRef, evType);

    record.setSourceArchive(srcRef, archive);
    record.setSourceTitle(srcRef, signature);

//    record.setSourceDataPage1(srcRef,evType,index,scanNo);
    record.setSourceDataPlac(srcRef, evType, index, plac);
    if(date.isValid()){
        record.setSourceDataDateFrom(srcRef, evType, index, date);
    }

    return true;
}



QString DatabaseDemos::getAddress(unsigned int id)
{
    auto rows = _worker.getAddress(id);
    if(rows.isEmpty()){
        return QString();
    }
    auto row = rows.first();

    QString address;

    QString domicile;
    QString dom = row.value("domicile").toString();
    QString normDom = row.value("normalizedDomicile").toString();

    if(_preferNormalized && !normDom.isEmpty()){
        domicile = normDom;
    } else {
        domicile = dom;
    }

    QString street = row.value("street").toString();
    QString descrNum = row.value("descr_num").toString();

    address = street;
    if(!street.isEmpty()){
        address += " " + descrNum;
        if(!domicile.isEmpty()){
            address += ", " + domicile;
        }
    } else {
        address = domicile;
        if(!descrNum.isEmpty()){
            address += ", " + descrNum;
        }
    }
    return address;
}


Date DatabaseDemos::dbDateToDate(QString date) const
{
    return Date::fromString(date,"yyyy-MM-dd","yyyy-MM-??","yyyy-??-??");
}

bool DatabaseDemos::isBrideSide(DbRecordRole role)
{
    switch(role){
    case DbRecordRole::MAR_BRIDE :
    case DbRecordRole::MAR_PRIEST :
    case DbRecordRole::MAR_B_F :
    case DbRecordRole::MAR_B_M :
    case DbRecordRole::MAR_B_M_F :
    case DbRecordRole::MAR_B_M_M :
    case DbRecordRole::MAR_B_FOST :
    case DbRecordRole::MAR_WIDOWER :
    case DbRecordRole::MAR_SV_1 :
    case DbRecordRole::MAR_SVREL_1 :
    case DbRecordRole::MAR_SV_2 :
    case DbRecordRole::MAR_SVREL_2 :
    case DbRecordRole::MAR_SV_3 :
    case DbRecordRole::MAR_SVREL_3 :
    case DbRecordRole::MAR_SV_4 :
    case DbRecordRole::MAR_SVREL_4 :
    case DbRecordRole::MAR_SPEAKER :
    case DbRecordRole::MAR_STARA :
    case DbRecordRole::MAR_BESTMAN :
    case DbRecordRole::MAR_BRIDESMAID :
        return true;
    default:
        return false;
    }
}

bool DatabaseDemos::isGroomSide(DbRecordRole role)
{
    switch(role){
    case DbRecordRole::MAR_GROOM :
    case DbRecordRole::MAR_PRIEST :
    case DbRecordRole::MAR_G_F :
    case DbRecordRole::MAR_G_M :
    case DbRecordRole::MAR_G_M_F :
    case DbRecordRole::MAR_G_M_M :
    case DbRecordRole::MAR_G_FOST :
    case DbRecordRole::MAR_WIDOW :
    case DbRecordRole::MAR_SV_1 :
    case DbRecordRole::MAR_SVREL_1 :
    case DbRecordRole::MAR_SV_2 :
    case DbRecordRole::MAR_SVREL_2 :
    case DbRecordRole::MAR_SV_3 :
    case DbRecordRole::MAR_SVREL_3 :
    case DbRecordRole::MAR_SV_4 :
    case DbRecordRole::MAR_SVREL_4 :
    case DbRecordRole::MAR_SPEAKER :
    case DbRecordRole::MAR_STARA :
    case DbRecordRole::MAR_BESTMAN :
    case DbRecordRole::MAR_BRIDESMAID :
        return true;
    default:
        return false;
    }
}

bool DatabaseDemos::setError(QString error)
{
    _error = error;
    return false;
}

void DatabaseDemos::clearError()
{
    _error = "";
}

QVector<QString> DatabaseDemos::getOccupations(unsigned int personId){
    auto rows = _worker.getIndividualOccupations(personId);
    QVector<QString> list;
    foreach(auto row, rows){
        QString occup = row.value("occupation").toString();
        QString normOccup = row.value("normalizedOccupation").toString();
        if(_preferNormalized && !normOccup.isEmpty()){
            list.append(normOccup);
        } else {
            list.append(occup);
        }
    }
    return list;
}

QPair<QString, Gender> DatabaseDemos::getGivenNames(unsigned int personId)
{
    QVector<DatabaseRow> stmt = _worker.getIndividualGivenNames(personId);
    QString names;
    QString genderCh;
    Gender gender;
    foreach(DatabaseRow row, stmt)
    {
        QString normName = row.value("normalizedName").toString();
        QString name = row.value("name").toString();
        if(_preferNormalized && !normName.isEmpty()){
            names.append(normName + " ");
        } else {
            names.append(name + " ");
        }
        genderCh = row.value("sex").toString();
    }

    if(!names.isEmpty()){
        names.chop(1);
    }

    if(genderCh == 'M'){
        gender = Gender::MALE;
    } else if(genderCh == 'F'){
        gender = Gender::FEMALE;
    } else {
        gender = Gender::UNDET;
    }

    return QPair<QString, Gender>(names,gender);
}


DbRecordRole DatabaseDemos::roleToEnum(QString roleString) const
{
    QHash<QString, DbRecordRole>::const_iterator it = _roleResolveTable.find(roleString);
    if(it != _roleResolveTable.end()){
        return it.value();
    }
    return DbRecordRole::UNDEFINED;
}



void DatabaseDemos::initDbRecordRoleResolveTable()
{
    _roleResolveTable.insert("main",DbRecordRole::BIR_MAIN);
    _roleResolveTable.insert("f",DbRecordRole::BIR_F);
    _roleResolveTable.insert("m",DbRecordRole::BIR_M);
    _roleResolveTable.insert("midwife",DbRecordRole::BIR_MIDWIFE);
    _roleResolveTable.insert("granted",DbRecordRole::BIR_GRANTED);
    _roleResolveTable.insert("f_f",DbRecordRole::BIR_F_F);
    _roleResolveTable.insert("f_m",DbRecordRole::BIR_F_M);
    _roleResolveTable.insert("f_m_f",DbRecordRole::BIR_F_M_F);
    _roleResolveTable.insert("f_m_m",DbRecordRole::BIR_F_M_M);
    _roleResolveTable.insert("m_f",DbRecordRole::BIR_M_F);
    _roleResolveTable.insert("m_m",DbRecordRole::BIR_M_M);
    _roleResolveTable.insert("m_m_f",DbRecordRole::BIR_M_M_F);
    _roleResolveTable.insert("m_m_m",DbRecordRole::BIR_M_M_M);
    _roleResolveTable.insert("gf_1",DbRecordRole::BIR_GF_1);
    _roleResolveTable.insert("gf_2",DbRecordRole::BIR_GF_2);
    _roleResolveTable.insert("gf_3",DbRecordRole::BIR_GF_3);
    _roleResolveTable.insert("gf_4",DbRecordRole::BIR_GF_4);
    _roleResolveTable.insert("gfrel_1",DbRecordRole::BIR_GFREL_1);
    _roleResolveTable.insert("gfrel_2",DbRecordRole::BIR_GFREL_2);
    _roleResolveTable.insert("gfrel_3",DbRecordRole::BIR_GFREL_3);
    _roleResolveTable.insert("gfrel_4",DbRecordRole::BIR_GFREL_4);
    _roleResolveTable.insert("husband",DbRecordRole::BIR_HUSBAND);
    _roleResolveTable.insert("bapt_husband",DbRecordRole::BIR_BAPT_HUSBAND);
    _roleResolveTable.insert("mar_groom",DbRecordRole::MAR_GROOM);
    _roleResolveTable.insert("mar_bride",DbRecordRole::MAR_BRIDE);
    _roleResolveTable.insert("mar_priest",DbRecordRole::MAR_PRIEST);
    _roleResolveTable.insert("mar_widower",DbRecordRole::MAR_WIDOWER);
    _roleResolveTable.insert("mar_g_f",DbRecordRole::MAR_G_F);
    _roleResolveTable.insert("mar_g_m",DbRecordRole::MAR_G_M);
    _roleResolveTable.insert("mar_g_m_f",DbRecordRole::MAR_G_M_F);
    _roleResolveTable.insert("mar_g_m_m",DbRecordRole::MAR_G_M_M);
    _roleResolveTable.insert("mar_g_fost",DbRecordRole::MAR_G_FOST);
    _roleResolveTable.insert("mar_widow",DbRecordRole::MAR_WIDOW);
    _roleResolveTable.insert("mar_b_f",DbRecordRole::MAR_B_F);
    _roleResolveTable.insert("mar_b_m",DbRecordRole::MAR_B_M);
    _roleResolveTable.insert("mar_b_m_f",DbRecordRole::MAR_B_M_F);
    _roleResolveTable.insert("mar_b_m_m",DbRecordRole::MAR_B_M_M);
    _roleResolveTable.insert("mar_b_fost",DbRecordRole::MAR_B_FOST);
    _roleResolveTable.insert("mar_sv_1",DbRecordRole::MAR_SV_1);
    _roleResolveTable.insert("mar_svrel_1",DbRecordRole::MAR_SVREL_1);
    _roleResolveTable.insert("mar_sv_2",DbRecordRole::MAR_SV_2);
    _roleResolveTable.insert("mar_svrel_2",DbRecordRole::MAR_SVREL_2);
    _roleResolveTable.insert("mar_sv_3",DbRecordRole::MAR_SV_3);
    _roleResolveTable.insert("mar_svrel_3",DbRecordRole::MAR_SVREL_3);
    _roleResolveTable.insert("mar_sv_4",DbRecordRole::MAR_SV_4);
    _roleResolveTable.insert("mar_svrel_4",DbRecordRole::MAR_SVREL_4);
    _roleResolveTable.insert("mar_speaker",DbRecordRole::MAR_SPEAKER);
    _roleResolveTable.insert("mar_stara",DbRecordRole::MAR_STARA);
    _roleResolveTable.insert("mar_bestman",DbRecordRole::MAR_BESTMAN);
    _roleResolveTable.insert("mar_bridesmaid",DbRecordRole::MAR_BRIDESMAID);
    _roleResolveTable.insert("bur_examinator",DbRecordRole::BUR_EXAMINATOR);
    _roleResolveTable.insert("bur_keeper",DbRecordRole::BUR_KEEPER);
    _roleResolveTable.insert("bur_gravedigger",DbRecordRole::BUR_GRAVEDIGGER);
    _roleResolveTable.insert("bur_main",DbRecordRole::BUR_MAIN);
    _roleResolveTable.insert("bur_f",DbRecordRole::BUR_F);
    _roleResolveTable.insert("bur_m",DbRecordRole::BUR_M);
    _roleResolveTable.insert("bur_m_f",DbRecordRole::BUR_M_F);
    _roleResolveTable.insert("bur_m_m",DbRecordRole::BUR_M_M);
    _roleResolveTable.insert("bur_husband",DbRecordRole::BUR_HUSBAND);
    _roleResolveTable.insert("bur_wife",DbRecordRole::BUR_WIFE);
    _roleResolveTable.insert("bur_son",DbRecordRole::BUR_SON);
    _roleResolveTable.insert("bur_daughter",DbRecordRole::BUR_DAUGHTER);
    _roleResolveTable.insert("bur_rel1",DbRecordRole::BUR_REL1);
    _roleResolveTable.insert("undefined",DbRecordRole::UNDEFINED);
}
