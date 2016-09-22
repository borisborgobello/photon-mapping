/**
 * \file raytracer.hpp
 * \author T.FEIGLER
 * \brief Declaration of abstract class Raytracer
 * \date 12 avril 2010
 *
 * The Raytracer renders a scene
 */

#ifndef RAYTRACER_HPP_
#define RAYTRACER_HPP_

#include "scene.hpp"
#include "image.hpp"

/**
 * \class Raytracer
 * \brief Base class of all renderers that render a scene
 *
 * This one-abstract-method class is the base class of all
 * renderers that render a scene, returning an Image.
 * Base class of PhotonMappingBased.
 */
class Raytracer
{
public:
	/**
	 * \brief The main rendering method for all raytracers
	 * \param scene : the Scene to render
	 * \return an Image
	 */
	virtual Image render(const Scene&) const = 0 ;
};

#endif /* PHOTON_LIST_HPP_ */
