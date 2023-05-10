/*!
 *  \file   boxoccupation.h
 *  \author Eva Navratilova
 */

#ifndef BOXOCCUPATION_H
#define BOXOCCUPATION_H

#include "boxeditablelist.h"
#include "userinterface/dialogs/data/dialogeditabledescription.h"

class BoxOccupation : public BoxEditableList
{
    Q_OBJECT
private:

public:
    BoxOccupation(Kernel &k, Identifier &p, QWidget *parent = nullptr);

    void update() override;

private:
    void swapIndexes(unsigned int index1, unsigned int index2) override;

private slots:
    void clickAdd() override;
    void clickEdit(unsigned int index) override;
    void clickDelete(unsigned int index) override;
    void saveData(const unsigned int index, QString &description, DateType &datetype, Date &date1, Date &date2);

};

#endif // BOXOCCUPATION_H
