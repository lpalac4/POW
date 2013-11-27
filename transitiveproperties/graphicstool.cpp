/** \file graphicstool.cpp
 *  @brief Definitions of graphics utility functions, including polygon scan-conversion.
 *  @author Charles Myers
 */

#include "graphicstool.h"

GraphicsTool::GraphicsTool(QImage* image, int imageWidth, int imageHeight, QObject *parent) :
    QObject(parent)
{
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->image = image;
}

int GraphicsTool::getImageWidth(void){
    return imageWidth;
}
int GraphicsTool::getImageHeight(void){
    return imageHeight;
}

/***************************************************************************************/

void GraphicsTool::clear(QRgb clearColor){
    for(int y=0; y<imageHeight; y++) {
        QRgb* lineOfPixels = (QRgb*) image->scanLine(y);
        for(int x=0; x<imageWidth; x++)
            *(lineOfPixels + x) = clearColor;
    }
}

/***************************************************************************************/

void GraphicsTool::drawPolygon(int vertices[][2], int numberOfVertices, QRgb color, Territory* currentTerritory, Territory*** pixelTerritoryMapping){
    Edge* edgeTable[imageHeight];
    for(int i=0; i<imageHeight; i++) edgeTable[i] = NULL;
    int yMin = imageHeight;

    //create edge table
    for(int i=0; i<numberOfVertices; i++){
        int* vertex1 = *(vertices+i);
        int* vertex2;
        if(i == numberOfVertices - 1) vertex2 = *vertices;
        else vertex2 = *(vertices+i+1);

        int x1 = vertex1[0], x2 = vertex2[0], y1 = vertex1[1], y2 = vertex2[1];

        int tableIndex;

        if(y1 == y2) continue;//no need to process horizontal edges

        //edge* thisEdge = new edge;
        int xMin, yMax, rise, run;

        if(y1 < y2){
            xMin = x1;
            yMax = y2;
            tableIndex = y1;
            rise = y2 - y1;
            run = x2 - x1;
        }else{
            xMin = x2;
            yMax = y1;
            tableIndex = y2;
            rise = y1 - y2;
            run = x1 - x2;
        }
        Edge* thisEdge = new Edge(xMin, yMax, rise, run);

        if(tableIndex < yMin) yMin = tableIndex;

        Edge* tableIterator = edgeTable[tableIndex];

        if(!tableIterator){
            edgeTable[tableIndex] = thisEdge;
        }else if(tableIterator->getXMin() > thisEdge->getXMin()){
            thisEdge->setNext(tableIterator);
            edgeTable[tableIndex] = thisEdge;
        }else{
            Edge* prev = tableIterator;
            tableIterator = tableIterator->getNext();
            while(true){
                if(!tableIterator){
                    prev->setNext(thisEdge);
                    break;
                }else{
                    if(tableIterator->getXMin() > thisEdge->getXMin()){
                        thisEdge->setNext(tableIterator);
                        prev->setNext(thisEdge);
                        break;
                    }else{
                        prev = tableIterator;
                        tableIterator = tableIterator->getNext();
                    }
                }
            }
        }
    }//end edge table construction

    //begin filling
    Edge* activeEdge = NULL;
    for(int y=yMin; y<imageHeight; y++){
        mergeEdges(activeEdge, edgeTable[y]);
        if(!activeEdge) break;//if no more edges, we are done
        Edge* edgeRunner = activeEdge;
        bool parity = false;

        QRgb* lineOfPixels = (QRgb*) image->scanLine(y);
        for(int x=0; x<imageWidth; x++){
            if(x == edgeRunner->getXMin()){
                parity = !parity;
                edgeRunner = edgeRunner->getNext();
                if(!edgeRunner) break;
            }

            //where pixels are actually set (also the pixelTerritoryMapping is set here is that is called for)
            if(parity){
                *(lineOfPixels + x) = color;
                if(currentTerritory){
                    *(*(pixelTerritoryMapping + y) + x) = currentTerritory;
                }
            }

            if(x == edgeRunner->getXMin()){
                parity = !parity;
                edgeRunner = edgeRunner->getNext();
                if(!edgeRunner) break;
            }
        }//end fill loop x
        clearActiveEdges(activeEdge, y+1);
        incrementEdges(activeEdge);
    }//end fill loop y
}

/***************************************************************************************/

void GraphicsTool::drawLine(int x0, int y0, int x1, int y1, QRgb color){
    if(x1 < x0){//switch point0 to the leftmost if necessary
        int temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int dy = y1 - y0, dx = x1 - x0;

    if(dy == 0) drawHorizontalLine(x0, y0, x1, color);
    if(dx == 0) drawVerticalLine(x0, y0, y1, color);
    if(dy > 0 && dy <= dx) drawPositiveLowLine(x0, y0, x1, dy, dx, color);
    else if(dy > 0) drawPositiveSteepLine(x0, y0, y1, dy, dx, color);
    else if((-dy) <= dx) drawNegativeLowLine(x0, y0, x1, dy, dx, color);
    else drawNegativeSteepLine(x0, y0, y1, dy, dx, color);
}

/***************************************************************************************/

void GraphicsTool::bubbleOnce(Edge* &activeEdge){
    if(!(activeEdge->getNext())) return; //only one element - no need to bubble

    Edge* prevInList = NULL;
    Edge* currentInList = activeEdge;
    Edge* nextInList = activeEdge->getNext();

    while(nextInList){
        if(currentInList->getXMin() > nextInList->getXMin()){
            if(prevInList) prevInList->setNext(nextInList);
            else activeEdge = nextInList;

            currentInList->setNext(nextInList->getNext());
            nextInList->setNext(currentInList);

            Edge* temp = currentInList;
            currentInList = nextInList;
            nextInList = temp;
        }

        if(prevInList) prevInList = prevInList->getNext();
        else prevInList = currentInList;
        currentInList = currentInList->getNext();
        nextInList = nextInList->getNext();
    }//end while
}//end bubbleOnce

/***************************************************************************************/

void GraphicsTool::incrementEdges(Edge* &activeEdge){
    if(!activeEdge) return;
    for(Edge* edgeRunner = activeEdge; edgeRunner; edgeRunner = edgeRunner->getNext()){
        int increment;
        if(edgeRunner->getRun() == 0) continue;
        else if(edgeRunner->getRun() < 0){
            increment = -1;
            edgeRunner->setXFrac(edgeRunner->getXFrac() + edgeRunner->getRun());
            while(edgeRunner->getXFrac() < -(edgeRunner->getRise())){
                edgeRunner->setXMin(edgeRunner->getXMin() + increment);
                edgeRunner->setXFrac(edgeRunner->getXFrac() + edgeRunner->getRise());
            }
        }else{
            increment = 1;
            edgeRunner->setXFrac(edgeRunner->getXFrac() + edgeRunner->getRun());
            while(edgeRunner->getXFrac() >= edgeRunner->getRise()){
                edgeRunner->setXMin(edgeRunner->getXMin() + increment);
                edgeRunner->setXFrac(edgeRunner->getXFrac() - edgeRunner->getRise());
            }
        }
    }//end  for
    bubbleOnce(activeEdge);//not sure if this will be sufficient for every possible polygon...
}//end incrementEdges

/***************************************************************************************/

void GraphicsTool::mergeEdges(Edge* &activeEdge, Edge* tableEdge){
    if(!tableEdge) return;
    if(!activeEdge){
        activeEdge = tableEdge;
        return;
    }

    Edge* temp = NULL;
    Edge* newList = NULL; //will hold sorted edges

    while(tableEdge && activeEdge){
        if(tableEdge->getXMin() < activeEdge->getXMin()){
            if(!newList){
                newList = tableEdge;
                temp = tableEdge;
            }else{
                temp->setNext(tableEdge);
                temp = tableEdge;
            }
            tableEdge = tableEdge->getNext();
        }else{
            if(!newList){
                newList = activeEdge;
                temp = activeEdge;
            }else{
                temp->setNext(activeEdge);
                temp = activeEdge;
            }
            activeEdge = activeEdge->getNext();
        }
    }

    if(!tableEdge) temp->setNext(activeEdge);
    else temp->setNext(tableEdge);

    activeEdge = newList;

}//end mergeEdges

/***************************************************************************************/

void GraphicsTool::clearActiveEdges(Edge* &activeEdge, int y){
    if(!activeEdge){
        return;
    }

    //clear active edges of finished edges
    //find first keepable edge
    for(; activeEdge && activeEdge->getYMax() <= y; activeEdge = activeEdge->getNext());
    //must check for emptiness again
    if(!activeEdge){
        return;
    }

    //then remove all other finished edges
    Edge* previousActiveEdge = activeEdge;
    Edge* activeEdgeNextHolder = activeEdge->getNext();
    while(activeEdgeNextHolder){
        if(activeEdgeNextHolder->getYMax() <= y){
            previousActiveEdge->setNext(activeEdgeNextHolder->getNext());
        }else{
            previousActiveEdge = activeEdgeNextHolder;
        }
        activeEdgeNextHolder = activeEdgeNextHolder->getNext();
    }

}//end clearActiveEdges

/***************************************************************************************/

void GraphicsTool::drawNegativeSteepLine(int x0, int y0, int y1, int dy, int dx, QRgb color){
    dy = -dy;
        int dE = dx, dNE = dx - dy, d = dx - dy/2;
        for(; y0>=y1; y0--){

            QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
            *(lineOfPixels + x0) = color;

            if(d<0){
                d += dE;
            }else{
                x0++;
                d += dNE;
            }
        }
}//end drawNegativeSteepLine

/***************************************************************************************/

void GraphicsTool::drawNegativeLowLine(int x0, int y0, int x1, int dy, int dx, QRgb color){
    dy = -dy;
        int dE = dy, dNE = dy - dx, d = dy - dx/2;
        for(; x0<=x1; x0++){

            QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
            *(lineOfPixels + x0) = color;

            if(d<0){
                d += dE;
            }else{
                y0--;
                d += dNE;
            }
        }

}//end drawNegativeLowLine

/***************************************************************************************/

void GraphicsTool::drawPositiveSteepLine(int x0, int y0, int y1, int dy, int dx, QRgb color){
    int dE = dx, dNE = dx - dy, d = dx - dy/2;
        for(; y0<=y1; y0++){

            QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
            *(lineOfPixels + x0) = color;

            if(d<0){
                d += dE;
            }else{
                x0++;
                d += dNE;
            }
        }
}//end drawPositiveSteepLine

/***************************************************************************************/

void GraphicsTool::drawPositiveLowLine(int x0, int y0, int x1, int dy, int dx, QRgb color){
    int dE = dy, dNE = dy - dx, d = dy - dx/2;
        for(; x0<=x1; x0++){

            QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
            *(lineOfPixels + x0) = color;

            if(d<0){
                d += dE;
            }else{
                y0++;
                d += dNE;
            }
        }

}//end drawPositiveLowLine

/***************************************************************************************/

void GraphicsTool::drawVerticalLine(int x0, int y0, int y1, QRgb color){
    for(; y0<=y1; y0++){
        QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
        *(lineOfPixels + x0) = color;
    }

}//end drawVerticalLine

/***************************************************************************************/

void GraphicsTool::drawHorizontalLine(int x0, int y0, int x1, QRgb color){
    for(; x0<=x1; x0++) {
        QRgb* lineOfPixels = (QRgb*) image->scanLine(y0);
        *(lineOfPixels + x0) = color;
    }

}//end drawHorizontalLine
