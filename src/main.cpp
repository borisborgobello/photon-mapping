/**
 * \file main.cpp
 * \brief Main starting point
 * \author B.BORGOBELLO / T. FEIGLER
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "our_renderer.hpp"

using namespace std ;

/**
 * \brief Main function of the program
 *
 * \param argc : Number of parameters on the command line
 * \param argv : List of strings from the command line
 *
 * The main function takes at most 4 parameters :
 */
int main(int argc, char ** argv)
{
    srand(time(0));

    string in_filename = "none", out_image_name = "result.tga", out_photonmap_image_name, temp_string;
    bool display = false;
    bool photon_map = false;

    for (int i = 1 ; i < argc; i++) {
        temp_string = argv[i];
        if (temp_string.find("--out=") != -1) out_image_name = temp_string.substr(6);
        else if (temp_string.find("--photonmap=") != -1) {
            out_photonmap_image_name = temp_string.substr(12);
            photon_map = true;
        }
        else if (temp_string.find("--display") != -1) display = true;

        if (temp_string[0] != '-') in_filename = temp_string;
    }

    if (argc == 1 || in_filename == "none") {
        cout << "The YAML file to read must be specified on the command line" << endl;
        return 0;
    }

    // Sum up
    cout << "- YAML file to read : " + in_filename << endl;
    cout << "- TGA file to write : " + out_image_name << endl;
    if (!photon_map) cout << "- No Photon Map image will be saved" << endl;
    else cout << "- TGA photon-map file to write : " << out_photonmap_image_name << endl;
    cout << ((display)? "- Automatic display" : "- No automatic display") << endl;
    // End sum up

    OurRenderer renderer ;
    system("pwd") ;

    renderer.parse_file(in_filename) ;

    cout << "Building scene\n" ;
    renderer.build_scene() ;
    cout << "Building photon tree...\n\n" ;
    renderer.build_photon_tree() ;
    cout << "Raytracing...\n\n" ;

    if (photon_map) {
        renderer.raytrace(true) ;
        cout << "Saving file " + out_photonmap_image_name + "...\n\n" ;
        renderer.save_to(out_photonmap_image_name) ;
        temp_string = "display " + out_photonmap_image_name + " &" ;
        if (display) system(temp_string.c_str()) ;
    }

    renderer.raytrace(false) ;
    cout << "Saving ...\n\n" ;
    renderer.save_to(out_image_name) ;
    temp_string = "display " + out_image_name + " &";
    if (display) system(temp_string.c_str()) ;

    exit(0);
}

