//
//  NSDictionary+SafeNull.m
//  Pegas
//
//  Created by Silviu Ojog on 09/09/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "NSDictionary+SafeNull.h"

@implementation NSDictionary (SafeNull)
- (id)safeObjectForKey:(id)key
{
	id value = [self valueForKey:key];
    
	if (value == [NSNull null]) {
		return nil;
	}
	return value;
}

@end
