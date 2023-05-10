/*!
 *  \file   subboxmergestring.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergestring.h"



SubboxMergeString::SubboxMergeString(Kernel &k, Identifier mergeIndi, QString title, QString dbValue, QString localValue, void (*writeF)(Kernel&, Identifier, QString), QWidget *parent):
    SubboxMerge (parent),_kernel(k), _mergeInd(mergeIndi), _title(title), _dbVal(dbValue), _localVal(localValue), _writeF(writeF)
{
    this->fill();
    
    connect(&_leResult, &QLineEdit::textChanged, this, &SubboxMergeString::lineEditChanged);
    connect(&_pbDbVal, &QPushButton::clicked, this, &SubboxMergeString::dbButtonPressed);
    connect(&_pbLocVal, &QPushButton::clicked, this, &SubboxMergeString::localButtonPressed);
}

void SubboxMergeString::fill()
{
    _laTitle.setText(_title);
    _laTitle.setStyleSheet("font-weight: bold");
    _pbDbVal.setText(_dbVal);
    _pbLocVal.setText(_localVal);
    _leResult.setText(_localVal);

    _pbDbVal.setFixedHeight(30);
    _pbLocVal.setFixedHeight(30);

    _layout.addWidget(&_laTitle,0,0);
    _layout.addWidget(&_pbDbVal,1,0);
    _layout.addWidget(&_pbLocVal,1,1);
    _layout.addWidget(&_leResult, 1,2);
    updateButtons();



    _pbDbVal.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    _pbLocVal.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    _leResult.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);



    //    this->setStyleSheet("border: 1px solid blue");
}

void SubboxMergeString::updateButtons()
{
    if(_dbVal == _leResult.text()){
        _pbDbVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbDbVal.setStyleSheet(_styleSheetNoMatch);
    }

    if(_localVal == _leResult.text()){
        _pbLocVal.setStyleSheet(_styleSheetMatch);
    } else {
        _pbLocVal.setStyleSheet(_styleSheetNoMatch);
    }
}

void SubboxMergeString::dbButtonPressed(bool checked)
{
    Q_UNUSED(checked)
    _leResult.setText(_dbVal);
}

void SubboxMergeString::localButtonPressed(bool checked)
{
    Q_UNUSED(checked)
    _leResult.setText(_localVal);
}

void SubboxMergeString::lineEditChanged(QString text)
{
    _writeF(_kernel, _mergeInd, text);
    updateButtons();
}



int SubboxMergeString::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeString::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}
