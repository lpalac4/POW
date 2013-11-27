/** \file combatprocessor.cpp
 *  @brief Definitions of functions in combat namespace.
 *  @author Andrew Ring
 */

#include "combatprocessor.h"

namespace combat {

    /**
     * @brief Calculates a winner in a conflict between 2 or more players over
     * a territory (owned by one of them).
     *
     * @param contestingPlayers A vector of player pointers.  Index 0 should always be
     * the defender, and there should always be at least one attacker.
     *
     * @param contestedArea A pointer to the territory being attacked.
     *
     * @return The Player who is victorious and should now be made the owner of the contestedArea.
     * Note: ownership is not changed by resolve().
     */
    Player* resolve(std::vector<Player*>* contestingPlayers, Territory* contestedArea)
    {
        clock_t startTime;
        if(testingFlags::TEST_CALC_RUNTIME) {
            startTime = clock();
        }
        srand ( time(NULL) );

        Player* currentPlayer;
        assert(contestingPlayers->size() >= 2);
        assert(contestedArea->ownedBy == contestingPlayers->at(0));

        if(DEBUG_OUTPUT_ENABLED) {
            qDebug() << "Entering resolve()...";
            qDebug() << "Number of contesting players: " << contestingPlayers->size();
            qDebug() << "Number of adjacent territories: " << contestedArea->adjacentTerritories->size();
            qDebug() << "********************";
        }

        unsigned int i;
        std::map<Player*,int> vals;

        // Get strength of defending terrirory/player.  No allies yet.
        vals[contestingPlayers->at(0)] = contestedArea->getDefensiveStrength();

        if(DEBUG_OUTPUT_ENABLED) {
            qDebug() << "Defender strength (no allies): " << vals[contestingPlayers->at(0)];
        }

        // Get offensive strength of each attacking player.  No allies yet.
        for(i = 1; i < contestingPlayers->size(); i++) {
            currentPlayer = contestingPlayers->at(i);
            assert(currentPlayer != 0);
            vals[currentPlayer] = currentPlayer->getOffensiveStrength(contestedArea);
            if(DEBUG_OUTPUT_ENABLED) {
                qDebug() << "Attacker " << i << " strength (no allies): " << vals[currentPlayer];
            }
        }

        addAllyStrengths(contestingPlayers, contestedArea, vals);

        if(DEBUG_OUTPUT_ENABLED) {
            for(i = 1; i < contestingPlayers->size(); i++) {
                // print after ally...
            }
        }

        int total = 0;
        for(i = 0; i < contestingPlayers->size(); i++) {
            currentPlayer = contestingPlayers->at(i);
            total += (vals[currentPlayer] += (rand() % COMBAT_RAND_VARIANCE));
        }
        int winningNumber = (rand() % total);
        for(i = 0; i < contestingPlayers->size(); i++) {
            currentPlayer = contestingPlayers->at(i);
            if(winningNumber < vals[currentPlayer]) {
                if(testingFlags::TEST_CALC_RUNTIME) {
                    double elapsedTime = double(clock() - startTime)/CLOCKS_PER_SEC;
                    qDebug() << "resolve() took " << elapsedTime << " seconds";
                }
                return currentPlayer;
            } else {
                winningNumber -= vals[currentPlayer];
            }
        }
        // THIS SHOULD NEVER HAPPEN!
        return 0;
    }

    /**
     * @brief Analyzes foreign relations policy of neighboring territories to a
     * conflict and adds support as appropriate to vals.
     *
     * @param contestingPlayers A vector of Player pointers.  The first player must be the
     * player owning the territory being contested (i.e. - be the defender).
     *
     * @param contestedArea A pointer to the territory being fought over.
     *
     * @param vals A map of the current players strengths.  This should already be initialized with
     * values for the players involved (even if they're 0).
     */
    void addAllyStrengths(std::vector<Player*>* contestingPlayers, Territory* contestedArea,
                          std::map<Player*,int> &vals) {
        Player* currentPlayer;
        Territory* currentTerritory;

        std::list<Territory*>::iterator territoryIt;
        for(territoryIt=contestedArea->adjacentTerritories->begin();
            territoryIt != contestedArea->adjacentTerritories->end(); ++territoryIt) {

            currentTerritory = (*territoryIt);

            bool twoPlayersOnly = (contestingPlayers->size() == 2);
            bool assisting = true;
            Player* helped = 0;
            int assistAmount = currentTerritory->getStrength();

            // Assume that it isn't one of the contesting players' territory unless proven otherwise.
            if( currentTerritory->ownedBy == contestingPlayers->at(0)) {
                assisting = false;
                break;
            }
            if( currentTerritory->ownedBy != contestingPlayers->at(0) &&
                    (currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(0)] == ASSIST ||
                    currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(0)] == DEFEND)) {
                helped = contestingPlayers->at(0);
            }
            if(twoPlayersOnly && (currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(0)] != OPPOSE &&
                                  currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(1)] == OPPOSE)) {
                helped = contestingPlayers->at(0);
            } else if(twoPlayersOnly && (currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(1)] != OPPOSE &&
                                         currentTerritory->ownedBy->foreignAffairs[contestingPlayers->at(0)] == OPPOSE)) {
                helped = contestingPlayers->at(1);
            }
            unsigned int i;
            for(i = 1; i < contestingPlayers->size() && assisting; i++) {
                currentPlayer = contestingPlayers->at(i);
                if(currentTerritory->ownedBy == currentPlayer) {
                    assisting = false;
                    break;
                }
                if(currentTerritory->ownedBy->foreignAffairs[currentPlayer] == ASSIST) {
                    if(helped == 0 || helped == currentPlayer) {
                        helped = currentPlayer;
                    } else {
                        assisting = false;
                        break;
                    }
                }
            }
            if(assisting && helped != 0) {
                vals[helped] += assistAmount;
            }
        }
    }
}
