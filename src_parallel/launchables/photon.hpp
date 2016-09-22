#ifndef PHOTON_HPP_
#define PHOTON_HPP_

/**
 * \file photon.hpp
 * \brief Description of class Photon
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include "launchables/launchable.hpp"
#include "color.hpp"

/**
 * \class Photon
 * \brief Derived class of Launchable with a color
 *
 * Class Photon is a launchable with a color
 */
class Photon : public Launchable
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Photon
	 *
	 * \param point : initial position of the Photon
	 * \param vector : initial direction of the Photon
	 * \param color : initial color of the Photon
	 */
	Photon( Point3D point, Vector3D vector, Color color ) :
		Launchable(point, vector), _color(color) {}

	Color get_color() const { return _color ; } ///< Returns the color of the Photon
	void set_color(Color c) { _color = c ; }    ///< Changes the colors

private:
	Color _color ; ///< Color of the Photon
};

#endif /* PHOTON_HPP_ */
