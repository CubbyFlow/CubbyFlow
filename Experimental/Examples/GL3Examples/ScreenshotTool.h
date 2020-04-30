//! reference : https://github.com/ocornut/imgui/wiki/screenshot_tool

/**
 * Window 여러개 생성해서 각각 FBO로 rendering 하도록 유도하고,
 * 마지막에는 MainWindow에서 defulat framebuffer에서 생성된 texture들을 합쳐서 rendering하도록 하기.
 * 
 */