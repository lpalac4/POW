/** \file claimcomparator.h
 *  @brief Declaration of comparator for territory selection priority queue.
 *  @author Charles Myers
 */

#ifndef CLAIMCOMPARATOR_H
#define CLAIMCOMPARATOR_H

#include <list>
#include <map>

#include "player.h"
#include "territory.h"

class ClaimComparator
{

private:
    Player* chooser;

    static const double RESOURCE_WEIGHT = 2.1;

public:
    ClaimComparator(Player* chooser);
    
    bool operator() (Territory* a, Territory* b);

signals:
    
public slots:
    
};

#endif // CLAIMCOMPARATOR_H
