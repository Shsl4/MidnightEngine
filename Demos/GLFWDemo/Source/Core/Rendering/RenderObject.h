//
//  RenderObject.h
//  GLFWDemo
//
//  Created by Pierre on 15/11/2021.
//

#pragma once

#include <iostream>

class RenderObject{
    
public:
    
    RenderObject() { }
    
    virtual void render(float deltaTime) = 0;
  
    virtual ~RenderObject() { }
    
};
