//
//  FISlidingMenuItem.m
//  FarmaciaInimii
//
//  Created by Parvan Florian on 20/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "FISlidingMenuItem.h"

@implementation FISlidingMenuItem

-(id)initWithItemName:(NSString*)itemName
      andStoryBoardId:(NSString*)storyboardId
         andImagePath:(NSString*)imagePath
{
    if (self = [super init])
    {
        self.itemName = itemName;
        self.storyBoardId = storyboardId;
        self.imagePath = imagePath;
    }
    return self;
}

@end
