#ifndef TRANSPORTMENUOPTIONS_H
#define TRANSPORTMENUOPTIONS_H

#include <QWidget>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QList>

#include "territory.h"

namespace Ui {
class TransportMenuOptions;
}

struct materiel{
    bool stash;
    bool boat;
    bool horse;
    bool weapon;
    bool ram;
};

class TransportMenuOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransportMenuOptions(QWidget *parent = 0);
    ~TransportMenuOptions();

private:
    Ui::TransportMenuOptions *ui;
    QButtonGroup radioGroup, boatGroup, horseGroup;
    void setBoatGroupEnabled(bool enabled);
    void setHorseGroupEnabled(bool enabled);
    void clearButtonGroups();
    void setButtonEnabled(QAbstractButton* button, bool enable);

signals:
    void chosenMateriel(materiel chosen);
    void goBack(void);

public slots:
    void setButtons(Territory* sourceTerritory);

private slots:
    void setUnitButtonsEnabled(bool enabled);
    void setUnitButtonsChecked(bool checked);
    void on_acceptButton_clicked();
    void on_boatRadio_toggled(bool checked);
    void on_horseRadio_toggled(bool checked);
    void on_backButton_clicked();
};

#endif // TRANSPORTMENUOPTIONS_H
