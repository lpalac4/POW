/** \file gamesetup.h
 *  @brief Declaration of UI for game setup.
 *  @author Charles Myers
 *  @author Luis Palacios
 */

#ifndef GAMESETUP_H
#define GAMESETUP_H

#include <QWidget>
#include <QSignalMapper>
#include <vector>

#include "ui_gamesetup.h"

/**
 * @brief The GameSetup class used to show the game setup screen
 *  @author Charles Myers
 *  @author Luis Palacios
 */

class GameSetup : public QWidget, private Ui::GameSetupUi
{
    Q_OBJECT
private:
public:
    explicit GameSetup(QWidget *parent = 0);
    std::vector<QPushButton*> toggleButtons;

signals:
    
public slots:
    void updateButtonEnabledness(int whichButton);
    void toggleAIness(QString whichButton);
};

#endif // GAMESETUP_H
