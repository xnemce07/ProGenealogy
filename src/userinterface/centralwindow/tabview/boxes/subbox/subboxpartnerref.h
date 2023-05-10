/*!
 *  \file   subboxpartnerref.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXPARTNERREF_H
#define SUBBOXPARTNERREF_H

#include "subbox.h"
#include "userinterface/objects/elidebutton.h"
#include "../../../../../textformatter.h"
#include "../../../../../kernel.h"
#include "../../../../dialogs/data/dialogassignperson.h"

class SubboxPartnerRef : public Subbox
{
    Q_OBJECT
private:
    Kernel &_kernel;

    Date _date;
    Identifier _ref;
    Identifier _main;

    Label _laPartner;
    ElideButton _buPerson;
    NarrowButton _buRemove;
    QPushButton _buSelAll;
    QPushButton _buSelPart;

    DialogAssignPerson *_dialogAddPartner;



public:
    explicit
    SubboxPartnerRef(Kernel &k, QWidget* parent = nullptr);

    void update();
    void setDisabled(bool disabled);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;
signals:
    void addClicked();
    void removeClicked();
    void partnerSelected(Identifier ref);
public slots:
    void mainChanged(Identifier ref);
private slots:
    void clickSelAll(bool checked);
    void clickSelPart(bool checked);
    void clickRemove(bool checked);

};

#endif // SUBBOXPARTNERREF_H
