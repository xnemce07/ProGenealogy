/*!
 *  \file   subboxmergedate.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergedate.h"



SubboxMergeDate::SubboxMergeDate(Kernel &k, Identifier ref, QString title, void (*writeF)(Kernel &, Identifier, Date, Date, DateType), QWidget *parent):
    SubboxMerge (parent), _kernel(k), _ref(ref), _title(title), _writeF(writeF)
{
    _subDate = new SubboxDate("",DateTypeAvailability::NoPeriod);
    connect(_subDate, &SubboxDate::date1Changed, this, &SubboxMergeDate::date1Changed);
    connect(_subDate, &SubboxDate::date2Changed, this, &SubboxMergeDate::date2Changed);
    connect(_subDate, &SubboxDate::dateTypeChanged, this, &SubboxMergeDate::dateTypeChanged);
    connect(&_pbDbVal, &QPushButton::clicked, this, &SubboxMergeDate::dbButtonPressed);
    connect(&_pbLocVal, &QPushButton::clicked, this, &SubboxMergeDate::localButtonPressed);
}

Identifier SubboxMergeDate::mergeInd() const
{
    return _ref;
}

void SubboxMergeDate::setMergeInd(const Identifier &mergeInd)
{
    _ref = mergeInd;
}

void SubboxMergeDate::setDbDate(Date date1, Date date2, DateType type)
{
    this->_dbDate1 = date1;
    this->_dbDate2 = date2;
    this->_dbDateType = type;
}

void SubboxMergeDate::setLocalDate(Date date1, Date date2, DateType type)
{
    this->_lDate1 = date1;
    this->_lDate2 = date2;
    this->_lDateType = type;
}

void SubboxMergeDate::finish()
{
    this->fill();
}

int SubboxMergeDate::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeDate::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}

void SubboxMergeDate::fill()
{
    _laTitle.setText(_title);
    _laTitle.setStyleSheet("font-weight: bold");
    _pbDbVal.setText(TextFormatter::dateStructureToText(_dbDateType,_dbDate1, _dbDate2));
    _pbLocVal.setText(TextFormatter::dateStructureToText(_lDateType,_lDate1, _lDate2));
    _pbDbVal.setFixedHeight(30);
    _pbLocVal.setFixedHeight(30);

//    _subDate->changeDate1(_lDate1);
//    _subDate->changeDate2(_lDate2);
//    _subDate->changeDateType(static_cast<int>(_lDateType));
    _subDate->update(_lDateType, _lDate1, _lDate2);
    _finalDate1 = _lDate1;
    _finalDate2 = _lDate2;
    _finalDateType = _lDateType;



    _layout.addWidget(&_laTitle,0,0);
    _layout.addWidget(&_pbDbVal,1,0);
    _layout.addWidget(&_pbLocVal,1,1);
    _layout.addWidget(_subDate,1,2);

    _layout.setColumnStretch(0,1);
    _layout.setColumnStretch(1,1);
    _layout.setColumnStretch(2,1);

    updateButtons();
//    _layout.setRowStretch(1,2);

//    _pbDbVal.setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
//    _pbLocVal.setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
//    _subDate->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);

}

void SubboxMergeDate::dateChanged()
{
    updateButtons();
    _writeF(_kernel, _ref, _finalDate1, _finalDate2, _finalDateType);
}

void SubboxMergeDate::updateButtons()
{
    if(_dbDate1 == _finalDate1 && _dbDate2 == _finalDate2 && _dbDateType == _finalDateType){
        _pbDbVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbDbVal.setStyleSheet(_styleSheetNoMatch);
    }

    if((_lDate1 == _finalDate1 && _lDate2 == _finalDate2 && _lDateType == _finalDateType) || (_lDateType == DateType::EMPT && _finalDateType == DateType::EMPT)){
        _pbLocVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbLocVal.setStyleSheet(_styleSheetNoMatch);
    }
}

void SubboxMergeDate::dbButtonPressed(bool checked)
{
    Q_UNUSED(checked);
    _subDate->update(_dbDateType, _dbDate1, _dbDate2);
    _finalDate1 = _dbDate1;
    _finalDate2 = _dbDate2;
    _finalDateType = _dbDateType;
    dateChanged();
}

void SubboxMergeDate::localButtonPressed(bool checked)
{
    Q_UNUSED(checked);
    _subDate->update(_lDateType, _lDate1, _lDate2);
    _finalDate1 = _lDate1;
    _finalDate2 = _lDate2;
    _finalDateType = _lDateType;
    dateChanged();
}

void SubboxMergeDate::date1Changed(Date date)
{
    _finalDate1 = date;
    dateChanged();
}

void SubboxMergeDate::date2Changed(Date date)
{
    _finalDate2 = date;
    dateChanged();
}

void SubboxMergeDate::dateTypeChanged(DateType type)
{
    _finalDateType = type;
    dateChanged();
}

