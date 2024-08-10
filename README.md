# Computer Graphics Playground
This project is to learn and play around with computer graphics using SFML and a graphics library.

The graphics library I initially chose is OpenGL, but I found it difficult to predict the communication between my code and the GPU. This is mainly due to OpenGL having a global state where commands affect this global state but can have other consequences; even when is not implicitly stated by the documentation. So, I'm mirgrating over to a new graphics library to predict my code output better.

I first thought to migrate over to Vulkan, but by doing research into other Graphic libraries. I realized that Vulkan would be too complex for a beginner to learn because one has to manually do everything with the GPU. Therefore, I looked for other graphics libraries and I decided to migrate over to DirectX 12 because it resembles modern graphic programming without too much complexity. Though it's more complex than OpenGL, I hope that I will be able to learn better with more control over the GPU alongside modern API. 

# The state of the project
This project is currently migrating to DirectX 12 from OpenGL; this will take some time.
