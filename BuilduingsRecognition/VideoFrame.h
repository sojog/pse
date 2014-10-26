//
//  Frame.h
//  Building Recognition
//
//  Created by stefan on 10/26/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#ifndef Building_Recognition_Frame_h
#define Building_Recognition_Frame_h


#include <cstddef>

struct VideoFrame
{
    size_t width;
    size_t height;
    size_t stride;
    
    unsigned char * data;
};

#endif
