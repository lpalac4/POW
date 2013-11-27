/** \file mapeditor.cpp
 *  @brief Definitions of map drawing methods.
 *  @author Charles Myers
 */

#include "mapeditor.h"

using namespace std;

MapEditor::MapEditor(MapData* mapData, QObject *parent) :
    QObject(parent)
{
    this->mapImage = NULL;
    this->mapData = mapData;
    this->graphicsTool = NULL;
    this->initialized = false;

    this->imageWidth = 0;
    this->imageHeight = 0;
    this->hexesWide = 0;
    this->hexesHigh = 0;

    this->hexPixelHeight = HEX_HEIGHT;
    this->hexPixelWidth = hexPixelHeight * 200/173;
    this->threeQuarterWidth = hexPixelWidth*3/4;
    this->halfHeight = hexPixelHeight/2;

    this->borderWidth = hexPixelHeight/HEXAGON_HEIGHT_TO_BORDER_HEIGHT_RATIO;
    this->borderBase = borderWidth/ROOT_THREE;
    this->borderHypoteneuse = borderBase*2;

    this->iconSquareSideLength = hexPixelWidth/2 * (3 - ROOT_THREE);
}

//iterate through the map data and tell graphicsTool to redraw it
//POSSIBLE ERRORS: I'm not quite sure how the integer rounding will
//affect the hexagon vertices, but it'll be far easier to debug this
//once we can see what is being drawn.

void MapEditor::redraw(void){
    std::set<Territory*> changedTerritories;
    redraw(changedTerritories);
}

void MapEditor::redraw(Territory* changedTerritory){
    std::set<Territory*> changedTerritories;
    changedTerritories.insert(changedTerritory);
    redraw(changedTerritories);
}

void MapEditor::redraw(std::set<Territory*> changedTerritories){
    clock_t startTime;
    if(testingFlags::TEST_CALC_RUNTIME) {
        startTime = clock();
    }
    qDebug() << "Redrawing map image.";

    //variables to hold hex coordinates
    int leftMiddleX = 0, topLeftX = hexPixelWidth/4, topRightX = threeQuarterWidth, rightMiddleX = hexPixelWidth - 1, bottomLeftX = hexPixelWidth/4, bottomRightX = threeQuarterWidth;
    int leftMiddleY = halfHeight, topLeftY = 0, topRightY = 0, rightMiddleY = halfHeight, bottomLeftY = hexPixelHeight - 1, bottomRightY = hexPixelHeight - 1;

    int hexVertices[6][2] = {//initialize first hexagon
                             {leftMiddleX,leftMiddleY},{topLeftX,topLeftY},{topRightX,topRightY},
                             {rightMiddleX,rightMiddleY},{bottomRightX,bottomRightY},{bottomLeftX,bottomLeftY}
                            };

    int iconCoordinates[2];
    if(!initialized) deriveIconCoordinates(iconCoordinates);
    int initialIconY = iconCoordinates[1];

    //get the territory info from mapData
    Territory*** hexGrid = mapData->getHexGrid();

    if(!initialized) initializeMap();

    //loop to redraw each hexagon
    for(int x=0; x<hexesWide; x++){
        for(int y=0; y<hexesHigh; y++){

//            if(changedTerritory && hexGrid[y][x] != changedTerritory){
            if(!changedTerritories.empty() && changedTerritories.count(hexGrid[y][x]) == 0){
                //increment y coordinates as we move down the column
                for(int i=0; i<6; i++) hexVertices[i][1] += hexPixelHeight-1;
                //iconCoordinates[1] += hexPixelHeight-1;
                continue;
            }

            QRgb fillColor;
            QRgb borderColor;
            //conditionally set the colors to be used
            if(hexGrid[y][x]){
                Player* owner = hexGrid[y][x]->ownedBy;
                borderColor = owner->playerColorSet->getBorderColor();
                if(hexGrid[y][x]->selected){
                    fillColor = owner->playerColorSet->getHighlightColor();
                }else{
                    fillColor = owner->playerColorSet->getFillColor();
                }
            }else{
                fillColor = qRgb(31,97,102);
                borderColor = qRgb(253,255,200);
            }

            //draw the polygon!
            if(!initialized){
                graphicsTool->drawPolygon(hexVertices, 6, fillColor, hexGrid[y][x], mapData->getPixelTerritoryMapping());
                if(hexGrid[y][x]) {//if not water, set the candidate coordinates for icon drawing
                    setIconCoordinates(iconCoordinates, hexGrid[y][x]);
                }
            }else{
                graphicsTool->drawPolygon(hexVertices, 6, fillColor);
            }

            //checkForOpposedTerritory is used to determine whether a border should
            //be drawn between any two adjacent hexes. The mod conditionals are necessary
            //because indexing a hex grid is WEIRD.
            if(checkForOpposedTerritory(hexGrid,x,y,x,y-1)) {
                drawBorder(hexVertices, TOP, borderColor);
            }
            if(checkForOpposedTerritory(hexGrid,x,y,x,y+1)) {
                drawBorder(hexVertices, BOTTOM, borderColor);
            }

            if(checkForOpposedTerritory(hexGrid,x,y,x-1,y)){
                if(x%2 == 0) {
                    drawBorder(hexVertices, BOTTOM_LEFT, borderColor);
                } else {
                    drawBorder(hexVertices, TOP_LEFT, borderColor);
                }
            }
            if(checkForOpposedTerritory(hexGrid,x,y,x+1,y)){
                if(x%2 == 0) {
                    drawBorder(hexVertices, BOTTOM_RIGHT, borderColor);
                } else {
                    drawBorder(hexVertices, TOP_RIGHT, borderColor);
                }
            }
            if(x%2 == 0){
                if(checkForOpposedTerritory(hexGrid,x,y,x-1,y-1))  {
                    drawBorder(hexVertices, TOP_LEFT, borderColor);
                }
                if(checkForOpposedTerritory(hexGrid,x,y,x+1,y-1)) {
                    drawBorder(hexVertices, TOP_RIGHT, borderColor);
                }
            }else{
                if(checkForOpposedTerritory(hexGrid,x,y,x-1,y+1)) {
                    drawBorder(hexVertices, BOTTOM_LEFT, borderColor);
                }
                if(checkForOpposedTerritory(hexGrid,x,y,x+1,y+1)) {
                    drawBorder(hexVertices, BOTTOM_RIGHT, borderColor);
                }
            }

            //increment y coordinates as we move down the column
            for(int i=0; i<6; i++) hexVertices[i][1] += hexPixelHeight-1;
            iconCoordinates[1] += hexPixelHeight-1;
        }

        //increment x coordinates as we move to the next column
        for(int i=0; i<6; i++) hexVertices[i][0] += threeQuarterWidth-1;
        iconCoordinates[0] += threeQuarterWidth-1;

        //reset the y coordinates to the top of the column
        if(x%2 == 0){
            hexVertices[0][1] = leftMiddleY + halfHeight;
            hexVertices[1][1] = topLeftY + halfHeight;
            hexVertices[2][1] = topRightY + halfHeight;
            hexVertices[3][1] = rightMiddleY + halfHeight;
            hexVertices[4][1] = bottomRightY + halfHeight;
            hexVertices[5][1] = bottomLeftY + halfHeight;

            iconCoordinates[1] = initialIconY + halfHeight;
        }else{
            hexVertices[0][1] = leftMiddleY;
            hexVertices[1][1] = topLeftY;
            hexVertices[2][1] = topRightY;
            hexVertices[3][1] = rightMiddleY;
            hexVertices[4][1] = bottomRightY;
            hexVertices[5][1] = bottomLeftY;

            iconCoordinates[1] = initialIconY;
        }    
    }

    //once everything else is drawn, draw the icons on top
    if(changedTerritories.empty()) {
        drawIcons(NULL);
    } else {
        std::set<Territory*>::iterator setIt;
        for(setIt = changedTerritories.begin(); setIt != changedTerritories.end(); setIt++){
            drawIcons(*setIt);
        }
    }

    if(!initialized) initialized = true;

    mapImage->save("Resources/MapImage/mapImage.png");//should probably be a configurable file name
    //signal whoever that we've updated the QImage holding the map
    //=> it should probably be redisplayed
    emit imageUpdated(mapImage);

    if(testingFlags::TEST_CALC_RUNTIME) {
        double elapsedTime = double(clock() - startTime)/CLOCKS_PER_SEC;
        qDebug() << "redraw() took " << elapsedTime << " seconds";
    }
}

//draw the trapezoidal borders
void MapEditor::drawBorder(int hexVertices[6][2], hexSide side, QRgb borderColor){

    int quadrangleVertices[4][2];

    switch (side){
    case TOP:
        quadrangleVertices[0][0] = hexVertices[1][0];
        quadrangleVertices[0][1] = hexVertices[1][1];
        quadrangleVertices[1][0] = hexVertices[2][0];
        quadrangleVertices[1][1] = hexVertices[2][1];

        quadrangleVertices[2][0] = hexVertices[2][0] + borderBase;
        quadrangleVertices[2][1] = hexVertices[2][1] + borderWidth;
        quadrangleVertices[3][0] = hexVertices[1][0] - borderBase;
        quadrangleVertices[3][1] = hexVertices[1][1] + borderWidth;
        break;
    case BOTTOM:
        quadrangleVertices[0][0] = hexVertices[5][0];
        quadrangleVertices[0][1] = hexVertices[5][1];
        quadrangleVertices[1][0] = hexVertices[4][0];
        quadrangleVertices[1][1] = hexVertices[4][1];

        quadrangleVertices[2][0] = hexVertices[4][0] + borderBase;
        quadrangleVertices[2][1] = hexVertices[4][1] - borderWidth;
        quadrangleVertices[3][0] = hexVertices[5][0] - borderBase;
        quadrangleVertices[3][1] = hexVertices[5][1] - borderWidth;
        break;
    case TOP_LEFT:
        quadrangleVertices[0][0] = hexVertices[0][0];
        quadrangleVertices[0][1] = hexVertices[0][1];
        quadrangleVertices[1][0] = hexVertices[1][0];
        quadrangleVertices[1][1] = hexVertices[1][1];

        quadrangleVertices[2][0] = hexVertices[1][0] + borderHypoteneuse;
        quadrangleVertices[2][1] = hexVertices[1][1];
        quadrangleVertices[3][0] = hexVertices[0][0] + borderBase;
        quadrangleVertices[3][1] = hexVertices[0][1] + borderWidth;
        break;
    case TOP_RIGHT:
        quadrangleVertices[0][0] = hexVertices[2][0];
        quadrangleVertices[0][1] = hexVertices[2][1];
        quadrangleVertices[1][0] = hexVertices[3][0];
        quadrangleVertices[1][1] = hexVertices[3][1];

        quadrangleVertices[2][0] = hexVertices[3][0] - borderBase;
        quadrangleVertices[2][1] = hexVertices[3][1] + borderWidth;
        quadrangleVertices[3][0] = hexVertices[2][0] - borderHypoteneuse;
        quadrangleVertices[3][1] = hexVertices[2][1];
        break;
    case BOTTOM_LEFT:
        quadrangleVertices[0][0] = hexVertices[0][0];
        quadrangleVertices[0][1] = hexVertices[0][1];
        quadrangleVertices[1][0] = hexVertices[5][0];
        quadrangleVertices[1][1] = hexVertices[5][1];

        quadrangleVertices[2][0] = hexVertices[5][0] + borderHypoteneuse;
        quadrangleVertices[2][1] = hexVertices[5][1];
        quadrangleVertices[3][0] = hexVertices[0][0] + borderBase;
        quadrangleVertices[3][1] = hexVertices[0][1] - borderWidth;
        break;
    case BOTTOM_RIGHT:
        quadrangleVertices[0][0] = hexVertices[4][0];
        quadrangleVertices[0][1] = hexVertices[4][1];
        quadrangleVertices[1][0] = hexVertices[3][0];
        quadrangleVertices[1][1] = hexVertices[3][1];

        quadrangleVertices[2][0] = hexVertices[3][0] - borderBase;
        quadrangleVertices[2][1] = hexVertices[3][1] - borderWidth;
        quadrangleVertices[3][0] = hexVertices[4][0] - borderHypoteneuse;
        quadrangleVertices[3][1] = hexVertices[4][1];
        break;
    default:
        cout << "ERROR: Not a side?" << endl;
        cout.flush();
        exit(1);
    }

    /*    qDebug() << quadrangleVertices[0][0] << " "
             << quadrangleVertices[1][0] << " "
             << quadrangleVertices[2][0] << " "
             << quadrangleVertices[3][0] << " ";

    qDebug() << quadrangleVertices[0][1] << " "
             << quadrangleVertices[1][1] << " "
             << quadrangleVertices[2][1] << " "
             << quadrangleVertices[3][1] << " ";
    qDebug() << ""; */

    graphicsTool->drawPolygon(quadrangleVertices, 4, borderColor);
}

//draw the resource/unit images over their proper hexes
void MapEditor::drawIcons(Territory* changedTerritory){

    if(changedTerritory){
        int* thisCoordinateSet[9];
        for(int i=0; i<9; i++){
            *(thisCoordinateSet + i) = (changedTerritory->iconLocalesOrganizedByTypeOfIcon)[i];
        }

        if(changedTerritory->stockpile) drawThisIconHere(imagePaths::stockpile, thisCoordinateSet[0][0], thisCoordinateSet[0][1]);
        if(changedTerritory->cityPresent) drawThisIconHere(imagePaths::city, thisCoordinateSet[1][0], thisCoordinateSet[1][1]);
        if(changedTerritory->weaponPresent) drawThisIconHere(imagePaths::weapon, thisCoordinateSet[2][0], thisCoordinateSet[2][1]);
        if(changedTerritory->horsePresent) drawThisIconHere(imagePaths::horse, thisCoordinateSet[3][0], thisCoordinateSet[3][1]);
        if(changedTerritory->wallPresent) drawThisIconHere(imagePaths::wall, thisCoordinateSet[4][0], thisCoordinateSet[4][1]);
        if(changedTerritory->ramPresent) drawThisIconHere(imagePaths::ram, thisCoordinateSet[5][0], thisCoordinateSet[5][1]);

        switch(changedTerritory->resourceGen){
        case WOOD:
            drawThisIconHere(imagePaths::wood, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            if(changedTerritory->affectedByCity) drawThisIconHere(imagePaths::cityWood, thisCoordinateSet[7][0], thisCoordinateSet[7][1]);
            if(changedTerritory->siloPresent) drawThisIconHere(imagePaths::siloWood, thisCoordinateSet[8][0], thisCoordinateSet[8][1]);
            break;
        case IRON:
            drawThisIconHere(imagePaths::iron, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            if(changedTerritory->affectedByCity) drawThisIconHere(imagePaths::cityIron, thisCoordinateSet[7][0], thisCoordinateSet[7][1]);
            if(changedTerritory->siloPresent) drawThisIconHere(imagePaths::siloIron, thisCoordinateSet[8][0], thisCoordinateSet[8][1]);
            break;
        case STONE:
            drawThisIconHere(imagePaths::coal, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            if(changedTerritory->affectedByCity) drawThisIconHere(imagePaths::cityCoal, thisCoordinateSet[7][0], thisCoordinateSet[7][1]);
            if(changedTerritory->siloPresent) drawThisIconHere(imagePaths::siloCoal, thisCoordinateSet[8][0], thisCoordinateSet[8][1]);
            break;
        case GOLD:
            drawThisIconHere(imagePaths::gold, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            if(changedTerritory->affectedByCity) drawThisIconHere(imagePaths::cityGold, thisCoordinateSet[7][0], thisCoordinateSet[7][1]);
            if(changedTerritory->siloPresent) drawThisIconHere(imagePaths::siloGold, thisCoordinateSet[8][0], thisCoordinateSet[8][1]);
            break;
        case CROP:
            drawThisIconHere(imagePaths::wheat, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            if(changedTerritory->affectedByCity) drawThisIconHere(imagePaths::cityWheat, thisCoordinateSet[7][0], thisCoordinateSet[7][1]);
            if(changedTerritory->siloPresent) drawThisIconHere(imagePaths::siloWheat, thisCoordinateSet[8][0], thisCoordinateSet[8][1]);
            break;
        case HORSE:
            drawThisIconHere(imagePaths::barn, thisCoordinateSet[6][0], thisCoordinateSet[6][1]);
            break;
        case NONE:
        case COUNT:
            break;
        }
    }else{//check them all
        std::list<Territory*>::iterator i;
        for(i = Territory::allTerritoryList.begin(); i != Territory::allTerritoryList.end(); i++){
            drawIcons(*i);
        }
    }
}

void MapEditor::drawThisIconHere(const char* imagePath, int x, int y){

    QImage iconImage(imagePath, "png");
    QImage scaledImage = iconImage.scaled(iconSquareSideLength, iconSquareSideLength);

    for(int i=0; i<iconSquareSideLength; i++){
        QRgb* mapLineOfPixels = (QRgb*) mapImage->scanLine(y + i);
        QRgb* iconLineOfPixels = (QRgb*) scaledImage.scanLine(i);
        for(int k=0; k<iconSquareSideLength; k++){
            if( (*(iconLineOfPixels + k)) >> ((sizeof(int)-1)*8) ) {//checking the alpha byte
                *(mapLineOfPixels + x + k) = *(iconLineOfPixels + k);
            }
        }
    }
}

//create QImage and GraphicsTool for making the map
void MapEditor::initializeMap(void){
    hexesWide = mapData->getHexesWide();//number of hexes wide the map is
    hexesHigh = mapData->getHexesHigh();//number of hexes high the map is

    imageHeight = hexPixelHeight*hexesHigh + hexPixelHeight/2;
    imageWidth = hexPixelWidth + threeQuarterWidth*(hexesWide - 1);

    mapImage = new QImage(imageWidth, imageHeight, QImage::Format_RGB32);
    graphicsTool = new GraphicsTool(mapImage, imageWidth, imageHeight);

    Territory*** pixelTerritoryMapping = new Territory**[imageHeight];
    for(int i=0; i<imageHeight; i++){
        *(pixelTerritoryMapping + i) = new Territory*[imageWidth];
        for(int k=0; k<imageWidth; k++){
            *(*(pixelTerritoryMapping + i) + k) = NULL;
        }
    }
    mapData->setPixelTerritoryMapping(pixelTerritoryMapping);

    graphicsTool->clear(qRgb(255,255,255));
}

//checks if given hexes belong to opposing to territories. Returns true if
//opposed or the second hex has invalid coordinates (first hex is assumed to be
//valid). Returns false otherwise.
bool MapEditor::checkForOpposedTerritory(Territory*** hexGrid, int x0, int y0, int x1, int y1){
    if(x1 < 0 || x1 >= hexesWide || y1 < 0 || y1 >= hexesHigh) {
        return true;
    } else {
        if(hexGrid[y0][x0] == hexGrid[y1][x1]) {
            return false;
        } else {
            return true;
        }
    }
}

void MapEditor::deriveIconCoordinates(int* coordinates){
    double x, y;

    x = hexPixelWidth/2*((ROOT_THREE - 1)/2);
    y = x - hexPixelWidth/2 + hexPixelHeight/2;

    *coordinates = (int) x;
    *(coordinates+1) = (int) y;
}

void MapEditor::setIconCoordinates(int* coordinates, Territory* currentTerritory){
    int count = 0;
    while(currentTerritory->iconLocalesOrganizedByTypeOfIcon[count][0] || currentTerritory->iconLocalesOrganizedByTypeOfIcon[count][1]){
        count++;
        if (count >= 9) return;
    }
    currentTerritory->iconLocalesOrganizedByTypeOfIcon[count][0] = coordinates[0];
    currentTerritory->iconLocalesOrganizedByTypeOfIcon[count][1] = coordinates[1];
}

QImage* MapEditor::getMapImage(void){
    return this->mapImage;
}

MapData* MapEditor::getMapData(void){
    return mapData;
}
