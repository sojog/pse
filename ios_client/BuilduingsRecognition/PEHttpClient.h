//
//  FIHttpClient.h
//  ObjectRecognition
//
//  Created by Silviu Ojog on 05/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <AFNetworking/AFNetworking.h>
#import "AFHTTPRequestOperationManager.h"

#import "PEBaseRequest.h"
#import "PEBaseResponse.h"
#import <SBJson/SBJson.h>



typedef void (^PERequestResponse)(PEBaseResponse *response, NSError* error);
@interface PEHttpClient : AFHTTPRequestOperationManager
+(id)sharedHTTPClient;




+(void (^)(AFHTTPRequestOperation *operation, NSError *error))failureBlockWithResponseBlock:(PERequestResponse)block;
+(void)cancelRequests;


@end
