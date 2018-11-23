//
//  FeedbackController.h
//  Stonefish
//
//  Created by Patryk Cieslak on 18/07/2014.
//  Copyright (c) 2014 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_FeedbackController__
#define __Stonefish_FeedbackController__

#include "controllers/Controller.h"
#include "controllers/SignalMux.h"

namespace sf
{

/*! Abstract class representing a feedback controller */
class FeedbackController : public Controller
{
public:
    FeedbackController(std::string uniqueName, unsigned int numberOfInputs, btScalar frequency);
    virtual ~FeedbackController();
    
    virtual void Reset() = 0;
    
    void setReferenceSignalGenerator(unsigned int inputId, SignalGenerator* sg);
    void setReferenceSignalMux(SignalMux* sm);
    void setReferenceValue(unsigned int inputId, btScalar value);
    void setReferenceValues(const std::vector<btScalar>& values);
    std::vector<btScalar> getReferenceValues();
	btScalar getLastOutput();
    unsigned int getNumOfInputs();
    ControllerType getType();
    
protected:
    virtual void Tick(btScalar dt) = 0;
	btScalar output;
    
private:
    std::vector<btScalar> reference;
    SignalGenerator* referenceGen;
    unsigned int referenceGenInput;
    SignalMux* referenceMux;
};

}

#endif
