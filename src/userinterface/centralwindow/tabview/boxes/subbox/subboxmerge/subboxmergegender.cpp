/*!
 *  \file   subboxmergegender.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergegender.h"

SubboxMergeGender::SubboxMergeGender(Kernel &k, QString title, Gender dbValue, Gender localValue, QWidget *parent) :
    SubboxMerge (parent), _kernel(k), _title(title), _dbVal(dbValue), _localVal(localValue)
{
    _mergeInd = _kernel.record()->getMergeIndividual();
    this->fill();

    connect(&_pbDbVal, &QPushButton::clicked, this, &SubboxMergeGender::dbButtonPressed);
    connect(&_pbLocVal, &QPushButton::clicked, this, &SubboxMergeGender::localButtonPressed);
//    connect(&_comboGender, &QComboBox::currentIndexChanged, this, &SubboxMergeGender::genderChanged);
    connect(&_comboGender, &QComboBox::currentTextChanged, this, &SubboxMergeGender::genderChanged);
}

int SubboxMergeGender::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeGender::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}

void SubboxMergeGender::fill()
{
    _laTitle.setText(_title);
    _laTitle.setStyleSheet("font-weight: bold");
    _pbDbVal.setText(gend2Str(_dbVal));
    _pbLocVal.setText(gend2Str(_localVal));
    _comboGender.addItems(QStringList() << tr("Male") << tr("Female") << tr("Undetermined"));
    setGenderIndex(_localVal);

    _pbDbVal.setFixedHeight(30);
    _pbLocVal.setFixedHeight(30);

    _pbDbVal.setStyleSheet(_styleSheetNoMatch);
    _pbLocVal.setStyleSheet(_styleSheetMatch);

    _layout.addWidget(&_laTitle,0,0);
    _layout.addWidget(&_pbDbVal, 1 ,0);
    _layout.addWidget(&_pbLocVal,1,1);
    _layout.addWidget(&_comboGender,1,2);

    _pbDbVal.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    _pbLocVal.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    _comboGender.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);

}

QString SubboxMergeGender::gend2Str(Gender gender)
{
    if(gender == Gender::MALE){
        return tr("Male");
    }
    if(gender == Gender::FEMALE){
        return tr("Female");
    }
    return tr("Undetermined");
}

void SubboxMergeGender::dbButtonPressed(bool checked)
{
    Q_UNUSED(checked)
    setGenderIndex(_dbVal);
    updateButtons();
}

void SubboxMergeGender::localButtonPressed(bool checked)
{
    Q_UNUSED(checked)
    setGenderIndex(_localVal);
    updateButtons();
}

void SubboxMergeGender::genderChanged(QString text)
{
    int index = _comboGender.currentIndex();
    Gender gender;
    if(index == 0){
        gender = Gender::MALE;
    } else if(index == 1){
        gender = Gender::FEMALE;
    } else {
        gender = Gender::UNDET;
    }
    updateButtons();
    _kernel.record()->setIndividualGender(_mergeInd, gender);
}

void SubboxMergeGender::updateButtons(){
    int index = _comboGender.currentIndex();
    Gender gender;
    if(index == 0){
        gender = Gender::MALE;
    } else if(index == 1){
        gender = Gender::FEMALE;
    } else {
        gender = Gender::UNDET;
    }

    if(gender == _dbVal){
        _pbDbVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbDbVal.setStyleSheet(_styleSheetNoMatch);
    }

    if(gender == _localVal){
        _pbLocVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbLocVal.setStyleSheet(_styleSheetNoMatch);
    }
}

void SubboxMergeGender::setGenderIndex(Gender g)
{
    _comboGender.setCurrentIndex(toUnderlying(g));
}
