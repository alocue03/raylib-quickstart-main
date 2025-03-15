#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "GameObject.h"
#include <vector>
#include "MemoryManager.h"
#include "AudioManager.h"
#include "CirCleComponent.h"
#include "Gato.h"
#include "lua.hpp"

#include "resource_dir.h"    // utility header for SearchAndSetResourceDir

extern "C" {
#include "md5.h"
}
// Definición de niveles de verbosidad
typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

// Definición de módulos
typedef enum {
    MODULE_RENDER,
    MODULE_INPUT,
    MODULE_AUDIO,
    MODULE_NETWORK
} Module;

// Variables globales para los filtros
LogLevel currentLogLevel = LOG_LEVEL_DEBUG;

// Función para establecer el nivel de verbosidad
void SetLogLevel(LogLevel level) {
    currentLogLevel = level;
}

// Función de DebugLog con filtros
void DebugLog(LogLevel level, Module module, const char* message) {
    if (level <= currentLogLevel) {
        const char* levelStr;
        switch (level) {
        case LOG_LEVEL_ERROR: levelStr = "ERROR"; break;
        case LOG_LEVEL_WARNING: levelStr = "WARNING"; break;
        case LOG_LEVEL_INFO: levelStr = "INFO"; break;
        case LOG_LEVEL_DEBUG: levelStr = "DEBUG"; break;
        default: levelStr = "UNKNOWN"; break;
        }

        const char* moduleStr;
        switch (module) {
        case MODULE_RENDER: moduleStr = "RENDER"; break;
        case MODULE_INPUT: moduleStr = "INPUT"; break;
        case MODULE_AUDIO: moduleStr = "AUDIO"; break;
        case MODULE_NETWORK: moduleStr = "NETWORK"; break;
        default: moduleStr = "UNKNOWN"; break;
        }

        printf("[%s] [%s] %s\n", levelStr, moduleStr, message);
    }
}

typedef struct {
    int resX;
    int resY;
    bool fullscreen;
    bool vsync;
} VideoConfig;

// Función para leer la configuración desde un archivo INI
void LoadConfig(const char* filename, VideoConfig* config) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se puede abrir el archivo %s\n", filename);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "resx=%d", &config->resX) == 1) continue;
        if (sscanf(line, "resy=%d", &config->resY) == 1) continue;
        if (sscanf(line, "fullscreen=%d", (int*)&config->fullscreen) == 1) continue;
        if (sscanf(line, "vsync=%d", (int*)&config->vsync) == 1) continue;
    }

    fclose(file);
}

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);
    // Front Face
    rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    // Back Face
    rlNormal3f(0.0f, 0.0f, -1.0f);     // Normal Pointing Away From Viewer
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    // Top Face
    rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    // Bottom Face
    rlNormal3f(0.0f, -1.0f, 0.0f);     // Normal Pointing Down
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    // Right face
    rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    // Left Face
    rlNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}

////////////////////////////////////
//LUA
////////////////////////////////////

int DrawCircle(lua_state* L)
{
    float x = (float)lua_tonumber(L, 1);
	float y = (float)lua_tonumber(L, 2);
    float radius = (float)lua_tonumber(L, 3);
    float r = (float)lua_tonumber(L, 4);
    float g  = (float)lua_tonumber(L, 5);
    float b = (float)lua_tonumber(L, 6);
    Color color = { r,g,b,255 };
    DrawCircle(x, y, radius, color);
    return 0;
}

int DrawRectangle(lua_state* L)
{
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float w = (float)lua_tonumber(L, 3);
    float h = (float)lua_tonumber(L, 4);
    float r = (float)lua_tonumber(L, 5);
    float g = (float)lua_tonumber(L, 6);
    float b = (float)lua_tonumber(L, 7);
	Color Color = { r,g,b,255 };
    DrawRectangle(x, y, w, h, color);
    return 0;
}

int Clear(lua_state* L)
{
    float r = (float)lua_tonumber(L, 1);
    float g = (float)lua_tonumber(L, 2);
    float b = (float)lua_tonumber(L, 3);
    float a = (float)lua_tonumber(L, 4);
    Color c = { r,g,b,a };
	ClearBackground(c);
    return 0;
}

void LuaDraw(lua_State* L, float dt)
{
	lua_getglobal(L, "Draw");
    if (lua_isfunction(L, -1))
    {
        lua_pushnumber(L, dt);
        if (lua_pcall(L, 1, 0, 0) != 0)
        {
            printf("Error calling draw from lua");
            printf(lua_tostring(L, -1));
        }
    }
    else
    {
        printf("Draw function not found Lua")
    }
}

int lua_mymodule(lua_state* L)
{
    static const luaL_Reg myModule[] =
    {
        {"Clear", Clear},
        {"DrawCircle", DrawCircle},
        {"DrawRect", DrawRectangle},
        {NULL, NULL}
    };
	luaL_newlib(L, myModule);
    return 1;
}

int main(int argc, char** argv)
{
    //Gato cat;
    //cat.GatoMain();
    
    //prueba de md5
    //char* input = "hola mundo";
    //uint8_t result[16];
    //md5String(input, result);
    //for (int i = 0; i < 16; i++)
    //{
    //    printf("%02x", result[i]);
    //}
    //char hash[33];
    //for (int i = 0; i < 32; i++)
    //{
    //    sprintf(&hash[i*2],"%02x", (unsigned int)result[i]);
    //}

    //puts("");
    //std::cout << hash << std::endl;
    // 
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    //// Initialize Lua
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, "SimpleDraw", lua_mymodule, 1);
    lua_pop(L, 1);

    if (luaL_dofile(L, "main.lua"))
    {
        printf("Error cargando el script main.lua");
        printf(lua_tostring(L, -1));
    }
    else
        printf("main.lua file loaded\n");

    // Leer configuración desde el archivo INI
    VideoConfig config = { 640, 480, false, false };  // Valores predeterminados
    LoadConfig("config.ini", &config);

    // Configurar la ventana según la configuración leída
    if (config.vsync) {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }
    if (config.fullscreen) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    // Crear la ventana y el contexto OpenGL
    InitWindow(config.resX, config.resY, "Hello Raylib");
    if (config.fullscreen) {
        ToggleFullscreen();
    }

	//std::vector<GameObject*> gameObjects;

 //   for (int i = 0; i < 10; i++)
 //   {
 //       GameObject *k = GameObject::Spawn({ 5.0f*i,5.0f*i }, { 100,5.0f*i }, "Ottis");
 //       k->enabled = i % 2 == 0;
 //       gameObjects.push_back(k);

 //   }

    GameObject* go = new GameObject();
    ptrComponents newComp = std::make_shared<CirCleComponent>();
    go->AddComponent(newComp);

    std::vector<GameObject*> gameobjects;
    for (int i = 0; i < 100; i++)
    {
        GameObject* go = new GameObject();
        ptrComponents newComp = std::make_shared<CirCleComponent>();
        go->AddComponent(newComp);
        gameobjects.push_back(go);
    }

	MemoryManager::getInstance()->alloc(800*1024*1024);

	//GameObject *k = GameObject::Spawn({ 100,100 }, { 250,100 }, "Ottis");



    // Establecer filtros
    SetLogLevel(LOG_LEVEL_DEBUG);

    // Ejemplos de uso de DebugLog
    DebugLog(LOG_LEVEL_INFO, MODULE_RENDER, "Render module initialized.");
    DebugLog(LOG_LEVEL_WARNING, MODULE_INPUT, "Input module warning.");
    DebugLog(LOG_LEVEL_ERROR, MODULE_AUDIO, "Audio module error.");
    DebugLog(LOG_LEVEL_DEBUG, MODULE_RENDER, "Render module debug message.");

    printf("Initializing game subsystem \n");

    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    Texture cubeTex = LoadTexture("Minecraftpng.png");


    Camera3D camera = { 0 };
    camera.position = { 4,0,2 };
    camera.target = { 0,0,0 };
    camera.up = { 0,1,0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    AudioManager::GetInstance()->LoadBackgroundMusic("TECNO1.XM");
    AudioManager::GetInstance()->PlayBGM();


    // game loop
    while (!WindowShouldClose())        // run the loop untill the user presses ESCAPE or presses the Close button on the window
    {
        AudioManager::GetInstance()->Update();

        UpdateCamera(&camera, CAMERA_FREE);
       // for (int i = 0; i < gameObjects.size(); i++)
       // {
       //     if(gameObjects[i]->enabled)
			    //gameObjects[i]->Update();
       // }
        for (auto& go : gameobjects)
        {
            go->Update(GetFrameTime());
        }
		//k->Update();

        // drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(LIGHTGRAY);

        BeginMode3D(camera);

        for (auto& go : gameobjects)
        {
            go->Draw(GetFrameTime());
        }

        DrawCubeTexture(cubeTex,{ 0, 0, 0 }, 2, 2, 2, WHITE);

        DrawGrid(100, 1);

        EndMode3D();

        // Dibujar la imagen descargada
        DrawTexture(wabbit, 10, 10, WHITE);

        luaDraw(L,GetFrameTime())

        //for (int i = 0; i < gameObjects.size(); i++)
        //{
        //    gameObjects[i]->Draw();
        //}
		//k->Draw();

        // end the frame and get ready for the next one  (display frame, poll input, etc...)
        EndDrawing();
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);
    UnloadTexture(cubeTex);

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
