/**
 * \file parser_yaml.cpp
 * \brief Implementation of class ParserYAML
 * \author B.BORGOBELLO
 */

/**
 * \brief Header (used objects/lights/textures/cameras)
 */
#include "parser_yaml.hpp"

#include <textures/colored.hpp>
#include <textures/checkers.hpp>
#include <textures/bitmap.hpp>

#include <cameras/conic_camera.hpp>
#include <cameras/planar_camera.hpp>

#include <shapes/sphere.hpp>
#include <shapes/parallelepiped.hpp>
#include <shapes/plane.hpp>
#include <shapes/triangle.hpp>

#include <lights/punctual_source.hpp>
#include <lights/hemispherical_source.hpp>
#include <lights/global_lighting.hpp>
#include <lights/radiant_volume.hpp>

#include "global_parameters.hpp"

/**
 * \brief Dynamic allocation of static members of the ParserYAML class
 */
std::vector< ParserYAML *> & ParserYAML::_parser_list = *(new std::vector< ParserYAML *>);

std::vector<std::string> & ParserYAML::_errors = *(new std::vector<std::string>);
std::vector<std::string> & ParserYAML::_warnings = *(new std::vector<std::string>);

std::map <std::string, ParserYAML *> &  ParserYAML::_object_list = *(new std::map <std::string, ParserYAML *>);
std::map <std::string, ParserYAML *> &  ParserYAML::_texture_list = *(new std::map <std::string, ParserYAML *>);
std::map <std::string, ParserYAML *> &  ParserYAML::_light_list = *(new std::map <std::string, ParserYAML *>);
std::map <std::string, ParserYAML *> &  ParserYAML::_camera_list = *(new std::map <std::string, ParserYAML *>);

std::map < std::string, std::map < std::string, std::vector< std::pair <std::string, std::string> > > > & ParserYAML::_matrix_param = *(new std::map < std::string, std::map < std::string, std::vector< std::pair <std::string, std::string> > > >);

/**
 * \brief Creates the tree of parameter for any type of things in the scene
 * it is a practical way to browser parameter and sum them up
 */
void ParserYAML::param_init() {
    using namespace std;

    map <string, vector<pair<string,string> > > current_map;
    vector<pair<string,string> > current_vector;

// OBJECTS

// sphere
    current_vector.push_back(pair<string, string>("texture", "string"));
    current_vector.push_back(pair<string, string>("absorb", "double"));
    current_vector.push_back(pair<string, string>("reflect", "double"));
    current_vector.push_back(pair<string, string>("refract", "double"));
    current_vector.push_back(pair<string, string>("indice", "double"));
    current_vector.push_back(pair<string, string>("center", "vector"));
    current_vector.push_back(pair<string, string>("radius", "double"));

    current_map["Sphere"] = current_vector;
    current_vector.clear();

// parallelepiped
    current_vector.push_back(pair<string, string>("texture", "string"));
    current_vector.push_back(pair<string, string>("absorb", "double"));
    current_vector.push_back(pair<string, string>("reflect", "double"));
    current_vector.push_back(pair<string, string>("refract", "double"));
    current_vector.push_back(pair<string, string>("indice", "double"));
    current_vector.push_back(pair<string, string>("corner", "vector"));
    current_vector.push_back(pair<string, string>("x_vector", "vector"));
    current_vector.push_back(pair<string, string>("y_vector", "vector"));
    current_vector.push_back(pair<string, string>("z_vector", "vector"));

    current_map["Parallelepiped"] = current_vector;
    current_vector.clear();

// plane
    current_vector.push_back(pair<string, string>("texture", "string"));
    current_vector.push_back(pair<string, string>("absorb", "double"));
    current_vector.push_back(pair<string, string>("reflect", "double"));
    current_vector.push_back(pair<string, string>("transparency", "double"));
    current_vector.push_back(pair<string, string>("point", "vector"));
    current_vector.push_back(pair<string, string>("normal", "vector"));

    current_map["Plane"] = current_vector;
    current_vector.clear();

// triangle
    current_vector.push_back(pair<string, string>("texture", "string"));
    current_vector.push_back(pair<string, string>("absorb", "double"));
    current_vector.push_back(pair<string, string>("reflect", "double"));
    current_vector.push_back(pair<string, string>("transparency", "double"));
    current_vector.push_back(pair<string, string>("point1", "vector"));
    current_vector.push_back(pair<string, string>("point2", "vector"));
    current_vector.push_back(pair<string, string>("point3", "vector"));
    current_vector.push_back(pair<string, string>("for_volume", "bool"));

    current_map["Triangle"] = current_vector;
    current_vector.clear();

// reset
    _matrix_param["OBJECTS"] = current_map;
    current_map.clear();



// LIGHTS
// Punctual
    current_vector.push_back(pair<string, string>("color", "vector double"));
    current_vector.push_back(pair<string, string>("power", "float"));
    current_vector.push_back(pair<string, string>("origine", "vector"));

    current_map["Punctual"] = current_vector;
    current_vector.clear();

// Hemispherical
    current_vector.push_back(pair<string, string>("color", "vector double"));
    current_vector.push_back(pair<string, string>("power", "float"));
    current_vector.push_back(pair<string, string>("origine", "vector"));
    current_vector.push_back(pair<string, string>("direction", "vector"));

    current_map["Hemispherical"] = current_vector;
    current_vector.clear();

// GlobalLighting
    current_vector.push_back(pair<string, string>("color", "vector double"));
    current_vector.push_back(pair<string, string>("power", "float"));

    current_map["GlobalLighting"] = current_vector;
    current_vector.clear();

// RadiantVolume
    current_vector.push_back(pair<string, string>("color", "vector double"));
    current_vector.push_back(pair<string, string>("power", "float"));
    current_vector.push_back(pair<string, string>("volume", "string"));

    current_map["RadiantVolume"] = current_vector;
    current_vector.clear();

// Clean

    _matrix_param["LIGHTS"] = current_map;
    current_map.clear();

// CAMERAS
// conic

    current_vector.push_back(pair<string, string>("origine", "vector"));
    current_vector.push_back(pair<string, string>("vector1", "vector"));
    current_vector.push_back(pair<string, string>("vector2", "vector"));
    current_vector.push_back(pair<string, string>("size1", "float"));
    current_vector.push_back(pair<string, string>("size2", "float"));

    current_map["Planar"] = current_map["Conic"] = current_vector;
    current_vector.clear();

    _matrix_param["CAMERAS"] = current_map;
    current_map.clear();

// TEXTURES
// Color

    current_vector.push_back(pair<string, string>("color", "vector"));

    current_map["Color"] = current_vector;
    current_vector.clear();

// Checkers
    current_vector.push_back(pair<string, string>("text_x", "vector"));
    current_vector.push_back(pair<string, string>("text_y", "vector"));
    current_vector.push_back(pair<string, string>("x_dim", "double"));
    current_vector.push_back(pair<string, string>("y_dim", "double"));
    current_vector.push_back(pair<string, string>("even", "vector double"));
    current_vector.push_back(pair<string, string>("odd", "vector double"));

    current_map["Checkers"] = current_vector;
    current_vector.clear();

// Bitmaps
    current_vector.push_back(pair<string, string>("text_x", "vector"));
    current_vector.push_back(pair<string, string>("text_y", "vector"));
    current_vector.push_back(pair<string, string>("x_dim", "double"));
    current_vector.push_back(pair<string, string>("y_dim", "double"));
    current_vector.push_back(pair<string, string>("file", "string"));

    current_map["Bitmap"] = current_vector;
    current_vector.clear();

// Clean
    _matrix_param["TEXTURES"] = current_map;
    current_map.clear();

    return;
}

/**
 * \brief Constructor of ParserYAML
 * Opens the file stream and verify YAML syntax
 * \param filename : file to be opened
 */
ParserYAML::ParserYAML(std::string filename) : _filename(filename)
{
    using namespace std;

    _file.open(filename.c_str());
    if (!_file) {
        cout << "File " << filename << " couldn't be opened" << endl;
        _correct = false;
    }
    else {
        try {
            string str, name;
            YAML::Parser parser(_file);

            parser.GetNextDocument(_root);

            _parser_list.push_back(this);

            if (!_matrix_param.size()) {
                param_init();
            }
            _correct = true;
        } catch(YAML::ParserException& e) {
            cout << "Erreur syntaxique YAML in file " << _filename << endl;
            cout << e.what() << endl;
            _file.close();
            _correct = false;
        }
    }
}

/**
 * \brief Verifies everything, the return value
 * indicated whether it is possible to generate the scene or not
 * This function is called only in the main parser (not recursive)
 */
bool ParserYAML::is_well_formed()
{
    bool well_formed = true;
    using namespace std;
    string str, name;

    cout << "!! STARTING COMPLETE CHECK !!" << endl << endl;

    well_formed &= check_all();

    // Texture checking
    cout << "Now testing texture existence of objects" << endl;

    map<string, ParserYAML *>::iterator it;
    string object_name, texture_name;

    for (it = _object_list.begin(); it != _object_list.end(); it++) {
        well_formed &= it->second->check_texture(it->first);
        cout << endl;
    }
    cout << endl;
    // End texture

    // Volume checking
    cout << "Now testing volume existence of lights" << endl;

    for (it = _light_list.begin(); it != _light_list.end(); it++) {
        well_formed &= it->second->check_volume(it->first);
        cout << endl;
    }
    cout << endl;
    // End volume


    if (!well_formed) {
        cout << "One or more errors, scene of " << _filename << " won't be tested" << endl;
        show_logs();
        return false;
    }

    cout << "ALL FILES SUCCEFULLY FORMED" << endl << endl;

    well_formed &= check_scene();
    show_logs();

    cout << endl;
    cout << "RESULT : " << _filename << ((well_formed) ? " describes a perfect scene\n" : " is NOT a correct scene\n") << endl;
    return well_formed;
}

/**
 * \brief Constructor of ParserYAML
 * Verifies the dimensions (3) of a vector
 * \param vector : the vector to be tested
 */
bool ParserYAML::check_vector(const YAML::Node& vector)
{
    if (vector.size() != 3)
        return false;
    int test;
    vector[0] >> test;
    vector[1] >> test;
    vector[2] >> test;
    return true;
}

/**
 * \brief Verifies the texture attached to an object
 * really exists somewhere in the files
 * \param object_name : the concerned object's name
 */
bool ParserYAML::check_texture(std::string object_name) {
    using namespace std;
    string texture_name;

    cout << "=> " << object_name;

    object_name = "-" + object_name;
    _root["OBJECTS"][object_name]["texture"] >> texture_name;

    cout << " with texture " << texture_name;

    map<string, ParserYAML *>::iterator it;

    for (it = _texture_list.begin(); it != _texture_list.end(); it++) {
        if (texture_name == it->first) {
            cout << "\t found";
            return true;
        }
    }
    _errors.push_back("Error (" + _filename + ") : Couldn't find texture " + texture_name + " for OBJECTS->" + object_name.substr(1));
    cout << "\tnot found";
    return false;
}

/**
 * \brief Verifies that the given object is a volume
 * \param object_name : the concerned object's name
 */
bool ParserYAML::is_volume(std::string object_name) {
    using namespace std;
    string type;
    _root["OBJECTS"]["-" + object_name]["type"] >> type;

    if (type == "Sphere" || type == "Parallelepiped") return true;
    else {
        _errors.push_back("Error (" + _filename + ") : OBJECT " + object_name + " is not a volume !");
        return false;
    }
}

/**
 * \brief Verifies the volume attached to a light
 * really exists somewhere in the files
 * \param light_name : the concerned light's name
 */
bool ParserYAML::check_volume(std::string light_name) {
    using namespace std;
    string volume_name;

    cout << "=> " << light_name;

    light_name = "-" + light_name;

    if (!_root["LIGHTS"][light_name].FindValue("volume")) {
        cout << " not a radiant volume";
        return true;
    }
    _root["LIGHTS"][light_name]["volume"] >> volume_name;

    cout << " with volume " << volume_name;

    map<string, ParserYAML *>::iterator it;

    for (it = _object_list.begin(); it != _object_list.end(); it++) {
        if (volume_name == it->first) {
            cout << "\t found";
            if (it->second->is_volume(volume_name))
                return true;
            else {
                cout << "\t but not a volume !";
                return false;
            }
        }
    }
    _errors.push_back("Error (" + _filename + ") : Couldn't find volume " + volume_name + " for LIGHTS->" + light_name.substr(1));
    cout << "\tnot found";
    return false;
}

/**
 * \brief This function opens the research of items
 * to new files specified in the LOADING section
 */
bool ParserYAML::check_loadings()
{
    using namespace std;
    string str;
    bool well_formed = true;

    if (!_root.FindValue("LOADING")) {
        _warnings.push_back("Info in " + _filename +" : No section LOADING");
        return true;
    }
    else {
        const YAML::Node& loadings_list = _root["LOADING"];
        string current_loading;

        unsigned int i;
        if (!loadings_list.size()) {
            _warnings.push_back("Info (" + _filename + ") : No loading file has been detected\nin the section LOADING of file\n" + _filename
            + "It might be because of a missing space between (-) and the file name");
            return true;
        }
        else {
            ParserYAML * pars;
            for (i = 0; i < loadings_list.size(); i++) {

                loadings_list[i] >> current_loading;

                if (already_searched(current_loading)) {
                    continue;
                }

                pars = new ParserYAML(current_loading);
                if (pars->is_correct()) {
                    cout << endl;
                    well_formed &= pars->check_all();
                }
                else delete pars;
            }
        }
    }
    return well_formed;
}

/**
 * \brief This function verifies that given a
 * section, everything is in order (parameters...)
 * \param section : the concerned section (OBJECTS,LIGHTS,...)
 */
bool ParserYAML::check_section(std::string section)
{
    using namespace std;
    string str;
    bool well_formed = true;
    bool correct_thing;
    bool all_param_ok;
    string thing_type, param_name, param_type;
    vector<pair <string, string> > params;

    cout << "Checking " << lower(section) << " of " << _filename << endl;

    if (!_root.FindValue(section)) {
        _warnings.push_back("Info (" + _filename + ") : No section " + section);
        cout << "No section" << endl;
        return true;
    }
    else {
        const YAML::Node& things_list = _root[section];

        string temp_thing;
        unsigned int i=0;

        for (YAML::Iterator it = things_list.begin(); it != things_list.end(); it++, i++) {

            correct_thing = true;
            it.first() >> temp_thing;
            if (temp_thing[0] != '-') {
                _errors.push_back("Error (" + _filename + ") : " + section + "->" + temp_thing + " must be started with a (-)");
                correct_thing = false;
                cout << "Missing (-)\t";
            }
            else temp_thing = temp_thing.substr(1);

            cout << "=> " << temp_thing << "\t";
            const YAML::Node& my_thing = it.second();

            if (!my_thing.FindValue(str = "type")) {
                _errors.push_back("Error (" + _filename + ") : No type specified for " + section + "->" + temp_thing);
                correct_thing = false;
                cout << "ERR" << endl;
            }
            else {
                all_param_ok = true;
                my_thing["type"] >> thing_type;
                params = _matrix_param[section][thing_type];

                if (!params.size()) {
                    _errors.push_back("Error (" + _filename + ") : Couldn't find any set of parameters (type " + thing_type + " not recognized) in " + section + "->" + temp_thing);
                    all_param_ok = false;
                }

                for (i = 0; i< params.size(); i++) {
                    if (!my_thing.FindValue(param_name = params[i].first)) {
                        _errors.push_back("Error (" + _filename + ") : parameter " + param_name + " of " + section + "->" + temp_thing + "(" + thing_type + ") not found");
                        all_param_ok = false;
                    }
                    else {
                        param_type = params[i].second;
                        if ( (param_type == "vector" || param_type == "vector double") && !check_vector(my_thing[param_name])) {
                            _errors.push_back("Error (" + _filename + ") : parameter " + param_name + " of " + section + "->" + temp_thing + "(" + thing_type + ") is not a correct vector");
                            all_param_ok = false;
                        }
                        else if (param_type == "float" || param_type == "double") {
                            double temp;
                            my_thing[param_name] >> temp; // Will exit(EXIT_FAILURE) if incoming string is not a double/float format
                        }
                        else if (param_type == "int") {
                            int temp;
                            my_thing[param_name] >> temp; // Will exit(EXIT_FAILURE) if incoming string is not an int format
                        }
                        else if (param_type == "bool") {
                            bool temp;
                            my_thing[param_name] >> temp; // Will exit(EXIT_FAILURE) if incoming string is not an int format
                        }
                    }
                }
                correct_thing &= all_param_ok;
                if (!all_param_ok) cout << "Wrong parameter(s)";

                if (correct_thing) {
                    if (section == "OBJECTS") _object_list[temp_thing] = this;
                    else if (section == "TEXTURES") _texture_list[temp_thing] = this;
                    else if (section == "LIGHTS") _light_list[temp_thing] = this;
                    else if (section == "CAMERAS") _camera_list[temp_thing] = this;

                    cout << "OK";
                }
                well_formed &= correct_thing;
                cout << endl;
            }
        }
        cout << "\t";
    }
    cout << endl;
    return well_formed;
}

/**
 * \brief Verifies that the scene specified in the main file
 * really describes a scene, and that everything was found in the files
 */
bool ParserYAML::check_scene()
{
    bool well_formed = true;
    using namespace std;
    string str, name;

    // Vérification de la scene
    cout << "Final check : SCENE of " << _filename << endl << endl;
    // Resolution
    cout << "Resolution" << "\t";
    if (!_root.FindValue("SCENE")) {
        cout << "Error (" + _filename + ") : No scene section" << endl;
        return false;
    }

    const YAML::Node& subsection = _root["SCENE"];
    if (!subsection.FindValue("resolution")) {
        _warnings.push_back("Warning (" + _filename + ") : No resolution => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        const YAML::Node& resolution = subsection["resolution"];
        if (resolution.size() != 2) {
            str = "Error (" + _filename + ")";
            str += " : " + resolution.size();
            str += " dimensions in the resolution";
            _errors.push_back(str);
            well_formed = false;
            cout << "ERR" << endl;
        }
        else if ((int)resolution[0] <= 0 || (int)resolution[1] <= 0) {
            _errors.push_back("Error (" + _filename + ") : Negative resolutions");
            well_formed = false;
            cout << "ERR" << endl;
        }
        else cout << "OK" << endl;
    }

    // Supersampling
    cout << "Supersampling" << "\t";
    if (!subsection.FindValue("supersampling")) {
        _warnings.push_back("Warning (" + _filename + ") : supersampling (anti-aliasing) not declared => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        bool temp;
        subsection["supersampling"] >> temp;
        cout << "OK" << endl;
    }

    // nb_photon_MAX
    cout << "nb_photon_MAX" << "\t";
    if (!subsection.FindValue("nb_photon_MAX")) {
        _warnings.push_back("Warning (" + _filename + ") : nb_photon_MAX not declared => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        int temp;
        subsection["nb_photon_MAX"] >> temp;
        cout << "OK" << endl;
    }

    // nb_photon_to_find
    cout << "nb_photon_to_find" << "\t";
    if (!subsection.FindValue("nb_photon_to_find")) {
        _warnings.push_back("Warning (" + _filename + ") : nb_photon_to_find not declared => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        int temp;
        subsection["nb_photon_to_find"] >> temp;
        cout << "OK" << endl;
    }

    // photon_depth
    cout << "photon_depth" << "\t";
    if (!subsection.FindValue("photon_depth")) {
        _warnings.push_back("Warning (" + _filename + ") : photon_depth not declared => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        int temp;
        subsection["photon_depth"] >> temp;
        cout << "OK" << endl;
    }

    // raytracer_depth
    cout << "raytracer_depth" << "\t";
    if (!subsection.FindValue("raytracer_depth")) {
        _warnings.push_back("Warning (" + _filename + ") : raytracer_depth not declared => using default values");
        cout << "OK (default)" << endl;
    }
    else {
        int temp;
        subsection["raytracer_depth"] >> temp;
        cout << "OK" << endl;
    }

    cout << endl;

    // camera
    cout << "CAMERA" << endl;
    if (!subsection.FindValue("camera")) {
        _errors.push_back("Error (" + _filename + "/scene) : no specified camera for scene");
        well_formed = false;
        cout << "ERR";
    }
    else {
        string camera_name;
        const YAML::Node& cameras_list_scene = subsection["camera"];

        if (cameras_list_scene.size() != 0) {
            _errors.push_back("Error (" + _filename + "/scene) : more than one camera specified");
            well_formed = false;
            cout << "ERR" << endl;
        }
        else {
            subsection["camera"] >> camera_name;
            cout << "==> Looking for " << camera_name;

            if (_camera_list[camera_name] != NULL)
                cout << "\tExists";
            else {
                cout << "\tNot Found";
                _errors.push_back("Error (" + _filename + "/scene) : Camera " + camera_name + " not found");
                well_formed = false;
            }
        }
    }
    cout << endl << endl;

    // Objects
    cout << "OBJECTS\t";
    if (!subsection.FindValue("objects")) {
        _errors.push_back("Error (" + _filename + "/scene) : no objects found");
        well_formed = false;
        cout << "ERR" << endl;
    }
    else {
        const YAML::Node& objects_list_scene = subsection["objects"];
        string current_object;

        cout << objects_list_scene.size() << endl;

        // For each object present on the scene
        for (unsigned int j=0; j < objects_list_scene.size(); j++) {
            objects_list_scene[j] >> current_object;
            cout << "==> Looking for " << current_object;


            if (_object_list[current_object] != NULL)
                cout << "\tExists";
            else {
                cout << "\tNot Found";
                _errors.push_back("Error (" + _filename + "/scene) : Object " + current_object + " not found");
                well_formed = false;
            }
            cout << endl;
        }
    }

    // Lights
    cout << "LIGHTS\t";
    if (!subsection.FindValue("lights")) {
        _errors.push_back("Error (" + _filename + "/scene) : no light specified");
        well_formed = false;
        cout << "ERR" << endl;
    }
    else {
        const YAML::Node& lights_list_scene = subsection["lights"];
        string current_light;

        // For each light present on the scene
        cout << lights_list_scene.size() << endl;

        for (unsigned int j=0; j < lights_list_scene.size(); j++) {
            lights_list_scene[j] >> current_light;
            cout << "==> Looking for " << current_light;

            if (_light_list[current_light] != NULL)
                cout << "\tExists";
            else {
                cout << "\tNot Found";
                _errors.push_back("Error (" + _filename + "/scene) : Object " + current_light + " not found");
                well_formed = false;
            }
            cout << endl;
        }
    }
    cout << endl << "End of scene checking" << endl << endl;
    return well_formed;
}

/**
 * \brief This is the principal exploration function
 * It browses every section and even the other files specified
 * in the LOADING section
 */
bool ParserYAML::check_all()
{
    bool well_formed = true;
    using namespace std;

    cout << "Checking ALL in " << _filename << endl << endl;

    well_formed &= check_section("OBJECTS");
    well_formed &= check_section("TEXTURES");
    well_formed &= check_section("LIGHTS");
    well_formed &= check_section("CAMERAS");
    well_formed &= check_loadings();

    return well_formed;
}

/**
 * \brief Creates the scene corresponding to the SCENE section
 * described in the file
 * This function must only be used after a successfull call to is_well_formed()
 */
Scene ParserYAML::generate_scene()
{
    using namespace std;
    Scene scene;
    boost::shared_ptr<Camera> camera;
    string camera_name, texture_name, object_name, light_name;

    cout << endl << "!!NOW STARTING SCENE GENERATION!!" << endl << endl;

// GlobalParameters
    cout << "Adding global parameters" << endl;
    GlobalParameters * global_param = GlobalParameters::get_unique_instance();
    if (_root["SCENE"].FindValue("resolution")) {
        global_param->set_res_x(_root["SCENE"]["resolution"][0]);
        global_param->set_res_y(_root["SCENE"]["resolution"][1]);
    }
    if (_root["SCENE"].FindValue("supersampling")) global_param->set_supersampling(_root["SCENE"]["supersampling"]);
    if (_root["SCENE"].FindValue("nb_photon_MAX")) global_param->set_nb_photon_MAX(_root["SCENE"]["nb_photon_MAX"]);
    if (_root["SCENE"].FindValue("photon_depth")) global_param->set_photon_depth(_root["SCENE"]["photon_depth"]);
    if (_root["SCENE"].FindValue("raytracer_depth")) global_param->set_raytracer_depth(_root["SCENE"]["raytracer_depth"]);
    if (_root["SCENE"].FindValue("nb_photon_to_find")) global_param->set_nb_photon_to_find(_root["SCENE"]["nb_photon_to_find"]);

    cout << "Resolution X : " << global_param->get_res_x() << endl;
    cout << "Resolution Y : " << global_param->get_res_y() << endl;
    cout << "Supersampling : " << global_param->get_supersampling() << endl;
    cout << "nb_photon_MAX : " << global_param->get_nb_photon_MAX() << endl;
    cout << "nb_photon_to_find : " << global_param->get_nb_photon_to_find() << endl;
    cout << "photon_depth : " << global_param->get_photon_depth() << endl;
    cout << "raytracer_depth : " << global_param->get_raytracer_depth() << endl;


    cout << endl;


// Camera
    _root["SCENE"]["camera"] >> camera_name;
    cout << "Adding camera\t" << camera_name;
    scene.add_camera(_camera_list[camera_name]->create_camera(camera_name));
    cout << endl;

// Textures
    map<string, Texture *> texture_map;
    map<string, Shape *> object_map;
    boost::shared_ptr<Shape> temp_shape;
    for (unsigned int i = 0; i < _root["SCENE"]["objects"].size(); i++) {
        _root["SCENE"]["objects"][i] >> object_name;
        cout << "Adding object\t" << object_name;
        scene.add_shape(temp_shape = _object_list[object_name]->create_object(object_name, texture_map));
        object_map[object_name] = temp_shape.get();
        cout << endl;
    }

// Lights
    for (unsigned int i = 0; i < _root["SCENE"]["lights"].size(); i++) {
        _root["SCENE"]["lights"][i] >> light_name;
        cout << "Adding light\t" << light_name;
        scene.add_light(_light_list[light_name]->create_light(light_name, object_map));
        cout << endl;
    }

    cout << endl << "SCENE CREATED SUCCESSFULLY" << endl << endl;

    return scene;
}

/**
 * \brief Creates the texture corresponding
 * to texture_name
 * It must be sure when calling this function that the texture
 * lies in this file
 * \param texture_name : the texture's name
 */
Texture * ParserYAML::create_texture(std::string texture_name)
{
    using namespace std;
    string texture_type;

    Texture * texture;
    const YAML::Node & sub_root = _root["TEXTURES"]["-"+texture_name];

    sub_root["type"] >> texture_type;
    //cout << " using " << texture_name << " (type " << texture_type << ")" << endl;

    if (texture_type == "Color") {
        texture = new Colored(Color(sub_root["color"][0], sub_root["color"][1], sub_root["color"][2]));
    }
    else if (texture_type == "Checkers") {
        Point3D text_x(sub_root["text_x"][0], sub_root["text_x"][1], sub_root["text_x"][2]);
        Point3D text_y(sub_root["text_y"][0], sub_root["text_y"][1], sub_root["text_y"][2]);
        Color even(sub_root["even"][0],sub_root["even"][1],sub_root["even"][2]);
        Color odd(sub_root["odd"][0],sub_root["odd"][1],sub_root["odd"][2]);
        texture = new Checkers(text_x, text_y, sub_root["x_dim"], sub_root["y_dim"], even, odd);
    }
    else if (texture_type == "Bitmap") {
        Point3D text_x(sub_root["text_x"][0], sub_root["text_x"][1], sub_root["text_x"][2]);
        Point3D text_y(sub_root["text_y"][0], sub_root["text_y"][1], sub_root["text_y"][2]);

        texture = new Bitmap(text_x, text_y, sub_root["x_dim"], sub_root["y_dim"], sub_root["file"]);
    }
    else {
        cout << endl << "CRITICAL FAILURE : It appears the type of item isn't specified for scene generation" << endl;
        exit(EXIT_FAILURE);
    }
    return texture;
}

/**
 * \brief Creates the shape corresponding
 * to object_name
 * It must be sure when calling this function that the object
 * lies in this file
 * Also calls the create_texture() function to allocate its texture
 * \param object_name : the object's name
 * \param texture_map : a map of already created texture not to create useless duplicates.
 */
boost::shared_ptr<Shape> ParserYAML::create_object(std::string object_name, std::map<std::string, Texture *>& texture_map)
{
    using namespace std;
    string object_type;
    Texture * texture;
    boost::shared_ptr<Shape> object;

    const YAML::Node & sub_root = _root["OBJECTS"]["-"+object_name];
    sub_root["type"] >> object_type;

    // Creating texture
    string texture_name = sub_root["texture"];
    cout << " of type " << object_type << " using " << texture_name;

    if (texture_map[texture_name] == NULL) {
        texture = _texture_list[texture_name]->create_texture(texture_name);
        texture_map[texture_name] = texture;
    }
    else {
        if (dynamic_cast<Procedural*>(texture_map[texture_name])) {
            cout << endl << "CRITICAL FAILURE : " << texture_name << " (procedural) : procedural textures can't be shared by two or more objects" << endl;
            exit(EXIT_FAILURE);
        }
        else {
            texture = texture_map[texture_name];
        }
    }
    // End texture

    if (object_type == "Sphere") {
        double absorb = sub_root["absorb"];
        double reflect = sub_root["reflect"];
        double refract = sub_root["refract"];
        double indice = sub_root["indice"];
        Point3D center(sub_root["center"][0], sub_root["center"][1], sub_root["center"][2]);
        double radius = sub_root["radius"];

        object = boost::shared_ptr<Shape>(new Sphere(absorb, reflect, refract, indice, texture, center, radius));
    }
    else if (object_type == "Parallelepiped") {
        double absorb = sub_root["absorb"];
        double reflect = sub_root["reflect"];
        double refract = sub_root["refract"];
        double indice = sub_root["indice"];
        Point3D corner(sub_root["corner"][0], sub_root["corner"][1], sub_root["corner"][2]);
        Point3D x_vector(sub_root["x_vector"][0], sub_root["x_vector"][1], sub_root["x_vector"][2]);
        Point3D y_vector(sub_root["y_vector"][0], sub_root["y_vector"][1], sub_root["y_vector"][2]);
        Point3D z_vector(sub_root["z_vector"][0], sub_root["z_vector"][1], sub_root["z_vector"][2]);

        object = boost::shared_ptr<Shape>(new Parallelepiped(absorb, reflect, refract, indice, texture, corner, x_vector, y_vector, z_vector));
    }
    else if (object_type == "Plane") {
        double absorb = sub_root["absorb"];
        double reflect = sub_root["reflect"];
        double transparency = sub_root["transparency"];
        Point3D point(sub_root["point"][0], sub_root["point"][1], sub_root["point"][2]);
        Vector3D normal(sub_root["normal"][0], sub_root["normal"][1], sub_root["normal"][2]);

        object = boost::shared_ptr<Shape>(new Plane(absorb, reflect, transparency, texture, point, normal));
    }
    else if (object_type == "Triangle") {
        double absorb = sub_root["absorb"];
        double reflect = sub_root["reflect"];
        double transparency = sub_root["transparency"];
        bool for_volume = sub_root["for_volume"];
        Point3D point1(sub_root["point1"][0], sub_root["point1"][1], sub_root["point1"][2]);
        Point3D point2(sub_root["point2"][0], sub_root["point2"][1], sub_root["point2"][2]);
        Point3D point3(sub_root["point3"][0], sub_root["point3"][1], sub_root["point3"][2]);

        object = boost::shared_ptr<Shape>(new Triangle(absorb, reflect, transparency, texture, point1, point2, point3, for_volume));
    }
    else {
        cout << endl << "CRITICAL FAILURE : It appears the type of item isn't specified for scene generation" << endl;
        exit(EXIT_FAILURE);
    }
    return object;
}

/**
 * \brief Creates the light corresponding
 * to light_name
 *
 * It must be sure when calling this function that the light
 * lies in this file
 *
 * \param light_name : the light's name
 * \param object_list : map of the previously created objects for
 * RadiantVolumes
 */
boost::shared_ptr<Light> ParserYAML::create_light(std::string light_name, std::map<std::string, Shape *>& object_list)
{
    using namespace std;
    string light_type;
    boost::shared_ptr<Light> light;

    const YAML::Node & sub_root = _root["LIGHTS"]["-"+light_name];
    sub_root["type"] >> light_type;
    cout << " of type " << light_type;

    Color color(sub_root["color"][0], sub_root["color"][1], sub_root["color"][2]);
    float power = sub_root["power"];

    if (light_type == "Punctual") {
        Point3D origine(sub_root["origine"][0], sub_root["origine"][1], sub_root["origine"][2]);
        light = boost::shared_ptr<Light>(new PunctualSource(color, power, origine));
    }
    else if (light_type == "Hemispherical") {
        Point3D origine(sub_root["origine"][0], sub_root["origine"][1], sub_root["origine"][2]);
        Vector3D direction(sub_root["direction"][0], sub_root["direction"][1], sub_root["direction"][2]);
        //light = boost::shared_ptr<Light>(new PunctualSource(color, power, origine));
        light = boost::shared_ptr<Light>(new HemisphericalSource(color, power, origine, direction));
    }
    else if (light_type == "GlobalLighting") {
        light = boost::shared_ptr<Light>(new GlobalLighting(color, power));
    }
    else if (light_type == "RadiantVolume") {
        string volume_name = sub_root["volume"];
        cout << " with " << volume_name;
        Volume * volume = dynamic_cast<Volume*>(object_list[volume_name]);
        if (volume == NULL) {
            cout << endl << "ERROR CRITICAL : " << volume_name << " must be included in the scene description" << endl;
            exit(EXIT_FAILURE);
        }
        light = boost::shared_ptr<Light>(new RadiantVolume(color, power, *volume));
    }
    else {
        cout << endl << "CRITICAL FAILURE : It appears the type of item isn't specified for scene generation" << endl;
        exit(EXIT_FAILURE);
    }
    return light;
}

/**
 * \brief Creates the camera corresponding
 * to camera_name
 * It must be sure when calling this function that the camera
 * lies in this file
 * \param camera_name : the camera's name
 */
boost::shared_ptr<Camera> ParserYAML::create_camera(std::string camera_name)
{
    using namespace std;
    string camera_type;
    boost::shared_ptr<Camera> camera;

    const YAML::Node & sub_root = _root["CAMERAS"]["-"+camera_name];
    sub_root["type"] >> camera_type;
    cout << " of type " << camera_type;

    Point3D origine(sub_root["origine"][0], sub_root["origine"][1], sub_root["origine"][2]);
    Point3D vector1(sub_root["vector1"][0], sub_root["vector1"][1], sub_root["vector1"][2]);
    Point3D vector2(sub_root["vector2"][0], sub_root["vector2"][1], sub_root["vector2"][2]);
    float size1 = sub_root["size1"];
    float size2 = sub_root["size2"];

    if (camera_type == "Conic") {
        camera = boost::shared_ptr<Camera>(new ConicCamera(origine, vector1, vector2, size1, size2));
    }
    else if (camera_type == "Planar") {
        camera = boost::shared_ptr<Camera>(new PlanarCamera(origine, vector1, vector2, size1, size2));
    }
    else {
        cout << endl << "CRITICAL FAILURE : It appears the type of item isn't specified for scene generation" << endl;
        exit(EXIT_FAILURE);
    }
    return camera;
}

