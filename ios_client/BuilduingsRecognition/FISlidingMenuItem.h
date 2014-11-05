//
//  FISlidingMenuItem.h
//  FarmaciaInimii
//
//  Created by Parvan Florian on 20/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FISlidingMenuItem : NSObject

@property (nonatomic, strong) NSString *itemName;
@property (nonatomic, strong) NSString *storyBoardId;
@property (nonatomic, strong) NSString *imagePath;

// For sliding Menu
-(id)initWithItemName:(NSString*)itemName
      andStoryBoardId:(NSString*)storyboardId
         andImagePath:(NSString*)imagePath;


@end
