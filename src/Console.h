#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>
#include <Config.h>

class Console
{
    public:
        Console();

        void begin(Config& config);

        void handleConsole();

    private: 
    
        Config config;
};

#endif