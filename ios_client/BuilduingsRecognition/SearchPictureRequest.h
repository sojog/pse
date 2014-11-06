//
//  PrimAjutorRequest.h
//  ObjectRecognition
//
//  Created by Silviu Ojog on 08/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "PEBaseRequest.h"


@interface SearchPictureRequest : PEBaseRequest

@property (strong, nonatomic) NSData *picture;
@property (strong, nonatomic) NSString *xCoordinate;
@property (strong, nonatomic) NSString *yCoordinate;


-(id)initWithPhoto:(UIImage*)photoImage xCoordinate:(NSString*)xPoint yCoordinate:(NSString*)yPoint ;
-(NSDictionary*)dictionaryForm;
@end
