#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "conslEngine.h"
#include <cstdlib>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sysinfoapi.h>
#define FPS 30

void skip(cv::VideoCapture* cap,int time,long long int* frame,char* fileName=nullptr){
    if(time>0){
        for(int i=0;i<=time*FPS;i++){
            cv::Mat frame;
            cap->read(frame);
        }
        *frame+=time*FPS;
    }
    else if(time<0){
        if(fileName){
            cap->release();
            cap->open(fileName);
            int debug=(*frame+time*FPS);
            for(int i=0;i<=debug;i+=1){
                cv::Mat frame;
                cap->read(frame);
            }
            *frame+=time*FPS;
        }
    }
}

int wmain(int argc, wchar_t* argv[])
{
    using namespace cv;
    using namespace std;
    if(argc<2)
        return 0;
    wchar_t* tempName=argv[1];
    char fileName[256];
    WideCharToMultiByte(CP_ACP,NULL,tempName,-1,fileName,256,NULL,NULL);
    VideoCapture cap(fileName);

    int imageWidth=840,imageHeight=270;
    Engine eng({0,0,(short)imageWidth*(short)(SYMWIDTH+1),(short)imageHeight*(short)(SYMWHEIGHT+2)});
    DWORD fdwSaveOldMode;
    HANDLE inputHandle=GetStdHandle(STD_INPUT_HANDLE);// хэндл ввода
    GetConsoleMode(inputHandle, &fdwSaveOldMode); // получаем нынешние параметры консоли
    SetConsoleMode(inputHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);// устанавливаем новые параметры консоли
    if (!cap.isOpened()) {
        cout << "ERROR! Unable to open camera\n";
        return -1;
    }
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    int start=GetTickCount();
    long long int frameNum=0;
    for (;;)
    {
        Mat frame;
        cap.read(frame);
        if (frame.empty()) {
            cout << "ERROR! blank frame grabbed\n";
            break;
        }
        Mat frame_2;
        applyColorMap(frame,frame_2,cv::COLORMAP_BONE);
        resize(frame_2,frame_2,Size(imageWidth,imageHeight));
        // imshow("Live", frame_2);

        uchar* image=frame_2.data;
        for(int x=0;x<imageWidth;x++){
            for(int y=0;y<imageHeight;y++){
                int debug1=image[(x+y*imageWidth)*3];
                int debug2=image[(x+y*imageWidth)*3+1];
                int debug3=image[(x+y*imageWidth)*3+2];
                eng.setPixel({(short)x,(short)y},{image[(x+y*imageWidth)*3],image[(x+y*imageWidth)*3+1],image[(x+y*imageWidth)*3+2]});
            }
        }
        eng.Draw();       
        while(GetTickCount()-start<1000/FPS){
            Sleep(5);            
        } 
        start=GetTickCount();
        int debug=0;
        frameNum+=1;

        INPUT_RECORD input[128];
        ZeroMemory(input,sizeof(input));
        DWORD record=0;
        PeekConsoleInput(inputHandle,input,128,&record);// читаем буффер консоли(но не очищаем его)
        for(int i=0;i<record;i++){
            switch (input[i].EventType){
                case MOUSE_EVENT:
                    break;
                case KEY_EVENT:// клавиатура
                    if(input[i].Event.KeyEvent.bKeyDown){// нажата ли кнопка
                        switch(input[i].Event.KeyEvent.wVirtualKeyCode){
                            case VK_LEFT://отмотка назад
                                skip(&cap,-5,&frameNum,fileName);
                                break;
                            case VK_RIGHT://отмотка вперёд
                                skip(&cap,5,&frameNum);
                                break;
                        }
                    }
                    break;
            }
        }
        FlushConsoleInputBuffer(inputHandle); //очищаем буффер консоли
    }
    SetConsoleMode(inputHandle, fdwSaveOldMode); // возвращаем старые параметры консоли
    system("pause");
    return 0;
}