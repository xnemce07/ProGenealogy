/*!
 *  \file   mergebox.cpp
 *  \author Leopold Nemcek
 */
#include "mergebox.h"

MergeBox::MergeBox(QString title, Kernel &k, Identifier &p, QWidget *parent): Box(title, BOX_NOT_CHECKABLE, k, p, parent)
{
    _mergeInd = _kernel.record()->getMergeIndividual();

    _spacer = new QSpacerItem(1,1,QSizePolicy::Preferred, QSizePolicy::Expanding);

    this->fill();

    connect(&_pbMerge, &QPushButton::clicked, this, &MergeBox::mergeClicked);
    connect(&_pbReset, &QPushButton::clicked, this, &MergeBox::resetClicked);
}

void MergeBox::update(){

    this->remove();


    if(!_dbIndi.isValid() || !_localIndi.isValid()){
        return;
    }


    ///////////////////////////////////////////////////
    /// GENERAL
    ///////////////////////////////////////////////////

    // Given name
    {
        QString dbString = _kernel.record()->getIndividualNameGiven(_dbIndi);
        QString lString = _kernel.record()->getIndividualNameGiven(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualNameGiven(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Given name"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Birth surname
    {
        QString dbString = _kernel.record()->getIndividualNameSurnameBirth(_dbIndi);
        QString lString = _kernel.record()->getIndividualNameSurnameBirth(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualNameSurname(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Birth name"), dbString, lString, nameL);
            _items.append(sub);
        }
    }

    // Married surname
    {
        QString dbString = _kernel.record()->getIndividualNameSurnameMarried(_dbIndi);
        QString lString = _kernel.record()->getIndividualNameSurnameMarried(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualNameMarname(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Married name"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Gender
    {
        Gender dbGender = _kernel.record()->getIndividualGender(_dbIndi);
        Gender lGender = _kernel.record()->getIndividualGender(_localIndi);

        if(dbGender != lGender && dbGender != Gender::UNDET){
            SubboxMergeGender *sub = new SubboxMergeGender(_kernel,tr("Gender"),dbGender,lGender);
            _items.append(sub);
        }
    }

    // Title in front
    {
        QString dbString = _kernel.record()->getIndividualNamePrefix(_dbIndi);
        QString lString = _kernel.record()->getIndividualNamePrefix(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualNamePrefix(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Title in front"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    //Title after
    {
        QString dbString = _kernel.record()->getIndividualNameSuffix(_dbIndi);
        QString lString = _kernel.record()->getIndividualNameSuffix(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualNameSuffix(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Title after"), dbString, lString, nameL);

            _items.append(sub);
        }
    }


    ///////////////////////////////////////////////////
    /// BIRTH
    ////////////////////////////////////////////////////

    // Birth date
    {
        Date dbDate1 = _kernel.record()->getIndividualBirthDate1(_dbIndi);
        Date dbDate2 = _kernel.record()->getIndividualBirthDate2(_dbIndi);
        DateType dbDateType = _kernel.record()->getIndividualBirthDateType(_dbIndi);

        Date lDate1 = _kernel.record()->getIndividualBirthDate1(_localIndi);
        Date lDate2 = _kernel.record()->getIndividualBirthDate2(_localIndi);
        DateType lDateType = _kernel.record()->getIndividualBirthDateType(_localIndi);

        if((dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType)  && dbDateType != DateType::EMPT ){

            auto birthL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                k.record()->setIndividualBirthDate1(ref, d1);
                k.record()->setIndividualBirthDate2(ref, d2);
                k.record()->setIndividualBirthDateType(ref, dt);
            };


            SubboxMergeDate *sub = new SubboxMergeDate(_kernel, _mergeInd, tr("Birth date"), birthL);
            sub->setDbDate(dbDate1, dbDate2, dbDateType);
            sub->setLocalDate(lDate1, lDate2, lDateType);
            sub->finish();
            _items.append(sub);
        }
    }

    // Birth place
    {
        QString dbString = _kernel.record()->getIndividualBirthPlaceName(_dbIndi);
        QString lString = _kernel.record()->getIndividualBirthPlaceName(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualBirthPlaceName(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Birth place"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Date of christening
    {
        Date dbDate1 = _kernel.record()->getIndividualChristeningDate1(_dbIndi);
        Date dbDate2 = _kernel.record()->getIndividualChristeningDate2(_dbIndi);
        DateType dbDateType = _kernel.record()->getIndividualChristeningDateType(_dbIndi);

        Date lDate1 = _kernel.record()->getIndividualChristeningDate1(_localIndi);
        Date lDate2 = _kernel.record()->getIndividualChristeningDate2(_localIndi);
        DateType lDateType = _kernel.record()->getIndividualChristeningDateType(_localIndi);

        if(dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType ){

            auto birthL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                k.record()->setIndividualChristeningDate1(ref, d1);
                k.record()->setIndividualChristeningDate2(ref, d2);
                k.record()->setIndividualChristeningDateType(ref, dt);
            };


            SubboxMergeDate *sub = new SubboxMergeDate(_kernel, _mergeInd, tr("Christening date"), birthL);
            sub->setDbDate(dbDate1, dbDate2, dbDateType);
            sub->setLocalDate(lDate1, lDate2, lDateType);
            sub->finish();
            _items.append(sub);
        }
    }

    // Date of marriage of parents
    {
        DbRecordRole dbIndiRole = _kernel.record()->getDbRecordIndividualRole(_dbrecord, _dbIndi);
        if(dbIndiRole == DbRecordRole::BIR_F || dbIndiRole == DbRecordRole::BIR_M){
            Identifier mergeFam = _kernel.record()->getMergeFamily();

            {

                Date dbDate1 = _kernel.record()->getDbRecordMarriageDate1(_dbrecord);
                Date dbDate2 = _kernel.record()->getDbRecordMarriageDate2(_dbrecord);
                DateType dbDateType = _kernel.record()->getDbRecordMarriageDateType(_dbrecord);

                Date lDate1 = _kernel.record()->getFamilyMarriageDate1(mergeFam);
                Date lDate2 = _kernel.record()->getFamilyMarriageDate2(mergeFam);
                DateType lDateType = _kernel.record()->getFamilyMarriageDateType(mergeFam);

                if(dbDateType != DateType::EMPT && _partner.isValid() && ( dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType))
                {
                    auto marrL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                        k.record()->setFamilyMarried(ref, true);
                        k.record()->setFamilyMarriageDate1(ref, d1);
                        k.record()->setFamilyMarriageDate2(ref, d2);
                        k.record()->setFamilyMarriageDateType(ref, dt);
                    };


                    SubboxMergeDate *sub = new SubboxMergeDate(_kernel, mergeFam ,tr("Marriage date"), marrL);
                    sub->setDbDate(dbDate1, dbDate2, dbDateType);
                    sub->setLocalDate(lDate1, lDate2, lDateType);
                    sub->finish();
                    _items.append(sub);
                }
            }
        }
    }

    ///////////////////////////////////////////////////
    /// DEATH
    ////////////////////////////////////////////////////

    // Date of death
    {
        Date dbDate1 = _kernel.record()->getIndividualDeathDate1(_dbIndi);
        Date dbDate2 = _kernel.record()->getIndividualDeathDate2(_dbIndi);
        DateType dbDateType = _kernel.record()->getIndividualDeathDateType(_dbIndi);

        Date lDate1 = _kernel.record()->getIndividualDeathDate1(_localIndi);
        Date lDate2 = _kernel.record()->getIndividualDeathDate2(_localIndi);
        DateType lDateType = _kernel.record()->getIndividualDeathDateType(_localIndi);

        if((dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType)  && dbDateType != DateType::EMPT){

            auto birthL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                k.record()->setIndividualDeathDate1(ref, d1);
                k.record()->setIndividualDeathDate2(ref, d2);
                k.record()->setIndividualDeathDateType(ref, dt);
            };


            SubboxMergeDate *sub = new SubboxMergeDate(_kernel, _mergeInd, tr("Death date"), birthL);
            sub->setDbDate(dbDate1, dbDate2, dbDateType);
            sub->setLocalDate(lDate1, lDate2, lDateType);
            sub->finish();
            _items.append(sub);
        }
    }

    // Place of death
    {
        QString dbString = _kernel.record()->getIndividualDeathPlaceName(_dbIndi);
        QString lString = _kernel.record()->getIndividualDeathPlaceName(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualDeathPlaceName(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Place of death"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Cause of death
    {
        QString dbString = _kernel.record()->getIndividualDeathCause(_dbIndi);
        QString lString = _kernel.record()->getIndividualDeathCause(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualDeathCause(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Death cause"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Date of burial
    {
        Date dbDate1 = _kernel.record()->getIndividualBurialDate1(_dbIndi);
        Date dbDate2 = _kernel.record()->getIndividualBurialDate2(_dbIndi);
        DateType dbDateType = _kernel.record()->getIndividualBurialDateType(_dbIndi);

        Date lDate1 = _kernel.record()->getIndividualBurialDate1(_localIndi);
        Date lDate2 = _kernel.record()->getIndividualBurialDate2(_localIndi);
        DateType lDateType = _kernel.record()->getIndividualBurialDateType(_localIndi);

        if((dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType)  && dbDateType != DateType::EMPT){

            auto birthL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                k.record()->setIndividualBurialDate1(ref, d1);
                k.record()->setIndividualBurialDate2(ref, d2);
                k.record()->setIndividualBurialDateType(ref, dt);
            };


            SubboxMergeDate *sub = new SubboxMergeDate(_kernel, _mergeInd, tr("Burial date"), birthL);
            sub->setDbDate(dbDate1, dbDate2, dbDateType);
            sub->setLocalDate(lDate1, lDate2, lDateType);
            sub->finish();
            _items.append(sub);
        }
    }

    // Place of burial
    {
        QString dbString = _kernel.record()->getIndividualBurialPlaceName(_dbIndi);
        QString lString = _kernel.record()->getIndividualBurialPlaceName(_localIndi);

        if(dbString != lString && !dbString.isEmpty()){
            auto nameL = [](Kernel &k, Identifier ref, QString str){

                k.record()->setIndividualBurialPlaceName(ref, str);
            };
            
            SubboxMergeString *sub = new SubboxMergeString(_kernel, _mergeInd, tr("Place of death"), dbString, lString, nameL);

            _items.append(sub);
        }
    }

    // Date of marriage
    {
        DbRecordRole dbIndiRole = _kernel.record()->getDbRecordIndividualRole(_dbrecord, _dbIndi);
        if(dbIndiRole == DbRecordRole::BUR_MAIN || dbIndiRole == DbRecordRole::BUR_WIFE || dbIndiRole == DbRecordRole::BUR_HUSBAND){
            Identifier mergeFam = _kernel.record()->getMergeFamily();

            {

                Date dbDate1 = _kernel.record()->getDbRecordMarriageDate1(_dbrecord);
                Date dbDate2 = _kernel.record()->getDbRecordMarriageDate2(_dbrecord);
                DateType dbDateType = _kernel.record()->getDbRecordMarriageDateType(_dbrecord);

                Date lDate1 = _kernel.record()->getFamilyMarriageDate1(mergeFam);
                Date lDate2 = _kernel.record()->getFamilyMarriageDate2(mergeFam);
                DateType lDateType = _kernel.record()->getFamilyMarriageDateType(mergeFam);

                if(dbDateType != DateType::EMPT && _partner.isValid() && ( dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType))
                {
                    auto marrL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                        k.record()->setFamilyMarried(ref, true);
                        k.record()->setFamilyMarriageDate1(ref, d1);
                        k.record()->setFamilyMarriageDate2(ref, d2);
                        k.record()->setFamilyMarriageDateType(ref, dt);
                    };


                    SubboxMergeDate *sub = new SubboxMergeDate(_kernel, mergeFam , tr("Marriage date"), marrL);
                    sub->setDbDate(dbDate1, dbDate2, dbDateType);
                    sub->setLocalDate(lDate1, lDate2, lDateType);
                    sub->finish();
                    _items.append(sub);
                }
            }
        }
    }

    ///////////////////////////////////////////////////
    /// MARRIAGE
    ////////////////////////////////////////////////////
    
    
    
    
    DbRecordRole dbIndiRole = _kernel.record()->getDbRecordIndividualRole(_dbrecord, _dbIndi);
    if(dbIndiRole == DbRecordRole::MAR_GROOM || dbIndiRole == DbRecordRole::MAR_BRIDE){
        Identifier mergeFam = _kernel.record()->getMergeFamily();
        // Date of marriage
        {

            Date dbDate1 = _kernel.record()->getDbRecordMarriageDate1(_dbrecord);
            Date dbDate2 = _kernel.record()->getDbRecordMarriageDate2(_dbrecord);
            DateType dbDateType = _kernel.record()->getDbRecordMarriageDateType(_dbrecord);

            Date lDate1 = _kernel.record()->getFamilyMarriageDate1(mergeFam);
            Date lDate2 = _kernel.record()->getFamilyMarriageDate2(mergeFam);
            DateType lDateType = _kernel.record()->getFamilyMarriageDateType(mergeFam);

            if(dbDateType != DateType::EMPT && _partner.isValid() && ( dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType))
            {
                auto marrL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                    k.record()->setFamilyMarried(ref, true);
                    k.record()->setFamilyMarriageDate1(ref, d1);
                    k.record()->setFamilyMarriageDate2(ref, d2);
                    k.record()->setFamilyMarriageDateType(ref, dt);
                };


                SubboxMergeDate *sub = new SubboxMergeDate(_kernel, mergeFam , tr("Marriage date"), marrL);
                sub->setDbDate(dbDate1, dbDate2, dbDateType);
                sub->setLocalDate(lDate1, lDate2, lDateType);
                sub->finish();
                _items.append(sub);
            }
        }

        //Date of divorce
        {
            Date dbDate1 = _kernel.record()->getDbRecordDivorceDate1(_dbrecord);
            Date dbDate2 = _kernel.record()->getDbRecordDivorceDate2(_dbrecord);
            DateType dbDateType = _kernel.record()->getDbRecordDivorceDateType(_dbrecord);

            Date lDate1 = _kernel.record()->getFamilyDivorceDate1(mergeFam);
            Date lDate2 = _kernel.record()->getFamilyDivorceDate2(mergeFam);
            DateType lDateType = _kernel.record()->getFamilyDivorceDateType(mergeFam);

            if(dbDateType != DateType::EMPT && _partner.isValid() && (dbDate1 != lDate1 || dbDate2 != lDate2 || dbDateType != lDateType)){

                auto divL = [](Kernel& k, Identifier ref, Date d1, Date d2, DateType dt){
                    k.record()->setFamilyDivorced(ref, true);
                    k.record()->setFamilyDivorceDate1(ref, d1);
                    k.record()->setFamilyDivorceDate2(ref, d2);
                    k.record()->setFamilyDivorceDateType(ref, dt);
                };


                SubboxMergeDate *sub = new SubboxMergeDate(_kernel, mergeFam, tr("Divorce date"), divL);
                sub->setDbDate(dbDate1, dbDate2, dbDateType);
                sub->setLocalDate(lDate1, lDate2, lDateType);
                sub->finish();
                _items.append(sub);
            }
        }
    }




    /////////////////////////////////////////////////
    /// EVENTS
    //////////////////////////////////////////////////

#if 1

    // Residence
    if(_kernel.record()->getIndividualResiCount(_dbIndi) > 0)
    {
        SubboxMergeResidence *sub = new SubboxMergeResidence(_kernel, _dbIndi, _localIndi);
        _items.append(sub);
    }

    // Education
    if(_kernel.record()->getIndividualEducCount(_dbIndi) > 0)
    {
        SubboxMergeEducation *sub = new SubboxMergeEducation(_kernel, _dbIndi, _localIndi);
        _items.append(sub);
    }

    // Occupation
    if(_kernel.record()->getIndividualOccuCount(_dbIndi) > 0)
    {
        SubboxMergeOccupation *sub = new SubboxMergeOccupation(_kernel, _dbIndi, _localIndi);
        _items.append(sub);
    }

    // Religion
    if(_kernel.record()->getIndividualReliCount(_dbIndi) > 0)
    {
        SubboxMergeReligion *sub = new SubboxMergeReligion(_kernel, _dbIndi, _localIndi);
        _items.append(sub);
    }
#endif
    /// Add merge boxes

    for(auto item = _items.begin(); item != _items.end(); ++item)
    {
        _vLayout.addWidget(*item,0, Qt::AlignTop);
//        _vLayout.addWidget(*item, _vLayout.rowCount(),0, Qt::AlignTop);
    }

    if(_items.isEmpty()){
        _vLayout.addWidget(&_laEmpty, 0,Qt::AlignTop);
//        _vLayout.addWidget(&_laEmpty, 0, 0, Qt::AlignTop);
    }

    _vLayout.addItem(_spacer);

    _scrollArea.setWidget(_scrollAreaWidget);

    _pbMerge.setEnabled(true);
    _pbReset.setEnabled(true);
}

void MergeBox::fill(){

    _laEmpty.setText(tr("There is no data to be merged"));
    _laEmpty.setStyleSheet("font-weight: bold;");

    _pbReset.setText(tr("Reset"));
    _pbReset.setToolTip(tr("Reset merging process"));
    _pbReset.setEnabled(false);
    _pbMerge.setText(tr("Merge"));
    _pbMerge.setToolTip(tr("Accept merge"));
    _pbMerge.setEnabled(false);



    _scrollAreaWidget = new QWidget;

    _scrollAreaWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    _scrollArea.setWidgetResizable(true);

    _scrollAreaWidget->setLayout(&_vLayout);
    _layout.addWidget(&_scrollArea,0,0,1,2);
    _layout.addWidget(&_pbReset, 1,0, Qt::AlignBottom);
    _layout.addWidget(&_pbMerge, 1, 1, Qt::AlignBottom);

//    _scrollArea.setSizePolicy(QSizePolicy::Policy::Preferred,QSizePolicy::Policy::Expanding);
//    _scrollArea.setFixedHeight(600);

    _scrollAreaWidget->setContentsMargins(10,0,0,10);
//    _vLayout.setSpacing(100);
    _layout.setSpacing(10);

    _pbMerge.setFixedHeight(30);
    _pbReset.setFixedHeight(30);

}

void MergeBox::remove()
{
    for(auto item = _items.begin(); item != _items.end(); ++item)
    {
        _vLayout.removeWidget(*item);
        delete (*item);
    }
    _items.clear();

    _vLayout.removeItem(_spacer);
    _vLayout.removeWidget(&_laEmpty);
    _laEmpty.setParent(nullptr);

    _pbMerge.setEnabled(false);
    _pbReset.setEnabled(false);
}

void MergeBox::merge()
{
    _kernel.record()->applyMergeIndividual(_localIndi);
    Identifier fam = _kernel.record()->applyMergeFamily(_localIndi, _partner);
    DbRecordType type = _kernel.record()->getDbRecordType(_dbrecord);
    DbRecordRole role = _kernel.record()->getDbRecordIndividualRole(_dbrecord, _dbIndi);

    Identifier sourceRef = _kernel.record()->addDbSourceToMemory(_kernel.record()->getDbRecordSourceRef(_dbrecord));
    if(type == DbRecordType::RT_BIRTH && role == DbRecordRole::BIR_MAIN){
        _kernel.record()->setIndividualBirthSourceCitationRef(_localIndi, sourceRef);
    } else if(type == DbRecordType::RT_BURIAL && role == DbRecordRole::BUR_MAIN){
        _kernel.record()->setIndividualDeathSourceCitationRef(_localIndi, sourceRef);
    } else if((type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_BRIDE || type == DbRecordType::RT_MARRIAGE_GROOM) &&
               (role == DbRecordRole::MAR_BRIDE || role == DbRecordRole::MAR_GROOM)){
        _kernel.record()->setFamilyMarriageSourceCitationRef(fam, sourceRef);
    }
    this->update();
    _kernel.record()->setDbRecordIndividualImported(_dbrecord, _dbIndi, true);
    emit this->individualMerged(_localIndi);
}




void MergeBox::dbIndividualChanged(Identifier indi, Identifier record)
{
    this->_dbIndi = indi;
    this->_dbrecord = record;
    this->update();
}

void MergeBox::localIndividualChanged(Identifier indi, Identifier partner)
{
    _kernel.record()->copyToMergeIndividual(indi, partner);
    this->_localIndi = indi;
    this->_partner = partner;
    this->update();
}

void MergeBox::resetClicked(bool checked)
{
    Q_UNUSED(checked)
    _kernel.record()->copyToMergeIndividual(_localIndi, _partner);
    this->update();
}

void MergeBox::mergeClicked(bool checked)
{
    Q_UNUSED(checked)
    this->merge();
}
