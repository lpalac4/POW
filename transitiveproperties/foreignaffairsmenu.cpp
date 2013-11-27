/** \file foreignaffairsmenu.cpp
 *  @brief Definition of programmatic elements of foreign affairs UI.
 *  @author Charles Myers
 */

#include "foreignaffairsmenu.h"
#include "ui_foreignaffairsmenu.h"

ForeignAffairsMenu *ForeignAffairsMenu::singleInstance = NULL;

ForeignAffairsMenu *ForeignAffairsMenu::getForeignAffairsMenu(void){
    if(!singleInstance){
        singleInstance = new ForeignAffairsMenu(&(Player::allPlayers));
    }

    singleInstance->update(MainWindow::currentPlayer);

    return singleInstance;
}

ForeignAffairsMenu::ForeignAffairsMenu(list<Player*>* players, QWidget *parent) :
    QWidget(parent), ui(new Ui::ForeignAffairsMenu)
{
    ui->setupUi(this);

    this->numberOfPlayers = players->size();
    indicators = new std::vector<PlayerIndicator*>(numberOfPlayers - 1);
    statusBars = new std::vector<QComboBox*>(numberOfPlayers - 1);

    for(int i=0; i<numberOfPlayers-1; i++){
        (*indicators)[i] = new PlayerIndicator(*players);
        (*statusBars)[i] = new QComboBox(this);
        (*statusBars)[i]->addItem("Assist");
        (*statusBars)[i]->addItem("Defend");
        (*statusBars)[i]->addItem("Neutral");
        (*statusBars)[i]->addItem("Oppose");

        //This works - but not on the button portion
        (*statusBars)[i]->setItemData(0, Qt::green, Qt::TextColorRole);
        (*statusBars)[i]->setItemData(1, Qt::yellow, Qt::TextColorRole);
        (*statusBars)[i]->setItemData(2, Qt::gray, Qt::TextColorRole);
        (*statusBars)[i]->setItemData(3, Qt::red, Qt::TextColorRole);

        //This doesnt for some reason
        QPalette newPalette((*statusBars)[i]->palette());
        newPalette.setColor(QPalette::Text, Qt::red);
        (*statusBars)[i]->setPalette(newPalette);
        (*statusBars)[i]->setStyleSheet("color: rgb(255,255,255)");
    }

    QVBoxLayout *topLayout = new QVBoxLayout(this);

    QHBoxLayout *titleRow = new QHBoxLayout;
    titleRow->addStretch();
    QLabel* title = new QLabel("Foreign Affairs");
    title->setStyleSheet("color: rgb(255, 255, 255)");
    titleRow->addWidget(title);
    titleRow->addStretch();
    topLayout->addLayout(titleRow);

    for(int i=0; i<numberOfPlayers - 1; i++){
        QHBoxLayout *newRow = new QHBoxLayout;

        newRow->addStretch();
        newRow->addWidget((*indicators)[i]->getView());
        newRow->addWidget((*statusBars)[i]);
        newRow->addStretch();

        topLayout->addLayout(newRow);
    }

    topLayout->addStretch();
    QHBoxLayout *bottomRow = new QHBoxLayout;
    MyButton *cancelButton = new MyButton(this);
    cancelButton->setText("Cancel");

    bottomRow->addStretch();
    bottomRow->addWidget(cancelButton);
    MyButton *acceptButton = new MyButton(this);
    acceptButton->setText("Accept");
    bottomRow->addWidget(acceptButton);
    bottomRow->setAlignment(acceptButton, Qt::AlignRight);
    bottomRow->setAlignment(cancelButton, Qt::AlignRight);
    topLayout->addLayout(bottomRow);
    this->setStyleSheet("background-color: rgb(0, 0, 0);");
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWithoutUpdate()));
    QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(closeWithUpdate()));
}

void ForeignAffairsMenu::update(Player* currentPlayer){
    list<Player*>::iterator it;
    int index = 0;
    for(it = Player::allPlayers.begin(); it != Player::allPlayers.end(); it++){
        if(currentPlayer == *it){
            continue;
        }
        PlayerIndicator* thisIndicator = (*indicators)[index];
        thisIndicator->setTurn(currentPlayer);
        thisIndicator->highlightPlayer(*it);

        QComboBox* thisBox = (*statusBars)[index];
        int standingAsInt = (int)(currentPlayer->foreignAffairs[*it]);
        thisBox->setCurrentIndex(standingAsInt);

        index++;
    }
}

void ForeignAffairsMenu::closeWithUpdate(void){
    if(inFocus) {
        list<Player*>::iterator it;
        int index = 0;
        for(it = Player::allPlayers.begin(); it != Player::allPlayers.end(); it++){
            Player* subjectPlayer = (*it);
            if(MainWindow::currentPlayer == subjectPlayer) continue;
            ForeignAffairsStanding standingToBeUpdated = (ForeignAffairsStanding)((*statusBars)[index]->currentIndex());
            MainWindow::currentPlayer->foreignAffairs[subjectPlayer] = standingToBeUpdated;
            index++;
        }

        emit foreignAffairsDone();
    }
}

void ForeignAffairsMenu::closeWithoutUpdate(void){
    if(inFocus){
        emit foreignAffairsDone();
    }
}

ForeignAffairsMenu::~ForeignAffairsMenu()
{
    indicators->clear();
    delete indicators;
    statusBars->clear();
    delete statusBars;
    delete singleInstance->layout();
    delete ui;
}
