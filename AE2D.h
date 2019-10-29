/*
 * Aasi Engine 2D
 *
 * Requires SDL2 headers and libraries to be
 * linked with the program in order to work.
 *
 * (c) AasiApina 2019
 */

#define AE_NORMAL_MODE            0x00
#define AE_HIGH_PERFORMANCE_MODE  0x01

#ifndef __AE2D_H__
#define __AE2D_H__

#include <SDL2/SDL.h>

#include <iostream>
#include <string>

class AE_Display {

private:

    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_RenderTarget;

    int m_Width, m_Height;

    int m_Pitch;
    uint32_t* m_Pixels;

    bool m_CloseRequested;
    SDL_Event m_Event;

    uint8_t m_RenderMode;

public:

    AE_Display(uint8_t render_mode = AE_NORMAL_MODE)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cout << "[AE2D] Failed to initialize display!" << std::endl;
        }
        m_Window = NULL;
        m_Renderer = NULL;
        m_RenderTarget = NULL;

        m_Width = -1;
        m_Height = -1;

        m_RenderMode = render_mode;

        m_CloseRequested = false;
    }

    ~AE_Display()
    {
        if (m_Window != NULL)
            SDL_DestroyWindow(m_Window);
    
        if (m_RenderMode == AE_HIGH_PERFORMANCE_MODE)
        {
            SDL_DestroyRenderer(m_Renderer);
            SDL_DestroyTexture(m_RenderTarget);
        }

        SDL_Quit();
    }

    bool createWindow(std::string title, int width, int height)
    {
        m_Width = width;
        m_Height = height;

        m_Window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            m_Width,
            m_Height,
            0
        );

        if (m_Window == NULL)
        {
            std::cout << "[AE2D] Failed to create a window!" << std::endl;
            return false;
        }

        if (m_RenderMode == AE_NORMAL_MODE)
        {
            m_Pitch = 4;
            m_Pixels = (uint32_t*) SDL_GetWindowSurface(m_Window)->pixels;
        }
        else if (m_RenderMode == AE_HIGH_PERFORMANCE_MODE)
        {
            m_Renderer = SDL_CreateRenderer(
                m_Window,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

            if (m_Renderer == NULL)
            {
                std::cout << "[AE2D] Failed to create a renderer!" << std::endl;
                return false;
            }

            m_RenderTarget = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);

            if (m_RenderTarget == NULL)
            {
                std::cout << "[AE2D] Failed to create a render target!" << std::endl;
                return false;
            }

            if (SDL_LockTexture(m_RenderTarget, NULL, (void**) &m_Pixels, &m_Pitch) != 0)
            {
                std::cout << "[AE2D] Failed to setup a render target!" << std::endl;
                return false;
            }
            SDL_UnlockTexture(m_RenderTarget);
        }
        else {
            std::cout << "[AE2D] Unknown render mode!" << std::endl;
            return false;
        }

        return true;
    }

    void pollEvents()
    {
        while (SDL_PollEvent(&m_Event) != 0)
        {
            if (m_Event.type == SDL_QUIT)
            {
                m_CloseRequested = true;
            }
        }
    }

    bool closeRequested()
    {
        return m_CloseRequested;
    }

    void closeWindow()
    {
        SDL_DestroyWindow(m_Window);
        m_Window = NULL;
    }

    void update()
    {
        if (m_RenderMode == AE_NORMAL_MODE)
        {
            SDL_UpdateWindowSurface(m_Window);
        }
        else if (m_RenderMode == AE_HIGH_PERFORMANCE_MODE)
        {
            SDL_UnlockTexture(m_RenderTarget);

            SDL_RenderCopy(m_Renderer, m_RenderTarget, NULL, NULL);
            SDL_RenderPresent(m_Renderer);

            if (SDL_LockTexture(m_RenderTarget, NULL, (void**) &m_Pixels, &m_Pitch) != 0)
            {
                std::cout << "[AE2D] Failed to lock render target!" << std::endl;
                exit(-1);
            }
        }
    }

    void setPixel(int x, int y, uint32_t color)
    {
        m_Pixels[y * m_Width + x] = color;
    }
};

#endif // __AE2D_H__