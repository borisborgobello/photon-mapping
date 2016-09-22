#ifndef OUR_RENDERER_HPP_
#define OUR_RENDERER_HPP_

#include <string>
#include "raytracing/photon_mapping_based.hpp"
#include "parsers/parser_yaml.hpp"

/**
 * \file our_renderer.hpp
 * \brief Declaration of OurRenderer
 * \author B.BORGOBELLO
 */

/**
 * \class OurRenderer
 * \brief The main object managing everything
 */
class OurRenderer
{
public:
    OurRenderer() {} ///< Empty constructor

    void parse_file(std::string filename) ; ///< Parses the given file
    void build_scene()  ;                   ///< Builds the scene of the given file
    void build_photon_tree() ;              ///< Launches the photon-mapping phase
    void raytrace(bool) ;                   ///< Creates an Image corresponding to the created scene (photon-map or normal raytracing)
    void save_to(std::string) ;             ///< Saves the previously created Image into .TGA

private:
    ParserYAML * _parser;                   ///< Contains the ROOT ParserYAML, with the scene
    Scene _scene;                           ///< Contains the created Scene
    PhotonMappingBased * _raytracer;        ///< Contains the raytracer/photon-mapper
    Image * _image;                         ///< Contains an Image of the raytraced scene
};

/**
 * \param filename : absolute or relative path
 * to the root file written in YAML
 */
void OurRenderer::parse_file(std::string filename) {
    _parser = new ParserYAML(filename);
    if (!_parser->is_correct()) exit(EXIT_FAILURE);
    if (!_parser->is_well_formed()) exit(EXIT_FAILURE);
}

/**
 * Creates the scene described in the previously checked
 * YAML file
 */
void OurRenderer::build_scene() {
    _scene = _parser->generate_scene();
    delete _parser;
}

/**
 * Creates the photon map, by launching photons from all the light
 * sources (but GlobalLighting)
 */
void OurRenderer::build_photon_tree()
{
    _raytracer = new PhotonMappingBased(_scene) ;
}

/**
 * \param the_photon_map : true if we want to render the
 * photon-map instead of the complete scene
 *
 * It is possible to call this function to render the
 * photon-map and then call it a second time to render
 * the entire scene
 * Saves the rendered scene into an Image
 */
void OurRenderer::raytrace(bool the_photon_map = false)
{
    if (the_photon_map) {
        Image res = _raytracer->render_photonmap(_scene) ;
        _image = new Image(res.get_res_x(), res.get_res_y()) ;
        (*_image) = res ;
    }
    else {
        Image res = _raytracer->render(_scene) ;
        _image = new Image(res.get_res_x(), res.get_res_y()) ;
        (*_image) = res ;
    }
}

/**
 * \param str : absolute or relative path
 * in which we want to save in the Image (with extension TGA)
 *
 * Saves the previously created Image in a file on the hardrive
 */
void OurRenderer::save_to(std::string str)
{
    _image->save_to_TGA(str.c_str()) ;
}

#endif
