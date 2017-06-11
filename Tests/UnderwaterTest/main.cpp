//
//  main.cpp
//  UnderwaterTest
//
//  Created by Patryk Cieslak on 02/03/2014.
//  Copyright(c) 2014-2017 Patryk Cieslak. All rights reserved.
//

#include "UnderwaterTestApp.h"
#include "UnderwaterTestManager.h"

int main(int argc, const char * argv[])
{
    UnderwaterTestManager* simulationManager = new UnderwaterTestManager(200.0);
    UnderwaterTestApp app(1000, 700, simulationManager);
#ifdef __linux__
    app.Init("../../../../Library/data", "../../../../Library/shaders");
#else
    app.Init("Data", "Shaders");
#endif
    app.EventLoop();
    app.CleanUp();
    
    return 0;
}
