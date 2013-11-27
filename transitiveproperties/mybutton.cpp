#include "mybutton.h"
#include <QDebug>

MyButton::MyButton(QWidget* parent) :
    QPushButton(parent){
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setDimensions(parent->width(),parent->height());

}


void MyButton::setDimensions(int w, int h){
    double ratio = w/h;
    double height = h * 0.175;
    double width = height * ratio;

    setMaximumSize(height, width);
    setMinimumSize(height, width);
    setFixedSize(height, width);


}

void MyButton::setSizePolicies(QSizePolicy::Policy x, QSizePolicy::Policy y)
{
    this->setSizePolicy(x,y);
}


void MyButton::resizeEvent(QResizeEvent* event){
    int buttonMargin = style()->pixelMetric(QStyle::PM_ButtonMargin);
    QFont f = font();
    f.setPixelSize(event->size().height() - buttonMargin * 2);
    setFont(f);
}
