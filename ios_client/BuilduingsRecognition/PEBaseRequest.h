//
//  FIBaseRequest.h
//  ObjectRecognition
//
//  Created by Silviu Ojog on 08/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PEBaseRequest : NSObject
-(NSDictionary*)appendAccessToken:(NSDictionary*)dictionary;

-(NSDictionary*)dictionaryForm;
@end
