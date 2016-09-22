#ifndef COLOR_HPP_
#define COLOR_HPP_

/**
 * \file color.hpp
 * \brief Declaration of class Color
 * \author T.FEIGLER
 *
 * Description of class Color
 */


/**
 * \class Color
 * \brief Describes a color (a mix of Red/Green/Blue)
 */
class Color
{
public:
	/**
	 * \brief Constructor of class Color
	 *
	 * \param r : red component
	 * \param g : green component
	 * \param b : blue component
	 */
	Color(double r, double g, double b) :
		_r(r), _g(g), _b(b) {}

	double get_r() const { return _r ; } ///< Returns the red component
	double get_g() const { return _g ; } ///< Returns the green component
	double get_b() const { return _b ; } ///< Returns the blue component
protected:

private:
	double _r, _g, _b ; ///< Red/Green/Blue components
};

#endif /* COLOR_HPP_ */
