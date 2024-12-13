#ifndef WASHERE_H
#define WASHERE_H

#include <Arduino.h>

class WasHere { 

    public:

        /**
         * Default constructor it only see the configurations in directory "/data/application.json"
        */
        WasHere ();
        
        /**
         * Constructor it accept any directory in the folder "/data/"
         * @param directory to find file in format json. For example, json/application.json
        */
        WasHere (String directory);

};


#endif