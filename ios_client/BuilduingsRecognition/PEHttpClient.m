//
//  FIHttpClient.m
//  ObjectRecognition
//
//  Created by Silviu Ojog on 05/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "PEHttpClient.h"
#define kBasePath               @"http://192.168.0.11:8000"
#define kPathIdentifyPaintings  @"identify_painting/"


@implementation PEHttpClient
static PEHttpClient *sharedClient = nil;

+(id)sharedHTTPClient
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedClient = [[self alloc] initWithBaseURL:[NSURL URLWithString:kBasePath]];
        
        
        sharedClient.responseSerializer = [AFJSONResponseSerializer serializer];
        sharedClient.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/html",@"text/plain",nil];
        sharedClient.requestSerializer = [AFJSONRequestSerializer serializer];
        [sharedClient.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
        
    });
    
    NSOperationQueue *operationQueue = sharedClient.operationQueue;
    
    [sharedClient.operationQueue setMaxConcurrentOperationCount:5];
    
    [sharedClient.reachabilityManager setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        switch (status) {
            case AFNetworkReachabilityStatusReachableViaWWAN:
            case AFNetworkReachabilityStatusReachableViaWiFi:
                [operationQueue setSuspended:NO];
                
                break;
            case AFNetworkReachabilityStatusNotReachable:
                
            default:
                //                [RestClient noInternetConnectionAlert];
                [operationQueue setSuspended:NO];
                break;
        }
    }];
    
    return sharedClient;
}
+(id)sharedHTTPClientWithBaseURL:(NSString *)baseURL
{
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedClient = [[self alloc] initWithBaseURL:[NSURL URLWithString:baseURL]];
        
        
        sharedClient.responseSerializer = [AFJSONResponseSerializer serializer];
        sharedClient.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/html",@"text/plain",nil];
        sharedClient.requestSerializer = [AFJSONRequestSerializer serializer];
        [sharedClient.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
        
    });
    
    NSOperationQueue *operationQueue = sharedClient.operationQueue;
    
    [sharedClient.operationQueue setMaxConcurrentOperationCount:5];
    
    [sharedClient.reachabilityManager setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        switch (status) {
            case AFNetworkReachabilityStatusReachableViaWWAN:
            case AFNetworkReachabilityStatusReachableViaWiFi:
                [operationQueue setSuspended:NO];
                
                break;
            case AFNetworkReachabilityStatusNotReachable:
                
            default:
                //                [RestClient noInternetConnectionAlert];
                [operationQueue setSuspended:NO];
                break;
        }
    }];
    
    return sharedClient;
}


+(void)getImageInformationWithRequest:(SearchPictureRequest*)request
                     andResponseBlock:(PERequestResponse)block
{
    
    
    [[PEHttpClient sharedHTTPClient] GET: [NSString stringWithFormat:@"%@" ,kPathIdentifyPaintings]
                               parameters:nil
                                  success:[PEHttpClient successBlockWithResponseBlock: block andRequest:request]
                                  failure:[PEHttpClient failureBlockWithResponseBlock:block]];
    
}


////variable baseURL


+(void)getImageInformationWithBaseURL:(NSString*)baseURL Request:(SearchPictureRequest*)request
                     andResponseBlock:(PERequestResponse)block
{
    
    
    [[PEHttpClient sharedHTTPClientWithBaseURL:baseURL] POST: [NSString stringWithFormat:@"%@" ,kPathIdentifyPaintings]
                                                  parameters:[request dictionaryForm]
                                                     success:[PEHttpClient successBlockWithResponseBlock: block andRequest:request]
                                                     failure:[PEHttpClient failureBlockWithResponseBlock:block]];
    
}



+(void)cancelRequests
{
    [[[PEHttpClient sharedHTTPClient] operationQueue] cancelAllOperations];
    
}


#pragma mark Helpers
+(void (^)(AFHTTPRequestOperation *operation, id responseObject))successBlockWithResponseBlock:(PERequestResponse)block
                                                                                    andRequest:(PEBaseRequest*)request
{
    return ^(AFHTTPRequestOperation *operation, id responseObject)
    {
        if(operation.responseString)
        {
            NSLog(@"OPERATION RESPONSE STRING: %@", operation.responseString);
            NSLog(@"OPERATION RESPONSE: %@ and status code %d", operation.response, operation.response.statusCode);
            NSString * json = [NSString stringWithString:operation.responseString ];
            
            SBJsonParser * parser = [[SBJsonParser alloc] init];
            
            NSDictionary *dict = [parser objectWithString:json];
            
            PEBaseResponse *fiResponse;
            
            if (dict)
            {
                
                if(operation.response.statusCode==200)
                {
                    if(block)
                    {
                        block (fiResponse, nil);
                    }
                    
                }
                else
                {
                    if(block)
                    {
                        NSError* error = [[NSError alloc] initWithDomain:@"com.adonissoft" code:-404 userInfo:nil];
                        
                        block(nil, error);
                    }
                    
                }
            }
            
            else
            {
                if(block)
                {
                    NSError* error = [[NSError alloc] initWithDomain:@"com.adonissoft" code:-404 userInfo:nil];
                    
                    block(nil, error);
                }
            }
            
        }
    };
}

+(void (^)(AFHTTPRequestOperation *operation, NSError *error))failureBlockWithResponseBlock:(PERequestResponse)block
{
    
    
    return ^(AFHTTPRequestOperation *operation, NSError *error)
    {
        if (operation.isCancelled)
        {
            return;
        }
        
        NSLog(@"operation: %@",operation.responseString);
        if(block)
        {
            if (block)
            {
                block(nil, error);
            }
        }
        
    };
}
+(BOOL)isRequestOk:(NSDictionary*)resultCode
{
    return  [[resultCode objectForKey:@"status"]integerValue]==200;
    
}


@end
