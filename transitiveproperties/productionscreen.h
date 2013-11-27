/** \file productionscreen.h
 *  @brief Declarations for the ProductionScreen class.
 *  @author Andrew Ring
 */

#ifndef PRODUCTIONSCREEN_H
#define PRODUCTIONSCREEN_H

#include <QWidget>
#include "ui_production.h"
#include "mainwindow.h"
#include "production.h"
#include "resources.h"
#include "mapimage.h"

/**
 * @brief The screen used to produce units.
 * @author Andrew Ring
 */
class ProductionScreen : public QWidget, private Ui::ProductionUi
{
private:
    Q_OBJECT

    /**
     * @brief A pointer to the image of the map.
     */
    MapImage* mapPix;

public:
    /**
     * @brief Default constructor.
     * @param parent The parent widget.
     */
    explicit ProductionScreen(QWidget *parent = 0);
    
    /**
     * @brief Rewrites the resource display label to show current Player's current Resources.
     */
    void updateProductionResourceDisplay();

    /**
     * @brief Setter for mapPix.  Should be set before displaying.
     * @param mapPix A pointer to the image of the map.
     */
    void setMapImage(MapImage *mapPix);

    /**
     * @brief Asks a yes or no question to the user.
     * @param question The text to ask the user.
     * @return True iff they press yes, else False.
     */
    bool popupQuestion(const char *question);

    void changeButtonSize(QPushButton *button, QScrollArea *scroll, QPushButton *endButtonPtr);
signals:
    /**
     * @brief Signifies that production is finished.
     */
    void productionFinished();
    
public slots:
    /**
     * @brief Updates which buttons are enabled based on the currently selected Territory and current
     * Player's Resources.
     */
    void updateProductionButtons();

    /**
     * @brief Checks if the player really wants to build the specified unit.  If so, builds it and expends resources.
     * @param typeToBuild The type of unit to build.
     */
    void productionCheck(production::units typeToBuild, bool ai = false);

    /**
     * @brief Wrapper slot for productionCheck(BOAT)
     */
    void boatButtonClicked();
    /**
     * @brief Wrapper slot for productionCheck(CITY)
     */
    void cityButtonClicked();
    /**
     * @brief Wrapper slot for productionCheck(RAM)
     */
    void ramButtonClicked();
    /**
     * @brief Wrapper slot for productionCheck(SILO)
     */
    void siloButtonClicked();
    /**
     * @brief Wrapper slot for productionCheck(WALL)
     */
    void wallButtonClicked();
    /**
     * @brief Wrapper slot for productionCheck(WEAPON)
     */
    void weaponButtonClicked();
};

#endif // PRODUCTIONSCREEN_H
