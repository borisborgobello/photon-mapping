#ifndef SCENE_HPP_
#define SCENE_HPP_

/**
 * \file scene.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class Scene
 */

#include <vector>
#include <boost/shared_ptr.hpp>
#include <cameras/camera.hpp>
#include <shapes/shape.hpp>
#include "lights/light.hpp"

/**
 * \class Scene
 * \brief Entirely describes a scene (lights/objects/camera)
 *
 * It contains a list of lights, a list of shapes and a camera
 *
 */
class Scene
{
public:
    /**
    * \brief Basic constructor of class Scene
    */
    Scene() {}
    /**
	 * \brief Complete constructor of class Scene
	 *
	 * \param camera : the camera of the scene
	 * \param shape_list : the list of shapes in the scene
	 * \param light_list : the list of lights in the scene
	 */
    Scene(boost::shared_ptr<Camera> camera,
    		std::vector< boost::shared_ptr<Shape> > shape_list,
    		std::vector< boost::shared_ptr<Light> > light_list) :
        _camera(camera), _shape_list(shape_list), _light_list(light_list) {}

    void add_camera(boost::shared_ptr<Camera> camera) {_camera = camera; } ///< Add/change the Camera of this scene
    void add_shape(boost::shared_ptr<Shape> shape) {_shape_list.push_back(shape); } ///< Adds a new Shape to this scene
    void add_light(boost::shared_ptr<Light> light) {_light_list.push_back(light); } ///< Adds a new Light to this scene

    const boost::shared_ptr<Camera>& get_camera() const ///< Returns the camera of this scene
    {
    	return _camera;
    }

    const std::vector< boost::shared_ptr<Shape> >& get_shape_list() const ///< Returns a list of all the shapes of this scene
    {
    	return _shape_list;
    }

    const std::vector< boost::shared_ptr<Light> >& get_light_list() const ///< Returns a list of all the lights of this scene
    {
    	return _light_list;
    }

private:
    boost::shared_ptr<Camera> _camera; ///< The camera of this scene
    std::vector< boost::shared_ptr<Shape> > _shape_list; ///< A list of shapes
    std::vector< boost::shared_ptr<Light> > _light_list; ///< A list of lights
};

#endif
