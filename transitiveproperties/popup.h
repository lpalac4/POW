/** \file popup.h
 *  @brief Declarations for the Popup class.
 *  @author Andrew Ring
 */

#ifndef POPUP_H
#define POPUP_H

#include <QMessageBox>

/**
 * @brief Provides the ability to display popup message boxes for a fixed amount of time.
 * @author Andrew Ring
 */
class Popup : public QMessageBox
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor.
     * @param parent The parent widget to display over.
     */
    explicit Popup(QWidget *parent = 0);

    /**
     * @brief Displays a pop message box for a specified amount of time.
     * @param message The message to display in the box.
     * @param millisecondsSleep How long to display for, in milliseconds.
     */
    static void display(const char* message, int millisecondsSleep);
signals:
    
public slots:
    
};

#endif // POPUP_H
