# JustRender
JustRender is a 3D Rasterization test of screen space reflections, screen space ambient occlusion, and temporal reprojection anti-aliasing in OpenGL with Discord support, positional audio, and a simple fast fourier transform implementation to create a music visuaizer.  
### Demo:  
https://youtu.be/ThF004bvOwY  
### Music
Song supplied in files is "Ordinary Days" by Klaus Veen and can be found at https://klausveen.bandcamp.com/track/klaus-veen-ordinary-days
### Contact Info
Feel free to reach out to me at rodriguezalejandro@utexas.edu if you have any questions about this project, be it implementation, compilation, or just general questions.
## Requirements to Run
This program only supports Windows, and is runnable by downloading the "Runnable" folder and running the .exe inside.  
### System Requirements:  
- x64 compatible Windows install
- OpenGL Version 4.1 Support
- Discrete GPU advised, but not required (step size of SSR is quite high, so can cause issues on slower GPUs)  
### Controls:
- WASD: Movement relative to camera
- Mouse: Camera orientation
- 1: Toggle SSR
- 2: Toggle SSAO
- 3: Toggle TAA
- Esc: Close the program
Should you wish to test the program with a different audio file, just replace "ordinary.wav" with any .wav of the same name. 
## Features
### SSAO
Implemented Screen Space Ambient Occlusion within a randomly sampled sphere against the depth buffer.    
Issues:  
- False shading is generated sometimes on the sides of objects because not recognizing the spacing between objects.
### SSR
Screen Space Reflections by reflecting rays in view space off the normals of the checkerboard floor below inside a fragment shader.    
Issues:
- Naively steps forward until it crosses out of screen space, so is quite slow
- Issues with the camera being pointed downward making the scene brighter somehow: has to do with Normals being small enough that the surface somewhat 'self reflects'.
### TAA
Temporal Reprojection Anti-Aliasing by reprojecting color from the previous 16 frames into the current frame's geometry.    
Issues: 
- The current weights assigned make the resolved image a bit soft
- The jittering of the camera needed for TAA is done prior to rendering and isn't compensated for at the end, so getting close to geometry shows the micro-stutters
- Interacts with SSAO to ghost some of the SSAO darkening when the visualizer spikes suddenly.
### Sound
Using SDL Mixer 2.0 this program is able to have sound! Through a naive calculation of sound falloff (similar in execution to raytracing light falloff) and dot products against left and right ear vectors, the visualizer is a source of sound with sound changing based on the player's relative position to simulate positional audio.    
Issues:  
- Volume is not influenced by object occlusion.
- Only supports a single source of sound at the moment.
- Only works with 'ordinary.wav' or files with the same name.

### Discord
Launching the program shows in Discord the name of the 'game' as well as the time spent as well as a small graphic.

## Compilation
Program has been tested on Visual Studio 2019 and 2017 Community editions and requires the use of NuGet Package Manager (can be added through the Visual Studio Installer) for handling OpenGL compilation.  
### Process:
1. Download or Clone this repo to our preferred location.
2. Open the FFT.sln file in Visual Studio.
3. Compile for your desired platform.
4. Copy materials from "Needed Files" to your "FFT/x64/(Debug or Release)" folder.
5. Run the program from either Visual Studio or the .exe

## Credits
This program was made using the following libraries/people
### Libraries:
- The University of Texas at Austin Graphics Library: used for helping with various small elements of the OpenGL setup process (specific files can be seen in their own folder when opening the solution in Visual Studio).
- SDL 2.0
- SDL Mixer
### People:
- Dr. Neal Hall of UT Austin for helping me understand the Discrete Fourier Transform as well as the process used by the Fast Fourier Transform for the visualizer.
- Dr. Etienne Vouga of UT Austin for explaining the mechanics of SSAO in lecture for his Computer Graphics class.
- Lasse Jon Fuglsang Pedersen of Playdead for his GDC talk on the process of Temporal Reprojection Anti-Aliasing.
- Will Pearce and his article on roar11.com about SSR implementation in DirectX
