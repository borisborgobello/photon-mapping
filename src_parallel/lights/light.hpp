#ifndef LIGHT_HPP_
#define LIGHT_HPP_

/**
 * \file light.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class Light
 */

#include "../color.hpp"

/**
 * \class Light
 * \brief Base class for all light sources
 */

class Light {

public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     */
    Light(Color color, float power) : _color(color), _power(power) {}

    virtual Color get_color() const { return _color ; } ///< Returns the color of the source
    float get_power() const { return _power ; } ///< Returns the power of the source

protected :
    Color _color; ///< Color of the source
    float _power; ///< Power of the source
};

#endif
