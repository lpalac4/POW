/** \file popup.cpp
 *  @brief Definitions of Popup member functions.
 *  @author Andrew Ring
 */

#include <QMessageBox>
#include <QString>

#include "popup.h"

Popup::Popup(QWidget *parent) :
    QMessageBox(parent){

}

void Popup::display(const char* message, int millisecondsSleep){
    Popup popupBox;
    popupBox.setText(QString(message));
    popupBox.show();
    QPoint pos = popupBox.parentWidget()->mapToGlobal(QPoint(popupBox.parentWidget()->width()/2  - popupBox.width()/2,
                                    popupBox.parentWidget()->height()/2 - popupBox.height()/2));
    popupBox.move(pos);
    usleep(millisecondsSleep * 1000);
}

