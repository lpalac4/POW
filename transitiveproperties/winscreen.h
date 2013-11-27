/** \file winscreen.h
 *  @brief Declaration of WinScreen class.
 *  @author Andrew Ring
 */

#ifndef WINSCREEN_H
#define WINSCREEN_H

#include <QWidget>
#include <QPalette>
#include <QColor>
#include <string>
#include <QDebug>
#include "ui_winscreen.h"
#include "player.h"
#include "color.h"

/**
 * @brief A screen used to display the winner of the game.
 */
class WinScreen : public QWidget, private Ui::WinScreenUi
{
    Q_OBJECT

    /**
     * @brief A pointer to the winning Player.
     */
    Player* winner;
    
    /**
     * @brief Sets the declaration of which player won as appropriate.
     */
    void updateLabel();

public:
    /**
     * @brief Default constructor.
     * @param parent The parent widget.
     */
    explicit WinScreen(QWidget *parent = 0);
    /**
     * @brief Destructor.
     */
    ~WinScreen();

    /**
     * @brief Sets the winning Player.  Should be called before displaying screen.
     * @param winner A pointer to the winning Player.
     */
    void setWinner(Player* winner);
    
};

#endif // WINSCREEN_H
