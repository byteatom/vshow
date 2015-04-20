#include "win_util.h"

void showConsole() {
    AllocConsole();
    //freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "wt", stdout);
    freopen("CONOUT$", "wt", stderr);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(out, &info);
    info.dwSize.Y = 1024;
    SetConsoleScreenBufferSize(out, info.dwSize);

#if 0
    //Redirect unbuffered STDIN to the console
    HANDLE stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
    if(stdInHandle != INVALID_HANDLE_VALUE)
    {
        int fileDescriptor = _open_osfhandle((intptr_t)stdInHandle, _O_TEXT);
        if(fileDescriptor != -1)
        {
            FILE* file = _fdopen(fileDescriptor, "r");
            if(file != NULL)
            {
                *stdin = *file;
                setvbuf(stdin, NULL, _IONBF, 0);
            }
        }
    }

    //Redirect unbuffered STDOUT to the console
    HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(stdOutHandle != INVALID_HANDLE_VALUE)
    {
        int fileDescriptor = _open_osfhandle((intptr_t)stdOutHandle, _O_TEXT);
        if(fileDescriptor != -1)
        {
            FILE* file = _fdopen(fileDescriptor, "w");
            if(file != NULL)
            {
                *stdout = *file;
                setvbuf(stdout, NULL, _IONBF, 0);
            }
        }
    }

    //Redirect unbuffered STDERR to the console
    HANDLE stdErrHandle = GetStdHandle(STD_ERROR_HANDLE);
    if(stdErrHandle != INVALID_HANDLE_VALUE)
    {
        int fileDescriptor = _open_osfhandle((intptr_t)stdErrHandle, _O_TEXT);
        if(fileDescriptor != -1)
        {
            FILE* file = _fdopen(fileDescriptor, "w");
            if(file != NULL)
            {
                *stderr = *file;
                setvbuf(stderr, NULL, _IONBF, 0);
            }
        }
    }
#endif

    /*std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();*/

    //ios::sync_with_stdio()

    /*
    outbuf ob;
    std::streambuf *sb = std::cout.rdbuf(&ob);
    ......
    std::cout.rdbuf(sb);
    */

    printf("printf ok\r\n");
    std::cout << "cout ok" << std::endl;
}
