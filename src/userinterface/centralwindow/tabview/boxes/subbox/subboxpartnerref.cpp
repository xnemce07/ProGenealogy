/*!
 *  \file   subboxpartnerref.cpp
 *  \author Leopold Nemcek
 */
#include "subboxpartnerref.h"

SubboxPartnerRef::SubboxPartnerRef(Kernel &k, QWidget* parent)
    : Subbox(parent), _kernel(k)
{
    this->fill();
    connect(&_buSelAll, &QPushButton::clicked, this, &SubboxPartnerRef::clickSelAll);
    connect(&_buSelPart, &QPushButton::clicked, this, &SubboxPartnerRef::clickSelPart);
    connect(&_buPerson, &QPushButton::clicked, this, &SubboxPartnerRef::clickSelAll);
    connect(&_buRemove, &QPushButton::clicked, this, &SubboxPartnerRef::clickRemove);
}

void SubboxPartnerRef::update()
{
    QString text;
    QIcon icon;

    bool valid = _kernel.record()->isIndividual(_ref);
    if(valid){
        text = TextFormatter::getPersonLineText(_kernel, _ref);
    } else {
        icon = QIcon(":icons/add");
    }
    _buPerson.setText(text);
    _buPerson.setIcon(icon);
    _buRemove.setDisabled(!valid);
}



void SubboxPartnerRef::setDisabled(bool disabled)
{
    _buPerson.setDisabled(disabled);
    _buSelAll.setDisabled(disabled);
    _buRemove.setDisabled(disabled);
    _buSelPart.setDisabled(disabled);
    _laPartner.setDisabled(disabled);
}

void SubboxPartnerRef::fill()
{
    _laPartner.setText(tr("Select partner"));
    _laPartner.setStyleSheet("font-weight: bold;");
    _buPerson.setCursor(Qt::CursorShape::PointingHandCursor);
    _buRemove.setIcon(QIcon(":icons/delete"));
    _buRemove.setToolTip(tr("Deselect partner"));

    _buSelAll.setText(tr("Select from all"));
    _buSelAll.setToolTip(tr("Select partner from all individuals"));
    _buSelPart.setText(tr("Select from partners"));
    _buSelPart.setToolTip(tr("Select partner from individual's existing partners"));

    // Add widgets
//    _layout.addWidget(&_laPartner,0,0);
    _layout.addWidget(&_buPerson,1,0,1,2);
    _layout.addWidget(&_buRemove,1,2);
    _layout.addWidget(&_buSelAll, 2, 0);
    _layout.addWidget(&_buSelPart, 2,1);

    // Adjust widgets
    _layout.setColumnStretch(0, 2);
    _layout.setColumnStretch(1, 2);
    _layout.setColumnStretch(2, 1);

}

int SubboxPartnerRef::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laPartner.text());
    return width;
}

void SubboxPartnerRef::setLabelsWidth(int width)
{
    _laPartner.setFixedWidth(width);
}

void SubboxPartnerRef::clickSelAll(bool checked)
{
     Q_UNUSED(checked);
    Identifier partner;
    QVector<Identifier> indis;
    Identifier first, last;
    first = _kernel.record()->getIdentifierIndividualFirst();
    last = _kernel.record()->getIdentifierIndividualLast();
    for(Identifier current = first; current < last; _kernel.record()->getIdentifierIndividualNext(current))
    {
        if(current != _main){
            indis.append(current);
        }

    }
    _dialogAddPartner = new DialogAssignPerson(_kernel, indis);
    int result = _dialogAddPartner->getSelectedId(partner);
    if(_kernel.record()->isIndividual(partner)){
        _ref = partner;
        this->update();
        emit(this->partnerSelected(_ref));
    }
}

void SubboxPartnerRef::clickSelPart(bool checked)
{
    Q_UNUSED(checked);
   Identifier partner;
   QVector<Identifier> indis = _kernel.record()->getIndividualPartners(_main);

   _dialogAddPartner = new DialogAssignPerson(_kernel, indis);
   int result = _dialogAddPartner->getSelectedId(partner);
   if(_kernel.record()->isIndividual(partner)){
       _ref = partner;
       this->update();
       emit(this->partnerSelected(_ref));
   }
}

void SubboxPartnerRef::clickRemove(bool checked)
{
    Q_UNUSED(checked);
    _ref = Identifier();
    emit(this->partnerSelected(_ref));
    update();
}

void SubboxPartnerRef::mainChanged(Identifier ref)
{
    _main = ref;
    _ref = Identifier();
    update();
}




