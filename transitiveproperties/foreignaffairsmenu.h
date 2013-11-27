/** \file foreignaffairsmenu.h
 *  @brief Declaration of UI for Foreign Affairs modification.
 *  @author Charles Myers
 */

#ifndef FOREIGNAFFAIRSMENU_H
#define FOREIGNAFFAIRSMENU_H

#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "mybutton.h"
#include <vector>
#include <list>

#include "player.h"
#include "color.h"
#include "playerindicator.h"
#include "mainwindow.h"

namespace Ui {
class ForeignAffairsMenu;
}

class ForeignAffairsMenu : public QWidget
{
    Q_OBJECT
    
public:
    static ForeignAffairsMenu* getForeignAffairsMenu(void);
    ~ForeignAffairsMenu();

    bool inFocus;
    
private:

    explicit ForeignAffairsMenu(list<Player*>* players, QWidget *parent = 0);
    static ForeignAffairsMenu* singleInstance;

    void update(Player* currentPlayer);

    int numberOfPlayers;
    std::vector<PlayerIndicator*>* indicators;
    std::vector<QComboBox*>* statusBars;

    Ui::ForeignAffairsMenu *ui;

signals:
    void foreignAffairsDone(void);

public slots:
    void closeWithUpdate(void);
    void closeWithoutUpdate(void);
};

#endif // FOREIGNAFFAIRSMENU_H
