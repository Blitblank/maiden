
# Contributing
### Note: not a real contributing guide.
Because this project is so early in development, this information outlines a design framework and tasks that need to be completed.

## Vulkan Roadmap
From the authors themselves: https://docs.vulkan.org/tutorial/latest/00_Introduction.html

Basic outline where I pretty much copy the guide:
 - Device Setup: GPU+Queues initialization, CPU+Queues initialization, include validation layers for testing
 - Window Presentation: Surface initialization, Window attachment, Swapchain creation
 - Graphics Pipeline Setup: Shaders -> Render Pass
 - Command Buffers: Framebuffers, Draw commands, Presenation. <<-- HelloTriangle is here
 - Vertex Buffers: Loading geometry data to gpu (mesh loading happens later)
 - Descriptor Sets: Dynamic arbitrary data transfer from CPU -> GPU (the guide introduces 3D here)
 - Textures: Loading Images, Sampling textures in shader
 - Depth Buffering: additional render pass for determining most-front geometry
 - Mesh Loading: guide uses TinyObj to load obj files into vertex & index vectors
 - ... Boring optimization stuff, like mipmaps, multithreading, multisampling
 - it has a section on raytracing :3

 Goals:
 - loading 3d models
 - applying textures
 - simple diffuse lighting
 - vertex animations
 - 3d camera and scene
 - transparency

Future ambitions:
 - entity component system
 - shadows
 - raytracing
 - rigidbody simulation

## Supporting App Infrastructure
Although not crucial to the core rendering functions of the app, features separate from the rendering engine are convenient to have for testing and usability. Components below are less urgent but should be simpler to develop and implement.

### Performance Profiler
In order to properly test different Vulkan practices and engine implementations, performance should be quickly monitorable by the program. Important compute metrics include: CPU utilization, thread count, memory utilization, VRAM utilization, GPU utilization, and most importantly FPS. Supplemental details like geometry count or hardware configuration could be beneficial by providing context. Frame time (and fps) can be easily recorded inside of the engine, though other metrics must be accessed via the operating system's tools perhaps via the POSIX interface (I haven't done too much research). Recording the performance can initially be done through the logging service (detailed further below) but in-window updates with a GUI (oh boy) would be ideal.

### Configuration Service
A configuration service allows the application to be more flexible by providing the option to tweak parameters without requiring recompilation. Useful configuration parameters could be on window creation, rendering settings (geometry culling, descriptor layout), or vertex loading patterns. There are many options for implementation, like yaml, json, or cfg, with various c++ libraries for support. Classes that require configuration parameters will be injected with a config class from the top-level app and can access whatever it needs through the service.

### Logging Service
Consolidating logging functionality to a single service makes the code much more organized and easier to maintain. The advantage of centralized logging over std::couts everywhere is that much more information can be added to each logged statement: timestamps, function & line traces, log severity level (debug, info, warning, error), etc. We can also give it control to write to log files instead of just the terminal session so all logged messages can persist. Again, implementation involves injecting the service to components that require it and they can interface through the reference.
