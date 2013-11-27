/** \file nullplayer.cpp
 *  @brief Function definitions for the NullPlayer class.
 *  @author Andrew Ring
 */

#include <QColor>
#include "nullplayer.h"

NullPlayer* NullPlayer::reusableInstance = 0;

NullPlayer::NullPlayer()
{
    name = "NullPlayer";
    playerColorSet = new Color(qRgb(0, 0, 0),  // border color
                               qRgb(76, 76, 76),  // fill color
                               qRgb(166, 166, 166)); // highlight color
    Player::allPlayers.remove(this);
}

int NullPlayer::getOffensiveStrength(Territory* target)
{
    return 0;
    target = target;  // unused warning... I banish thee!
}

NullPlayer* NullPlayer::getNullPlayer()
{
    if(reusableInstance == 0) {
        reusableInstance = new NullPlayer;
    }

    return reusableInstance;
}

std::list<Territory*> *NullPlayer::collectResources()
{
    return NULL;
}
