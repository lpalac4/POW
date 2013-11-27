#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include "ui_mainmenu.h"

/**
 * @brief The MainMenu class for the main menu screen
 *  @author Luis Palacios
 */

class MainMenu : public QWidget, private Ui::MainMenuUi
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);
    
signals:
    
public slots:

private slots:

    
};

#endif // MAINMENU_H
