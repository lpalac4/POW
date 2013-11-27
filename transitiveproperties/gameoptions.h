#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include <QWidget>
#include "ui_gameoptions.h"
/**
 * @brief The GameOptions class used to show the game options screen.
 *  @author Luis Palacios
 */
class GameOptions : public QWidget, private Ui::GameOptionsUi
{
    Q_OBJECT
public:
    explicit GameOptions(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // GAMEOPTIONS_H
