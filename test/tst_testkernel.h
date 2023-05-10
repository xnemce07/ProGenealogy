/*!
 *  \file   tst_testkernel.h
 *  \author Eva Navratilova, Leopold Nemcek
 */

#ifndef TST_TESTKERNEL_H
#define TST_TESTKERNEL_H
#pragma once

#include <QtTest>
#include <QString>

#include "../src/kernel.h"
#include "../src/kernel/record/recordRAM/date.h"
// add necessary includes here

class TestKernel : public QObject
{
    Q_OBJECT
private:
    Kernel _kernel;
    Identifier _florian;

public:
    TestKernel();
    ~TestKernel();

private slots:
    void testImport();
    void testSourceRecordDataImported();
    void testIndividualDataImported();
    void testIndividualOccupation();
    void testDate();

    // Testing of temporary records
    void testDbRecordCreate();
    void testDbRecordAddIndis();
    void testDbIndividualAdd();
    void testDbSourceAdd();
    void testMergeIndividualApply();
    void testMergeFamilyApply();

};

#endif // TST_TESTKERNEL_H
