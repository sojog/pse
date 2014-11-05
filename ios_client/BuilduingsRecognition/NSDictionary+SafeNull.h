//
//  NSDictionary+SafeNull.h
//  Pegas
//
//  Created by Silviu Ojog on 09/09/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDictionary (SafeNull)
- (id)safeObjectForKey:(id)key;

@end
