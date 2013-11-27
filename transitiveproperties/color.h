/** \file color.h
 *  @brief Declarations for the Color class.
 *  @author Andrew Ring
 *  @author Charles Myers
 */

#ifndef COLOR_H
#define COLOR_H

#include <QColor>

/**
 * @brief Contains a selection of colors affiliated with a Player.
 * @author Andrew Ring
 */
class Color
{

private:
    /**
     * @brief The color which should be used to draw the borders of Territory areas on the map.
     */
    QRgb borderColor;
    /**
     * @brief The color which should be used to fill Territory areas on the map.  Also the primary
     * color associated with the Player.
     */
    QRgb fillColor;
    /**
     * @brief The color which should be used to fill Territory areas which have been selected.
     */
    QRgb highlightColor;

public:
    /**
     * @brief Default constructor.
     */
    Color(void);
    /**
     * @brief Constructor for explicitly setting colors at creation.
     * @param border The color for borders.
     * @param fill The color for filling a Territory.
     * @param highlight The color for filling a selected Territory.
     */
    Color(QRgb border, QRgb fill, QRgb highlight);

    /**
     * @brief Border color getter.
     * @return Border color.
     */
    QRgb getBorderColor(void);
    /**
     * @brief Fill color getter.
     * @return Fill color.
     */
    QRgb getFillColor(void);
    /**
     * @brief Highlight color getter.
     * @return Highlight color.
     */
    QRgb getHighlightColor(void);
};

#endif // COLOR_H
