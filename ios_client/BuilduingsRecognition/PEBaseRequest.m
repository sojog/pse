//
//  FIBaseRequest.m
//  ObjectRecognition
//
//  Created by Silviu Ojog on 08/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "PEBaseRequest.h"

@implementation PEBaseRequest
-(NSDictionary*)appendAccessToken:(NSDictionary*)dictionary
{
    NSMutableDictionary *tempDict;
    if (dictionary != nil)
    {
        tempDict = [dictionary mutableCopy];
    }
    else
    {
        tempDict = [[NSMutableDictionary alloc]init];
    }
    
    NSString *accessToken = [[NSUserDefaults standardUserDefaults] stringForKey:@"access_token"];
    if (accessToken != nil)
    {
        [tempDict setObject:accessToken forKey:@"access_token"];
    }
    
    //    [tempDict setObject:@"en" forKey:@"language"];
    
    return [NSDictionary dictionaryWithDictionary:tempDict];
}

-(NSDictionary*)dictionaryForm
{
    return [self appendAccessToken:nil];
}
@end
