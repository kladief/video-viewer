#ifndef CONSLENGINE_H
#define CONSLENGINE_H
#define SYMWHEIGHT 16
#define SYMWIDTH 8
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <math.h>
class Engine{
private:
    HANDLE hConsl;
    CHAR_INFO* Buff;
    RECT conslRect;
    wchar_t gradient[18] = L" .:!/r(l1Z4H9W8$@";
    struct RGB{
        WORD r;
        WORD g;
        WORD b;
    };
public:
    Engine(RECT);
    void setPixel(COORD,RGB);
    void Draw();

};
#endif