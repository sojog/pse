//
//  PrimAjutor.h
//  ObjectRecognition
//
//  Created by Silviu Ojog on 09/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "PEBaseResponse.h"
#import "PEBaseResponse.h"



@interface SearchPictureResponse : PEBaseResponse

@property (nonatomic, strong) NSMutableArray    *data;
-(id)initWithDictionary:(NSDictionary *)dictionary;
@property (nonatomic, strong) NSString *totalPages;

@end
