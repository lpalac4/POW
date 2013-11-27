/** \file resources.cpp
 *  @brief Definitions of Resources member functions.
 *  @author Andrew Ring
 */

#include "resources.h"

Resources::Resources()
{
    empty();
}

Resources::Resources(unsigned short stone, unsigned short gold, unsigned short iron, unsigned short crop, unsigned short wood)
{
    this->stone = stone;
    this->gold = gold;
    this->iron = iron;
    this->crop = crop;
    this->wood = wood;
}

bool Resources::equalOrGreaterThan(const Resources *const r)
{
    if(this->stone >= r->stone &&
            this->gold >= r->gold &&
            this->iron >= r->iron &&
            this->crop >= r->crop &&
            this->wood >= r->wood) {
        return true;
    }
    return false;
}

bool Resources::subtract(const Resources *const r)
{
    if(!equalOrGreaterThan(r)) {
        return false;
    }

    stone -= r->stone;
    gold -= r->gold;
    iron -= r->iron;
    crop -= r->crop;
    wood -= r->wood;

    return true;
}

void Resources::add(const Resources *const r)
{
    stone += r->stone;
    gold += r->gold;
    iron += r->iron;
    crop += r->crop;
    wood += r->wood;
}

void Resources::empty()
{
    stone = 0;
    gold = 0;
    iron = 0;
    crop = 0;
    wood = 0;
}

void Resources::takeAll(Resources* r)
{
    add(r);
    r->empty();
}

QString Resources::getStringRepresentation()
{
    std::ostringstream os;
    os << "Crop: " << crop <<
          "  Gold: " << gold <<
          "  Iron: " << iron <<
          "  Stone: " << stone <<
          "  Wood: " << wood;
    return QString::fromStdString(os.str());
}
