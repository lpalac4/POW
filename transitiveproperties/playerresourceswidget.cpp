/** \file playerresourceswidget.cpp
 *  @brief Definitions for playerResourcesWidget member functions.
 *  @author Andrew Ring
 */

#include "playerresourceswidget.h"

playerResourcesWidget::playerResourcesWidget(QWidget *parent) :
    QWidget(parent)
{
    //icon
}

playerResourcesWidget::~playerResourcesWidget()
{

}

void playerResourcesWidget::updateLabels()
{
    Player* currentPlayer = MainWindow::currentPlayer;

    nameLabel.setText(QString::fromStdString(currentPlayer->name + "'s Resources"));

    cropNumLabel.setText(QString::fromStdString(""+currentPlayer->stash->crop));
    goldNumLabel.setText(QString::fromStdString(""+currentPlayer->stash->gold));
    ironNumLabel.setText(QString::fromStdString(""+currentPlayer->stash->iron));
    stoneNumLabel.setText(QString::fromStdString(""+currentPlayer->stash->stone));
    woodNumLabel.setText(QString::fromStdString(""+currentPlayer->stash->wood));
}
