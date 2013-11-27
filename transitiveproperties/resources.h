/** \file resources.h
 *  @brief Declaration of Resources class.
 *  @author Andrew Ring
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include <QString>
#include <sstream>
#include <string>
#include <iostream>

/**
 * @brief Structure for holding the number of each type of resources in a group.
 * @author Andrew Ring
 */
class Resources
{
public:
    /**
     * @brief Default constructor.
     */
    Resources();

    /**
     * @brief Constructor with value initialization.
     * @param stone Number of coal at creation.
     * @param gold Number of gold at creation.
     * @param iron Number of iron at creation.
     * @param crop Number of wheat at creation.
     * @param wood Number of wood at creation.
     */
    Resources(unsigned short stone, unsigned short gold, unsigned short iron, unsigned short crop, unsigned short wood);

    /**
     * @brief The number of wood.
     */
    unsigned short wood;
    /**
     * @brief The number of iron.
     */
    unsigned short iron;
    /**
     * @brief The number of stone.
     */
    unsigned short stone;
    /**
     * @brief The number of gold.
     */
    unsigned short gold;
    /**
     * @brief The number of crop.
     */
    unsigned short crop;

    /**
     * @brief Determines if this set of resources conatins >= the number of ALL types of resources.
     * @param r A pointer to the Resources being compared.
     * @return True iff this set of resources >= r's for ALL types of resources, else FALSE.
     */
    bool equalOrGreaterThan(const Resources *const r);
    /**
     * @brief For each type of resource, subtract the amount specified by r.
     * @param r The set of resources to be subtracted.
     * @return False if there are insufficient resources in to subtract, in which case this set is NOT modified. Otherwise True.
     */
    bool subtract(const Resources *const r);
    /**
     * @brief For each type of resource, add the amount specified by r.
     * @param r The set of resources to be added.
     */
    void add(const Resources *const r);
    /**
     * @brief Reset all types to 0.
     */
    void empty();
    /**
     * @brief Add all of the resources contained in r to this set, removing them from r.
     * @param r The set of resources to be moved to this set.
     */
    void takeAll(Resources* r);
    /**
     * @brief Generates a QString representation of the resources contained within.
     * @return A QString representation of the resources.
     */
    QString getStringRepresentation();
};

#endif // RESOURCES_H
