/** \file nullplayer.h
 *  @brief Declarations for the NullPlayer class.
 *  @author Andrew Ring
 */

#ifndef NULLPLAYER_H
#define NULLPLAYER_H

#include "player.h"

/**
 * @brief Singleton class which represents a lack of player.  Used to avoid excessive checks for NULL.
 * @author Andrew Ring
 */
class NullPlayer : public Player
{
private:
    /**
     * @brief Stores the address of the singleton instance.
     */
    static NullPlayer* reusableInstance;

    /**
     * @brief Default constructor.  PRIVATE!
     */
    NullPlayer();
public:
    /**
     * @brief Overridden to not collect anything.  NullPlayer does not hold resources.
     * @return An empty list.
     */
    std::list<Territory*>* collectResources();

    /**
     * @brief Used to get the singleton's address.
     * @return A pointer to the singleton.
     */
    static NullPlayer* getNullPlayer();

    /**
     * @brief Never really used.  Overloaded to protect against extra computation.
     * @param The territory to be attacked.
     * @return 0
     */
    int getOffensiveStrength(Territory* target);
};

#endif // NULLPLAYER_H
