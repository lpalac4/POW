/**
 * LoadProgressScreen class written by Luis Palacios
 * Class to be used as the splash screen during the time data structures and drawing
 * is being computed.  Class is limited to a static screen due to our graphics engine
 * stalling any and all drawing threads including signals.
 */

#ifndef LOADPROGRESSSCREEN_H
#define LOADPROGRESSSCREEN_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include <QGraphicsPixmapItem>



namespace Ui {
class LoadProgressScreen;
}

/**
 * @brief The LoadProgressScreen class used to show the loading screen.
 */

class LoadProgressScreen : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief Default constructor.
     * @param parent
     */
    explicit LoadProgressScreen(QWidget *parent = 0);
    /**
     * Default destructor.
     */
    ~LoadProgressScreen();
    /**
     * @brief To be used to set the progress bar.
     * @param width
     * @param height
     */
    void setNumberOfHexes(int width, int height);
    /**
     * @brief To be used to draw a rotating circle.
     */
    void startAnimation();

private:
    Ui::LoadProgressScreen *ui;
    QLabel* progressLabel;
    QLabel* tips;
    /**
     * @brief Randomly assign a tip for the loadscreen.
     */
    void setTips();
    /**
     * @brief To be used to set the status text to inform user of progress.
     */
    void setProgressText();
    /**
     * @brief To be used to display hexagon image.
     */
    QGraphicsView* hexView;
    /**
     * @brief Value as a percentage of total number of hexes.
     */
    int progression;
    int numberOfHex;

public slots:
    void setProgress();

signals:
    void finishedLoading();

    

};

#endif // LOADPROGRESSSCREEN_H
