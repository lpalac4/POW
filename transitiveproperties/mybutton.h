/**
 *MyButton class.  Written by Luis Palacios
 *The class extends QPushButton and has its resizeEvent() overwritten to try to maintain text size
 *and aspect ratio(without a layout) proportional to the parent widget.
 */

#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QString>
#include <QStyle>
#include <sstream>
#include <QWidget>
#include <QResizeEvent>

class MyButton : public QPushButton
{
private:
    Q_OBJECT

    int textSize;
    QString defaultStyle;
    QString pressedStyle;
    QString completeStyleString;
    /**
     * @brief Overwritten call to maintain text size proportional to the size of the button.
     * @param event
     */
    void resizeEvent(QResizeEvent *event);

public:
    explicit  MyButton(QWidget *parent = 0);
    /**
     * @brief Dimensions of the buttons will maintain 10% width proportion to the parent widget and height
     *  will be proportional to the buttons width to maintain aspect ratio.
     * @param w - width of parent
     * @param h - height of parent
     */
    void setDimensions(int w, int h);
    void setSizePolicies(QSizePolicy::Policy x, QSizePolicy::Policy y);

};

#endif // MYBUTTON_H
