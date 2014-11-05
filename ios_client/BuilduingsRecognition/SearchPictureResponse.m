//
//  PrimAjutor.m
//  FarmaciaInimii
//
//  Created by Silviu Ojog on 09/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "SearchPictureResponse.h"
#import "NSDictionary+SafeNull.h"

@implementation SearchPictureResponse

-(id)initWithDictionary:(NSDictionary *)dictionary
{
    if (self = [super initWithDictionary:dictionary])
    {

        _data = [[NSMutableArray alloc]init];
          NSArray *objectArray = [dictionary safeObjectForKey:@"photo"];
        for (NSDictionary *dict in objectArray)
        {
           

        }
    
    }
    
    return self;
}

@end
