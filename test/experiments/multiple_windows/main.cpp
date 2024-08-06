#include <Logger.h>
#include <ErrorHandling.h>

#include "sdl.h"

#include <iostream>

using namespace GameEngine;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

class LWindow : private Logable
{
public:
    //Intializes internals
    LWindow(): Logable("LWindow")
    {
        //Create window
        m_window = SDL_CreateWindow("SDL_Window_Exp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        EXPECT_MSG(m_window, SDL_GetError());

        m_isMouseFocus = true;
        m_isKeyboardFocus = true;
        m_width = SCREEN_WIDTH;
        m_height = SCREEN_HEIGHT;

        //Grab window identifier
        m_windowID = SDL_GetWindowID(m_window);

        //Create renderer for window
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);
        EXPECT_MSG(m_renderer, SDL_GetError());

        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        //Flag as opened
        m_isShown = true;
    }

    ~LWindow()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    //Handles window events
    void HandleEvent(SDL_Event& e)
    {
        // Check if an event was detected for this window
        if (e.type != SDL_WINDOWEVENT || e.window.windowID != m_windowID)
        {
            return;
        }

        // Caption update flag
        bool isCaptionUpdated = false;

        switch (e.window.event)
        {
            //Window appeared
            case SDL_WINDOWEVENT_SHOWN:
            LOG_DEBUG(m_windowID << " shown");
            m_isShown = true;
            break;

            //Window disappeared
            case SDL_WINDOWEVENT_HIDDEN:
            LOG_DEBUG(m_windowID << " hidden");
            m_isShown = false;
            break;

            //Repaint on expose
            case SDL_WINDOWEVENT_EXPOSED:
            LOG_DEBUG(m_windowID << " exposed");
            SDL_RenderPresent(m_renderer);
            break;

            //Mouse enter
            case SDL_WINDOWEVENT_ENTER:
            LOG_DEBUG(m_windowID << " mouse enter");
            m_isMouseFocus = true;
            isCaptionUpdated = true;
            break;
            
            //Mouse exit
            case SDL_WINDOWEVENT_LEAVE:
            LOG_DEBUG(m_windowID << " mouse leave");
            m_isMouseFocus = false;
            isCaptionUpdated = true;
            break;

            //Keyboard focus gained
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            LOG_DEBUG(m_windowID << " keybord focus gained");
            m_isKeyboardFocus = true;
            isCaptionUpdated = true;
            break;
            
            //Keyboard focus lost
            case SDL_WINDOWEVENT_FOCUS_LOST:
            LOG_DEBUG(m_windowID << " keybord focus lost");
            m_isKeyboardFocus = false;
            isCaptionUpdated = true;
            break;

            //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
            LOG_DEBUG(m_windowID << " minimized");
            m_isMinimized = true;
            break;

            //Window maxized
            case SDL_WINDOWEVENT_MAXIMIZED:
            LOG_DEBUG(m_windowID << " maximized");
            m_isMinimized = false;
            break;
            
            //Window restored
            case SDL_WINDOWEVENT_RESTORED:
            LOG_DEBUG(m_windowID << " restored");
            m_isMinimized = false;
            break;

            //Hide on close
            case SDL_WINDOWEVENT_CLOSE:
            LOG_DEBUG(m_windowID << " close");
            SDL_HideWindow(m_window);
            break;
        }

        // Update window caption with new data
        if (isCaptionUpdated)
        {
            std::stringstream caption;
            caption << "SDL Tutorial - ID: " << m_windowID << " MouseFocus:" << ( ( m_isMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( m_isKeyboardFocus ) ? "On" : "Off" );
            SDL_SetWindowTitle(m_window, caption.str().c_str());
        }
    }

    //Focuses on window
    void Focus()
    {
        //Restore window if needed
        if (!m_isShown)
        {
            SDL_ShowWindow(m_window);
        }

        //Move window forward
        SDL_RaiseWindow(m_window);
    }

    //Shows windows contents
    void Render()
    {
        if (!m_isMinimized)
        {    
            //Clear screen
            SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(m_renderer);

            //Update screen
            SDL_RenderPresent(m_renderer);
        }
    }

    bool IsShown() { return m_isShown; }

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int m_windowID = -1;

    int m_width = 0;
    int m_height = 0;

    bool m_isMinimized = false;
    bool m_isMouseFocus = false;
    bool m_isKeyboardFocus = false;
    bool m_isShown = false;
};

void GameLoopRun()
{
    const int TOTAL_WINDOWS = 3;
    LWindow gWindows[ TOTAL_WINDOWS ];

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        //Handle events on queue
        while (SDL_PollEvent(&e))
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }

            //Handle window events
            for (auto i = 0; i < TOTAL_WINDOWS; ++i)
            {
                gWindows[i].HandleEvent(e);
            }

            //Pull up window
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_1:
                    LOG_DEBUG("SDLK_1");
                    gWindows[0].Focus();
                    break;

                    case SDLK_2:
                    LOG_DEBUG("SDLK_2");
                    gWindows[1].Focus();
                    break;
                        
                    case SDLK_3:
                    LOG_DEBUG("SDLK_3");
                    gWindows[2].Focus();
                    break;
                }
            }
        }

        // Update all windows
        bool areAllWindowsClosed = true;
        for (auto i = 0; i < TOTAL_WINDOWS; ++i)
        {
            gWindows[i].Render();
            areAllWindowsClosed = areAllWindowsClosed && !gWindows[i].IsShown();
        }

        // User closed all windows => quiting
        if (areAllWindowsClosed)
        {
            LOG_INFO("All windows are closed - quiting");
            quit = true;
        }
    }
}

int main()
{
    const LoggerInitializer loggerInitialer(LogLevel::DEBUG);

    try
    {
        AddLogHandler(CreateStdoutLogChannel());
        SetLogLevel(LogLevel::TRACE);

        EXPECT_MSG(SDL_Init(SDL_INIT_VIDEO) == 0, SDL_GetError());
        GameLoopRun();
        SDL_Quit();
    }
    catch (const std::exception &e)
    {
        HANDLE_EXCEPTION();
        return 1;
    }
    catch (...)
    {
        std::cerr << "unexpected exception occurred" << std::endl;
        return 1;
    }

    return 0;
}