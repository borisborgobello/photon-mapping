# Photon mapping by Boris Borgobello & Thomas Feigler

First of all, know that this is an old project made for educational purpose during year 2011. By cleaning my stuff a little I discussed with Thomas and we decided to share this project with the community.
Unfortunately, the former versioning system SVN that we used is no more and it's not possible with SVN (contrary to GIT) to recover from a working SVN tree back to a bare repository. History is, thus, LOST FOREVER.

Before I uploaded the project to git I reorganized the project and quickly updated the external libs if it was straightforward. 

==> This project is not maintained anymore.

 Many included papers are written in french, but the doxygen lines have always been written in english which makes understanding of the project easy to anyone.

### Basic features of the program

The program is written in C++ and uses 4 external libraries. Memory tests have been run to track any memory leak (given the size of a photon map)

- It takes a YAML 3D scene description as primary entry positionning the following
  - Cameras
  - Lights
  - Objects
- The program will deliver to you a TGA file based on the scene using the photon-mapping technique, and if desired the corresponding photon-map

If you wish to see our results during developpement and thus the ones you may obtain, they are available in a second repository that you can checkout as follow :

```
git clone git@github.com:orkactus/photon-mapping-imgs.git
```

### Cloning the repository

To clone the entire repository :

```
git clone git@github.com:orkactus/photon-mapping.git
```

### Program usage

##### Execute

If you are running ubuntu64 (other deb based should be compatible), you can find an executable in folder perma_bins named photon_mapping_ubuntu64.
Other, scroll down to the section "Project & Compilation".

Using ./photon_mapping_ubuntu64 will immediately run the program.
Using --help or -h or no argument will display help.

The YAML file should be in the same directory as the executable file call.
Example : 
```
cd tests/test1 && ./../bin/photon_mapping_ubuntu64 -t cornel_box.txt
```

Executable arguments ares few :
- YAML file in
- TGA image out
- TGA photon-map out
- Display

##### YAML customization

The YAML parser implementation of the project does surface checks. 
It also allows you to organize objects into libraries. Then you import the object you want into the scene.

Check the cornel_box files in the tests directories, it's pretty powerful and straight forward.

When executing the program you give a YAML input file. This file can contain CAMERAS, OBJECTS, LIGHTS, and LOADINGS (which import another YAML file)

In particular this file will contain a part called SCENE which will take elements previously declared in the YAML file or it's imports. Scenes from imported YAML files are NOT USED. Only the scene from the source YAML file will be use. The SCENE is MANDATORY.

Let's review it :

```
SCENE:
  resolution: [100, 100] => Resolution of final picture
  supersampling: false 
  nb_photon_MAX: 100000
  nb_photon_to_find: 5 => For kd-tree search, it find the (K=5) nearest photon around raytracing impact to choose a color
  photon_depth: 40 => How many times a photon can be refracted or reflected (it stops when absorbed)
  raytracer_depth: 4 => How many reflection/refraction recursivity
  camera: Ze_camera => The camera that will be used
  objects: [UP, DOWN, LEFT, RIGHT, FACE, SPHERE1, SPHERE3, PARA1] => The objects used in the scene
  lights: [Radiant_SPHERE, Glob] => The lights used in the scene
```
Flexible and straightforward.

##### YAML root types

YAML file roots :
- LOADING
- CAMERAS
- OBJECTS
- LIGHTS
- TEXTURES
- SCENE

OBJECTS/LIGHTS/CAMERAS/TEXTURES syntax :

```
SECTION: 
	-element_name1:
		attribute1: value
		attribute2: [value1, value2, value3] 
        attribute3: value
	-nom_element2: ...*/
```

Examples : 

```
OBJECTS:
  -FACE:
    type: Plane
    texture: Vert
    absorb: 0.2
    reflect: 0.8
    transparency: 0
    point: [1, 0, 0]
    normal: [-1, 0, 0]
  -LEFT:
    type: Plane
    texture: Orange
    absorb: 0.2
    reflect: 0.8
    transparency: 0
    point: [0, 0, -1]
    normal: [0, 0, 1]
```
Face and left are here 2 walls of the cornel box.

LOADING syntax :

```
LOADING:
  - cornel_box_2.txt
```
Imports YAML file cornel_box_2.txt

##### YAML full catalogue

The program supports procedural and image based textures.
Please read PDF docs/docs_fr/Documentation utilisateur.pdf
Jump directly to section 3.3

### Project & Compilation

##### Compilation

Compilation si based on CMAKE, please make sure you have CMAKE installed.
use the following command to build :
```
./build.sh
```

Clean is done automatically before each build. If custom clean is necessary use :
```
./clean.sh
```

The file CMakeLists is the main config files for compilation. It is called automatically by the build.sh script. You find inside any include folder, sources, defines, etc...

##### Project structure

- docs : contains the documentation

- src : contains the WORKING sources of the project

- src_parallel : contains sources that used Intel TBB to increase the program speed. It used to work but it has not been retested again or updated like the working src directory.
- tests : contains tests YAML
- extlibs : contains external libraries

##### Ext libs

The project contains 4 external libraries shipped directly with the repository :
- Boost 1.61.0 (custom)
- Eigen 2
- libssrckdtree
- yaml-cpp

Libssrckdtree has been updated to version 1.0.8, the most recent one.
Yaml-cpp has been updated as well.
Boost as been updated as well and I left just a subset of headers to reduce the size of the project, using a boost modularized being too much trouble for nothing in my opinion.
Eigen 2 has not been updated because the update is not straightforward

I planned on adding submodules to git but didn't for many reason.
Yaml is not maintained, libssrckdtree is given as zip, and boost is too complicated. I still optimized the boost.

As for eigen, it relies on a mercurial. For anybody that change the code for eigen3 compatibility, it would be a good ideal to link the mercurial instead.

On the good side, it means that the project ships with any external lib you might be needing (aside the intel tbb available from repos)

##### Ext libs usage

- Eigen is used for geometry
- Boost is used for the smart_ptr and the filesystem
- Lib ssrckdtree is used to find the nearest photon around a point
- yaml is used as the yaml parser

### Final thoughts

The procession time is long, very long, we didn't have time to transfer the code to use GPU via CUDA. We think it's the absolute next step for increasing the speed.

Source is delivered as is, without any warranty of any kind, in the sole purpose of helping people that want to achieve similar goals or go deeper inside the project. We are open to any fork.

We are really busy and cannot answer specific question, but if you really need to contact us, feel free to contact me to : boris.borgobello@gmail.com

Thanks for reading !
Boris & Thomas