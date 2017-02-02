#include "clicker.h"
#include <chrono>
#include <thread>
#include <X11/extensions/XTest.h>

Clicker::Clicker():
    count(0),
    delay(0),
    clickBtn(0),
    clickMode(0),
    freezePointer(false),
    keepGoing(false)
{

}

Clicker::~Clicker()
{

}

long Clicker::click()
{
    Display *display = XOpenDisplay(NULL);

    if ( display == nullptr )
        return -1;  // cannot open display

    XTestFakeButtonEvent(display, clickBtn, true, CurrentTime);     // press
    XTestFakeButtonEvent(display, clickBtn, false, CurrentTime);    // release

    if (clickMode == 2)
    {
        // i dont find any information about differebce between single & double clicks in x11
        // so just do the second click
        XTestFakeButtonEvent(display, clickBtn, true, CurrentTime);     // press
        XTestFakeButtonEvent(display, clickBtn, false, CurrentTime);    // release
    }

    XFlush(display);
    XCloseDisplay(display);

    return 0;
}

void Clicker::setKeepGoing(const bool &_keepGoing)
{
    mutex.lock();

    keepGoing = _keepGoing;

    mutex.unlock();
}

void Clicker::start()
{
    long clicksCounter = 0;

    while (clicksCounter < count || count == 0)
    {
        if (!keepGoing)
            break;

        if (click())    // here we can show error, but not now :)
            break;

        clicksCounter++;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    emit finished();
}

void Clicker::setup(const long& _count, const long& _delay, const int& _clickBtn, const int& _clickMode, const bool& _freezePointer)
{
    mutex.lock();

    count = _count;
    delay = _delay;
    clickBtn = _clickBtn;
    clickMode = _clickMode;
    freezePointer = _freezePointer;

    mutex.unlock();
}

