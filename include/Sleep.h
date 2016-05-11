#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#ifdef _WIN32
    #include <windows.h>

    void MySleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>

    void MySleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif

#endif // SLEEP_H_INCLUDED
