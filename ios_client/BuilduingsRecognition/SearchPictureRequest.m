//
//  PrimAjutorRequest.m
//  FarmaciaInimii
//
//  Created by Silviu Ojog on 08/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "SearchPictureRequest.h"

@implementation SearchPictureRequest

-(id)initWithPhoto:(UIImage*)photoImage xCoordinate:(NSString*)xPoint yCoordinate:(NSString*)yPoint {
    
    if (self = [super init])
    {
            NSData *dataImage = UIImagePNGRepresentation(photoImage);
                _picture = [dataImage base64EncodedStringWithOptions:
                         
                         NSDataBase64Encoding64CharacterLineLength];
        
        _xCoordinate=xPoint;
        _yCoordinate=yPoint;
    }
  return self;
}

-(NSDictionary*)dictionaryForm
{
    NSDictionary *dict =[[ NSDictionary alloc] initWithObjectsAndKeys:_xCoordinate,@"x",_yCoordinate,@"y", _picture, @"image", nil];

 
    
    return dict;
}

@end
