This project contains 4 files main.cpp that implement different programs using OPENGL.

The first program creates two shapes one hexagon and one decagon. Using the IMGUI library which is a free source library changes to the color fo the shapes can be applied while the program is run. This program aims for the familiarization with the IMGUI library 

The second program creates an hexagon which can be moved with the use of the keyboard keys. While moving constant changes in the color of the hexagon are applied.The changes occur throught the VertexShader which applies the changes to the color by tracking the location. For every location for our shape  the values  of the first 2 indices of the vec3 of the output color change based on the x.y location respectively.

The third program except for the imgui it also uses the glm library which is an open source library mathemtical library. In this code it will be used for handling the matrix transformation to create a 
