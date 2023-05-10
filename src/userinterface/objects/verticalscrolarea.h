#ifndef VERTICALSCROLAREA_H
#define VERTICALSCROLAREA_H

#include <QScrollArea>
#include <QScrollBar>
#include <QEvent>

class VerticalScrollArea : public QScrollArea
{
Q_OBJECT
public:
explicit VerticalScrollArea(QWidget *parent = 0);

virtual bool eventFilter(QObject *o, QEvent *e);

};
#endif // VERTICALSCROLAREA_H
