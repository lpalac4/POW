/** \file color.cpp
 *  @brief Definitions for Color class member functions.
 *  @author Andrew Ring
 *  @author Charles Myers
 */

#include "color.h"

Color::Color(void)
{
    borderColor = qRgb(0,0,0);
    fillColor = qRgb(0,0,0);
    highlightColor = qRgb(0,0,0);
}

Color::Color(QRgb border, QRgb fill, QRgb highlight){
    borderColor = border;
    fillColor = fill;
    highlightColor = highlight;
}

QRgb Color::getBorderColor(void){
    return borderColor;
}

QRgb Color::getFillColor(void){
    return fillColor;
}

QRgb Color::getHighlightColor(void){
    return highlightColor;
}
