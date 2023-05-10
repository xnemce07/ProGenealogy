/*!
 *  \file   tst_testkernel.cpp
 *  \author Eva Navratilova
 */

#include "tst_testkernel.h"
QTEST_MAIN(TestKernel);


TestKernel::TestKernel()
{

}

TestKernel::~TestKernel()
{

}

void TestKernel::testImport()
{
    QString file1 = "../test/testdata/SampleAllData.ged";
    bool saveable, successful;
    Identifier first, last;
    int cycleCount = 0;

    successful = _kernel.importFile(file1, saveable);

    // Import ok
    QCOMPARE(successful, true);     // Should be imported successfuly
    QCOMPARE(saveable, true);       // Should be saveable - data was meant for this program

    // INDIVIDUAL count ok
    first = _kernel.record()->getIdentifierIndividualFirst();
    last = _kernel.record()->getIdentifierIndividualLast();
    QCOMPARE(first.getNumber(), static_cast<unsigned long long>(22));    // First identifier number
    QCOMPARE(last.getNumber(), static_cast<unsigned long long>(78 + 1)); // Last identifier number + 1
    for(Identifier it=first; it != last; _kernel.record()->getIdentifierIndividualNext(it))
    {
        cycleCount++;
    }
    QCOMPARE(cycleCount, 14);    // Total of 6 people in file1

    // FAMILY count ok
    cycleCount = 0;
    first = _kernel.record()->getIdentifierFamilyFirst();
    last = _kernel.record()->getIdentifierFamilyLast();
    QCOMPARE(static_cast<signed>(first.getNumber()), 16);        // First identifier number
    QCOMPARE(static_cast<signed>(last.getNumber()), 17 + 1);     // Lat identifier number + 1
    for(Identifier it=first; it != last; _kernel.record()->getIdentifierFamilyNext(it))
    {
        cycleCount++;
    }
    QCOMPARE(cycleCount, 2);    // Two families

    // SUBMITTER count ok
    cycleCount = 0;
    first = _kernel.record()->getIdentifierSubmitterFirst();
    last = _kernel.record()->getIdentifierSubmitterLast();
    QCOMPARE(static_cast<signed>(first.getNumber()), 0);        // First identifier number
    QCOMPARE(static_cast<signed>(last.getNumber()), 0 + 1);     // Lat identifier number + 1
    for(Identifier it=first; it != last; _kernel.record()->getIdentifierSubmitterNext(it))
    {
        cycleCount++;
    }
    QCOMPARE(cycleCount, 1);    // Exactly one submitter

    // SOURCE RECORD count ok
    cycleCount = 0;
    first = _kernel.record()->getIdentifierSourceFirst();
    last = _kernel.record()->getIdentifierSourceLast();
    QCOMPARE(static_cast<signed>(first.getNumber()), 0);        // First identifier number
    QCOMPARE(static_cast<signed>(last.getNumber()), 2 + 1);     // Lat identifier number + 1
    for(Identifier it=first; it != last; _kernel.record()->getIdentifierSourceNext(it))
    {
        cycleCount++;
    }
    QCOMPARE(cycleCount, 3);    // Three sources
}

void TestKernel::testSourceRecordDataImported()
{
    auto birtSrc = _kernel.record()->getSourceRefs(CommonEventType::BIRT);
    auto deatSrc = _kernel.record()->getSourceRefs(CommonEventType::DEAT);
    auto marrSrc = _kernel.record()->getSourceRefs(CommonEventType::MARR);

    // Should find single appearance of each
    QCOMPARE(birtSrc.length(), 1);
    QCOMPARE(deatSrc.length(), 1);
    QCOMPARE(marrSrc.length(), 1);

    birtSrc = _kernel.record()->getSourceRefs(CommonEventType::BIRT, Date(1844, 5, 2));
    // Should find Florian's Birth Source
    QCOMPARE(birtSrc.length(), 1);

    birtSrc = _kernel.record()->getSourceRefs(CommonEventType::BIRT, Date(1844, 5, 2), "Čelčice");
    // Should find Florian's Birth Source
    QCOMPARE(birtSrc.length(), 1);

}

void TestKernel::testIndividualDataImported()
{
    // Florian identifier init
    Identifier wife = _kernel.record()->getIdentifierIndividualFirst();
    auto husbands = _kernel.record()->getIndividualPartners(wife);

    QCOMPARE(husbands.length(), 1); // Should have exactly one husband
    if(! husbands.isEmpty())
    {
        _florian = _kernel.record()->getIndividualFather(husbands.first());
    }

    Identifier birtSrc = _kernel.record()->getIndividualBirthSourceRef(_florian);

    QCOMPARE(_kernel.record()->isSource(birtSrc), true);
    QCOMPARE(_kernel.record()->isSource(birtSrc), true);

    Identifier priest = _kernel.record()->getIndividualChristeningPriest(_florian);
    QCOMPARE(_kernel.record()->isIndividual(priest), true);

}

void TestKernel::testIndividualOccupation()
{
    unsigned int count = _kernel.record()->getIndividualOccuCount(_florian);
    QCOMPARE(count, static_cast<unsigned>(2));
}

void TestKernel::testDate()
{
    Date date(1850, 1, 1);
    QCOMPARE(date.year(), 1850);
    QCOMPARE(date.datePrecision(), DatePrecision::FULL);
    date = Date(1851);
    QCOMPARE(date.year(), 1851);
    QCOMPARE(date.datePrecision(), DatePrecision::YEAR);
}

void TestKernel::testDbRecordCreate()
{
    Identifier rec = _kernel.record()->newDbRecord(DbRecordType::RT_BIRTH,0);
    Identifier src = _kernel.record()->getDbRecordSourceRef(rec);

    QCOMPARE(_kernel.record()->isDbRecord(rec), true);
    QCOMPARE(_kernel.record()->isDbSource(src), true);

    // record should be not imported at start
    QCOMPARE(_kernel.record()->getDbRecordImported(rec), false);

    // Test event date setter
    _kernel.record()->setDbRecordDivorceDate1(rec, Date(1850, 1, 1));
    _kernel.record()->setDbRecordDivorceDate2(rec, Date(1950, 1, 1));
    _kernel.record()->setDbRecordDivorceDateType(rec, DateType::BETWEEN);

    QCOMPARE(_kernel.record()->getDbRecordDivorceDate1(rec).year(), 1850);
    QCOMPARE(_kernel.record()->getDbRecordDivorceDate2(rec).year(), 1950);
    QCOMPARE(_kernel.record()->getDbRecordDivorceDateType(rec), DateType::BETWEEN);

    // mark imported
    _kernel.record()->setDbRecordImported(rec, true);
    QCOMPARE(_kernel.record()->getDbRecordImported(rec), true);

    // Set archive name
    _kernel.record()->setSourceArchive(src, "Test archive");
    QCOMPARE(_kernel.record()->getSourceArchive(src), "Test archive");
}

void TestKernel::testDbRecordAddIndis()
{
    Identifier rec = _kernel.record()->getIdentifierDbRecordFirst();

    // DbRecord should be empty
    QCOMPARE(_kernel.record()->getDbRecordIndividuals(rec).count(), 0);
    
    Identifier main = _kernel.record()->newDbIndividual(rec,DbRecordRole::BIR_MAIN,1);
    Identifier f = _kernel.record()->newDbIndividual(rec,DbRecordRole::BIR_F,11);

    QCOMPARE(_kernel.record()->getDbRecordIndividuals(rec).count(), 2);

    QCOMPARE(_kernel.record()->isDbIndividual(main), true);
    QCOMPARE(_kernel.record()->isDbIndividual(f), true);

    // Get individual by role
    QCOMPARE(_kernel.record()->getDbRecordIndividualByRole(rec, DbRecordRole::BIR_MAIN), main);
    QCOMPARE(_kernel.record()->getDbRecordIndividualByRole(rec, DbRecordRole::BIR_F), f);
}

void TestKernel::testDbIndividualAdd()
{
    Identifier rec = _kernel.record()->getIdentifierDbRecordFirst();
    Identifier dbIndi = _kernel.record()->getDbRecordIndividualByRole(rec, DbRecordRole::BIR_MAIN);


    // Add data to db individual
    _kernel.record()->setIndividualBirthDate1(dbIndi, Date(1850, 1, 1));
    _kernel.record()->setIndividualNameGiven(dbIndi, "Frederik");
    _kernel.record()->setIndividualNote(dbIndi, "Test note");
    _kernel.record()->setIndividualDeathCause(dbIndi, "Test death");
    _kernel.record()->setIndividualDeceased(dbIndi, true);
    unsigned int index = _kernel.record()->setIndividualEduc(dbIndi);
    _kernel.record()->setIndividualEducDescription(dbIndi, index, "Test educ");


    // Add indi to database
    Identifier localIndi = _kernel.record()->addDbRecordIndividualToMemory(dbIndi);
    Identifier first, last;
    first = _kernel.record()->getIdentifierIndividualFirst();
    last = _kernel.record()->getIdentifierIndividualLast();
    bool found = false;
    for(Identifier curr = first; curr != last; curr = _kernel.record()->getIdentifierDbIndividualNext(curr)){
        if(curr == localIndi){
            found = true;
            break;
        }
    }
    QCOMPARE(found, true);
    QCOMPARE(_kernel.record()->isIndividual(localIndi), true);
    QCOMPARE(_kernel.record()->getIndividualNameGiven(localIndi), "Frederik");
    QCOMPARE(_kernel.record()->getIndividualNote(localIndi), "Test note");
    QCOMPARE(_kernel.record()->getIndividualDeathCause(localIndi), "Test death");
    QCOMPARE(_kernel.record()->isIndividualDeceased(localIndi), true);
    QCOMPARE(_kernel.record()->getIndividualEducCount(localIndi), 1);
    QCOMPARE(_kernel.record()->getIndividualEducDescription(dbIndi, index), "Test educ");
}

void TestKernel::testDbSourceAdd()
{
    Identifier rec = _kernel.record()->getIdentifierDbRecordFirst();
    Identifier dbSrc = _kernel.record()->getDbRecordSourceRef(rec);


    _kernel.record()->setSourceArchive(dbSrc, "Test archive");
    unsigned int indexB = _kernel.record()->setSourceData(dbSrc, CommonEventType::BIRT);
    _kernel.record()->setSourceDataPlac(dbSrc, CommonEventType::BIRT, indexB, "Test birth plac");

    _kernel.record()->setSourceData(dbSrc, CommonEventType::MARR);
    _kernel.record()->setSourceData(dbSrc, CommonEventType::MARR);

    Identifier src = _kernel.record()->addDbSourceToMemory(dbSrc);

    QCOMPARE(_kernel.record()->isSource(src), true);

    QCOMPARE(_kernel.record()->getSourceArchive(src), "Test archive");
    QCOMPARE(_kernel.record()->getSourceDataCount(src, CommonEventType::BIRT), 1);
    QCOMPARE(_kernel.record()->getSourceDataCount(src, CommonEventType::MARR), 2);
    QCOMPARE(_kernel.record()->getSourceDataPlac(src, CommonEventType::BIRT, 0), "Test birth plac");
}

void TestKernel::testMergeIndividualApply()
{
    Identifier main = _kernel.record()->getIdentifierIndividualFirst();
    Identifier part = _kernel.record()->getIdentifierIndividualNext(main);

    // Set data of person
    _kernel.record()->setIndividualNameGiven(main, "Peter");
    _kernel.record()->setIndividualNameSurname(main, "Petrovic");
    _kernel.record()->setIndividualBirthDate1(main, Date(1400));

    _kernel.record()->copyToMergeIndividual(main, part);

    Identifier mergeIndi = _kernel.record()->getMergeIndividual();
    Identifier mergeFam = _kernel.record()->getMergeFamily();

    // Change and add some of the data in merge individual
    _kernel.record()->setIndividualNameMarname(mergeIndi, "Marek");
    _kernel.record()->setIndividualDeathDate1(mergeIndi, Date(1450));

    // Add marriage date and place to mergefam
    _kernel.record()->setFamilyMarriageDate1(mergeFam, Date(1440));
    _kernel.record()->setFamilyMarriagePlaceName(mergeFam, "Test marr place");

    // Apply changes
    _kernel.record()->applyMergeIndividual(main);
    _kernel.record()->applyMergeFamily(main, part);

    // Untouched data should staty the same
    QCOMPARE(_kernel.record()->getIndividualNameGiven(main), "Peter");
    QCOMPARE(_kernel.record()->getIndividualBirthDate1(main).year(), 1400);

    // Chagned or added data
    QCOMPARE(_kernel.record()->getIndividualNameSurnameMarriedBirth(main), "Marek");
    QCOMPARE(_kernel.record()->getIndividualDeathDate1(main).year(), 1450);


    // Family should get created in apply
    Identifier newFam = _kernel.record()->getFamilyOfCouple(main, part);
    QCOMPARE(_kernel.record()->isFamily(newFam), true);
    QCOMPARE(_kernel.record()->getFamilyMarriageDate1(mergeFam).year(), 1440);
    QCOMPARE(_kernel.record()->getFamilyMarriagePlaceName(mergeFam) , "Test marr place");

}

void TestKernel::testMergeFamilyApply()
{

}

