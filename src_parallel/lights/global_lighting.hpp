#ifndef GLOBAL_LIGHTING_HPP_
#define GLOBAL_LIGHTING_HPP_

/**
 * \file global_lighting.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class GlobalLighting
 */

#include "light.hpp"

/**
 * \class GlobalLighting
 * \brief Class for global illumination
 *
 * Global lighting bears a general light component
 * Derived from Light
 */

class GlobalLighting : public Light
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     */
    GlobalLighting(Color color, float power) : Light(color, power) {}
    ~GlobalLighting() {}
};

#endif
