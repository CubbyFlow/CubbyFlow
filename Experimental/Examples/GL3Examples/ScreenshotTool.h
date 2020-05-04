//! reference : https://github.com/ocornut/imgui/wiki/screenshot_tool

/**
 * Window 여러개 생성해서 각각 FBO로 rendering 하도록 유도하고,
 * 마지막에는 MainWindow에서 defulat framebuffer에서 생성된 texture들을 합쳐서 rendering하도록 하기.
 * 
 * Real-time fluid surfacing is performed using a GPU based ellipsoid
splatting technique. Particle anisotropy is first computed using the
method of Yu and Turk [2013], and the surface is reconstructed
using a method based on the screen-space filtering presented in
[van der Laan et al. 2009].

* https://subscription.packtpub.com/book/game_development/9781849695046/7/ch07lvl1sec54/implementing-volume-rendering-using-single-pass-gpu-ray-casting
 * 
 * 
 ****************8 fluid volumetric ray casting********************
 * 
 */