/**
 * @file misc.c
 * @author zhihaohong52
 * @brief This file contains miscellaneous functions
 * @version 0.1
 * @date 2024-05-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

#ifdef WIN32
#include "windows.h"
#include "io.h"
#else
#include "sys/time.h"
#include "sys/select.h"
#include "unistd.h"
#include "string.h"
#endif

/**
 * @brief Get the Time Ms object
 *
 * @return int The time in milliseconds
 */
int GetTimeMs() {
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}

/**
 * @brief Wait for the user to input
 * @cite http://home.arcor.de/dreamlike/chess/
 * @return int User input
 */
int InputWaiting()
{
#ifndef WIN32
    fd_set readfds;
    struct timeval tv;
    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec=0; tv.tv_usec=0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
#else
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;

    if (!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if (!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }
    if (pipe) {
        if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
        return dw;
    } else {
        GetNumberOfConsoleInputEvents(inh, &dw);
        return dw <= 1 ? 0 : dw;
	}
#endif
}

/**
 * @brief Read input from the user
 * @cite http://home.arcor.de/dreamlike/chess/
 * @param info Pointer to the search info
 */
void ReadInput(S_SEARCHINFO *info) {
    int bytes;
    char input[256] = "", *endc;

    if (InputWaiting()) {
		info->stopped = TRUE;
		do {
#ifndef WIN32
            bytes=read(fileno(stdin),input,256);
#else
            bytes=_read(_fileno(stdin),input,256);
#endif
		} while (bytes<0);
		endc = strchr(input,'\n');
		if (endc) *endc=0;

		if (strlen(input) > 0) {
			if (!strncmp(input, "quit", 4))    {
			    info->quit = TRUE;
			}
		}
		return;
    }
}