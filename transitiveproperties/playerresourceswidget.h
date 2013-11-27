#ifndef PLAYERRESOURCESWIDGET_H
#define PLAYERRESOURCESWIDGET_H

#include <QWidget>
#include "mainwindow.h"

/**
 * @brief A display widget to show a player's resources.  Current version is text only.
 */
class playerResourcesWidget : public QWidget
{
private:
    Q_OBJECT

    /**
     * @brief nameLabel
     */
    QLabel nameLabel, cropNumLabel, goldNumLabel, ironNumLabel, stoneNumLabel, woodNumLabel;

public:
    explicit playerResourcesWidget(QWidget *parent = 0);
    ~playerResourcesWidget();
    
public slots:
    void updateLabels();
};

#endif // PLAYERRESOURCESWIDGET_H
