#ifndef __SDCARD__
#define __SDCARD__

class SDCard {
    public:
        SDCard();
        void diagnose();
        void log(float rpm, char *weight);

    private:
        bool ok;
        char fileName[128];
};

#endif