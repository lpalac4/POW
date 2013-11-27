#include "playerindicatorview.h"

PlayerIndicatorView::PlayerIndicatorView(QWidget* parent): QGraphicsView(parent){
    setAttribute(Qt::WA_AcceptTouchEvents);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

bool PlayerIndicatorView::event(QEvent *event){
    if(event->type() == QEvent::TouchBegin){
        emit indicatorTouched();
        return true;
    }

    return QGraphicsView::event(event);
}
