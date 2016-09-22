#ifndef PARSER_YAML_HPP_
#define PARSER_YAML_HPP_

/**
 * \file parser_yaml.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of the ParserYAML class
 *
 * Header of ParserYAML
 */

#include "scene.hpp"
#include <yaml.h>
#include <string>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>


/**
 * \class ParserYAML
 * \brief Class reading a YAML file
 *
 * Class ParserYAML is oriented recursively
 * it is capable of reading YAML formatted files
 * verify scenes, and create scenes
 */

class ParserYAML
{
public :
    ParserYAML(std::string); ///< Constructor

/**
 * \brief Frees memory allocated dynamically through
 * recursive creations of several ParserYAML.
 */
    ~ParserYAML() {
        _file.close();
        for (int i = _parser_list.size()-1; i >= 0; i--) {
            if (_parser_list[i] == this && i == 0) { // Final delete
                _parser_list.clear();
                _errors.clear();
                _warnings.clear();
                _object_list.clear();
                _light_list.clear();
                _texture_list.clear();
                _camera_list.clear();
                _matrix_param.clear();
            }
            else if (_parser_list[i] == this) {
                _parser_list.pop_back();
                break;
            }
            else
                delete _parser_list[i];
        }
    }

    bool is_correct() { return _correct;} ///< Returns whether or not the creation of the parser was successful and the syntaxe was correct
    std::string get_filename() {return _filename;} ///< Returns the file being parsed by this parser
    virtual Scene generate_scene(); ///< Creates the Scene described in the file

    virtual bool is_well_formed(); ///< Verifies the parameters of everything in the files (recursively)
    bool check_all(); ///< Verifies everything but SCENE section in the file
    bool check_section(std::string); ///< Verifies a section
    bool check_loadings(); ///< Browse new files specified in the LOADING section
    bool check_texture(std::string object_name); ///< Verifies the texture of a given object exists
    bool check_volume(std::string light_name); ///< Verifies the volume of a given light exists
    bool is_volume(std::string object_name); ///< Returns whether a shape is a volume or not

    bool check_scene(); ///< Verifies the scene of the file (MASTER file)

    bool check_vector(const YAML::Node& vector); ///< Verifies that a given vector (parameter) is really a vector
//
/**
 * \brief Shows all errors/warnings
 */
    void show_logs() {
        using namespace std;
        cout << "ERRORS/WARNINGS/INFO" << endl << endl;
        for (unsigned int i=0; i<_errors.size(); i++) {
            cout << _errors[i];
            cout << endl;
        }
        for (unsigned int i=0; i<_warnings.size(); i++) {
            cout << _warnings[i];
            cout << endl;
        }
    }
/**
 * \brief Lowers a given string
 * \param str : String to lower
 */
    std::string lower(std::string str) {
        for (unsigned int i = 0; i < str.size(); i++) {
            str[i] = tolower(str[i]);
        }
        return str;
    }
/**
 * \brief Prevents recursive calls from deadlocking
 * \param filename : the file to be opened next
 */
    static bool already_searched(std::string filename)
    {
        for (unsigned int i = 0; i < _parser_list.size(); i++) {
            if (_parser_list[i]->get_filename() == filename) return true;
        }
        return false;
    }

    Texture * create_texture(std::string); ///< Creates a texture
    boost::shared_ptr<Light> create_light(std::string, std::map<std::string, Shape *>&); ///< Creates a light
    boost::shared_ptr<Camera> create_camera(std::string); ///< Creates a camera
    boost::shared_ptr<Shape> create_object(std::string, std::map<std::string, Texture *> &); ///< Creates an object

    static std::vector<std::string> & _errors; ///< Errors log
    static std::vector<std::string> & _warnings; ///< Warning log

    static std::vector<ParserYAML *> & _parser_list; ///< Opened parser/files list
    static std::map <std::string, ParserYAML *> & _object_list; ///< Objects found mapped to the corresponding parser
    static std::map <std::string, ParserYAML *> & _light_list; ///< Lights found mapped to the corresponding parser
    static std::map <std::string, ParserYAML *> & _texture_list; ///< Texture found mapped to the corresponding parser
    static std::map <std::string, ParserYAML *> & _camera_list; ///< Camera found mapped to the corresponding parser

    static std::map < std::string, std::map < std::string, std::vector< std::pair <std::string, std::string> > > > & _matrix_param; ///< Tree of parameters
    static void param_init(); ///< Initialize the tree of parameters

private :

    YAML::Node _root; ///< Source node of file
    std::ifstream _file; ///< Incoming stream of opened file
    std::string _filename; ///< Name of opened file
    bool _correct; ///< True if the file was successfully opened and formatted
};

#endif
