/*!
 *  \file   tabsync.h
 *  \author Leopold Nemcek
 */
#ifndef TABSYNC_H
#define TABSYNC_H

#include <QWidget>
#include <QGridLayout>

#include "../../../kernel.h"
#include "tabsync/localrecordsbox.h"
#include "tabsync/mergebox.h"
#include "tabsync/dbrecordsbox.h"


class TabSync : public QWidget, public Sizer
{
    Q_OBJECT
private:
    Kernel &_kernel;
    Identifier &_proband;

    QGridLayout _layout;


    ///Boxes
    DbRecordsBox *_dbRecordsBox;
    MergeBox *_mergeBox;
    LocalRecordsBox *_localRecordsBox;


public:
    explicit TabSync(Kernel &k, Identifier &p, QWidget *parent = nullptr);
    ~TabSync() override;
    void update();
    void updateSync();


private:
    void fill();

signals:
    void dataChanged(Identifier ref);
public slots:
    static void defaultSettings();
    void saveSettings() const;
};

#endif // TABSYNC_H
