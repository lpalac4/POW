#ifndef TRANSPORTMENU_H
#define TRANSPORTMENU_H

#include <QWidget>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QList>
#include <QRadioButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>

#include "territory.h"
#include "mainwindow.h"
#include "mapimage.h"
#include "mapeditor.h"
#include "player.h"
#include "transportmenuoptions.h"

namespace Ui {
class TransportMenu;
}

class TransportMenu : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransportMenu(QWidget *parent = 0);
    ~TransportMenu();

private slots:
    void on_acceptButton_clicked();
    /**
     * @brief Enables the Accept button.
     */
    void enableAcceptButton(void);
    /**
     * @brief Resets push buttons.
     */
    void resetButtons(void);

    void on_skipButton_clicked();

    void on_deselectButton_clicked();

    void deselect();

public slots:
    void setSourceTerritory(Territory* targetTerritory);
    void setDestinationTerritory(Territory* targetTerritory);
    void acceptTransportManifest(materiel toTransport);

private:
    Territory* sourceTerritory;
    Territory* destinationTerritory;
    Ui::TransportMenu *ui;
    void clearButtonGroups(void);
    bool stash, boat, horse, weapon, ram;
    //MapImage* mapImage;
    QGraphicsScene* mapScene;
    QGraphicsView* mapView;
    //MapEditor* mapEdit;
    materiel itemsToTransport;
    void resetMateriel();



signals:
    void finished(void);
    void sourceHasBeenSet(Territory* source);
    void chosenShipmentFromSource(Territory* source, int radius);
    void deselectAll(void);
};

#endif // TRANSPORTMENU_H
