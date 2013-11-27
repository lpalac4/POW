#ifndef PLAYERINDICATORVIEW_H
#define PLAYERINDICATORVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>


class PlayerIndicatorView : public QGraphicsView
{
    Q_OBJECT
public:
    PlayerIndicatorView(QWidget* parent);
    bool event(QEvent *event);

protected:
signals:
    void indicatorTouched();
};

#endif // PLAYERINDICATORVIEW_H
