#include "conslEngine.h"

Engine::Engine(RECT rect){
    conslRect=rect;
    HWND hwnd;
    Buff=new CHAR_INFO[((conslRect.right-conslRect.left)/SYMWIDTH)* //строки
                       ((conslRect.bottom-conslRect.top)/SYMWHEIGHT) /*столбцы*/];
    if((hConsl=CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE ,0,CONSOLE_TEXTMODE_BUFFER ,0))!=INVALID_HANDLE_VALUE){            //создаем дискриптор буфера консоли.
    	SetConsoleActiveScreenBuffer(hConsl);             //делаем наш буфер консоли активным
        wchar_t Title[1024];
		GetConsoleTitle(Title, 1024); 
		hwnd=FindWindow(NULL, Title); 
		MoveWindow(hwnd,rect.left,rect.top,rect.right,rect.bottom,TRUE);
	}
	else{
		GetLastError();
	}
}
void Engine::setPixel(COORD coord,RGB rgb){
    Buff[coord.X+coord.Y*((conslRect.right-conslRect.left)/SYMWIDTH)].Attributes=FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN;
    Buff[coord.X+coord.Y*((conslRect.right-conslRect.left)/SYMWIDTH)].Char.UnicodeChar=gradient[(int)round((float)(rgb.b+rgb.g+rgb.r)/(float)765*(float)16)];
}
void Engine::Draw(){
    ShowCursor(FALSE);
    ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0);
    SMALL_RECT temp={0,0,(short)((conslRect.right-conslRect.left)/SYMWIDTH),(short)((conslRect.bottom-conslRect.top)/SYMWHEIGHT)};
    SetLastError(0);
    if(!WriteConsoleOutputW(hConsl,Buff,{(short)((conslRect.right-conslRect.left)/SYMWIDTH),(short)((conslRect.bottom-conslRect.top)/SYMWHEIGHT)},{0,0}, &temp)){
        int debug=GetLastError();
        int a=0;
    }
    ZeroMemory(Buff,((conslRect.right-conslRect.left)/SYMWIDTH)*((conslRect.bottom-conslRect.top)/SYMWHEIGHT)*sizeof(CHAR_INFO));
}