#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>

class CustomButton : public QPushButton
{
public:
    CustomButton();


private:


    int size;
    QImage defaultButton;
    QImage pressedButton;

//    border-image:url(:/images/button.png);
//    color: rgb(255, 255, 255);
//    font: 4pt "Ubuntu";

    void setTextSize();
    void setRectSize();

};

#endif // CUSTOMBUTTON_H
