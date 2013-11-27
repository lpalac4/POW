/** \file winscreen.cpp
 *  @brief Definitions of WinScreen member functions.
 *  @author Andrew Ring
 */

#include "winscreen.h"

WinScreen::WinScreen(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

WinScreen::~WinScreen()
{

}

void WinScreen::setWinner(Player* winner)
{
    this->winner = winner;

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(winner->playerColorSet->getFillColor()));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    updateLabel();
}

void WinScreen::updateLabel()
{
    QLabel* winnerLabel = findChild<QLabel*>("winnerLabel");
    std::string message = "<font color='black'>" + winner->name + " is the WINNER!</font>";
    qDebug() << message.c_str();
    winnerLabel->setText(tr(message.c_str()));
    winnerLabel->update();
}
