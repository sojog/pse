//
//  FIHttpClient.h
//  ObjectRecognition
//
//  Created by Silviu Ojog on 05/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <AFNetworking/AFNetworking.h>
#import "AFHTTPRequestOperationManager.h"

#import "SearchPictureRequest.h"
#import "SearchPictureResponse.h"

#import "PEBaseRequest.h"
#import "PEBaseResponse.h"
#import <SBJson/SBJson.h>



typedef void (^PERequestResponse)(PEBaseResponse *response, NSError* error);
@interface PEHttpClient : AFHTTPRequestOperationManager
+(id)sharedHTTPClient;
+(id)sharedHTTPClientWithBaseURL:(NSString *)baseURL;

+(void)getImageInformationWithRequest:(SearchPictureRequest*)request
                     andResponseBlock:(PERequestResponse)block;

+(void)getImageInformationWithBaseURL:(NSString*)baseURL Request:(SearchPictureRequest*)request
                     andResponseBlock:(PERequestResponse)block;

+(void (^)(AFHTTPRequestOperation *operation, NSError *error))failureBlockWithResponseBlock:(PERequestResponse)block;
+(void)cancelRequests;


@end
