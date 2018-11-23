//
//  GraphicalSimulationApp.h
//  Stonefish
//
//  Created by Patryk Cieslak on 11/28/12.
//  Copyright (c) 2012-2018 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_GraphicalSimulationApp__
#define __Stonefish_GraphicalSimulationApp__

#include <SDL2/SDL.h>
#include "core/SimulationApp.h"
#include "graphics/OpenGLPipeline.h"
#include "graphics/IMGUI.h"
#include "graphics/Console.h"

namespace sf
{

/*!
    A class that defines a graphical application interface.
 */
class GraphicalSimulationApp : public SimulationApp
{
public:
    GraphicalSimulationApp(std::string name, std::string dataDirPath, RenderSettings s, SimulationManager* sim);
    virtual ~GraphicalSimulationApp();
    
    //GUI
    virtual void DoHUD();
    void ShowHUD();
    void HideHUD();
    void ShowConsole();
    void HideConsole();
    
    //input handling
    bool* joystickButtons;
    int16_t* joystickAxes;
    uint8_t* joystickHats;
    
    virtual void ProcessInputs();
    virtual void WindowEvent(SDL_Event* event);
    virtual void KeyDown(SDL_Event* event);
    virtual void KeyUp(SDL_Event* event);
    virtual void MouseDown(SDL_Event* event);
    virtual void MouseUp(SDL_Event* event);
    virtual void MouseMove(SDL_Event* event);
    virtual void MouseScroll(SDL_Event* event);
    virtual void JoystickDown(SDL_Event* event);
    virtual void JoystickUp(SDL_Event* event);
    
    //accessors
    OpenGLPipeline* getGLPipeline();
    IMGUI* getGUI();
	bool hasGraphics();
    SDL_Joystick* getJoystick();
	double getDrawingTime();
    GLint getWindowWidth();
    GLint getWindowHeight();
    bool isLoading();
	std::string getShaderPath();
    
protected:
	void Init();
    void Loop();
	void CleanUp();
   
    void StartSimulation();
    void ResumeSimulation();
    void StopSimulation();
    
private:
    void InitializeSDL();
    void InitializeGUI();
	void RenderLoop();
    
    SDL_GLContext glMainContext;
    SDL_GLContext glLoadingContext;
    SDL_Thread* loadingThread;
    SDL_Thread* simulationThread;
    SDL_Window* window;
    SDL_Joystick* joystick;
    
    OpenGLPipeline* glPipeline;
    RenderSettings renderSettings;
    IMGUI* gui;
    
    Entity* lastPicked;
	bool displayHUD;
    bool displayConsole;
    std::string shaderPath;
    bool loading;
	double drawingTime;
    
    static int RenderLoadingScreen(void* data);
    static int RunSimulation(void* data);
};

typedef struct
{
    GraphicalSimulationApp* app;
    SDL_mutex* drawMutex;
}
GraphicalSimulationThreadData;

typedef struct
{
    GraphicalSimulationApp* app;
    SDL_mutex* mutex;
}
LoadingThreadData;

}

#endif
