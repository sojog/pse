//
//  Globals.h
//  QHTV
//
//  Created by Corlade Silviu on 29/01/14.
//  Copyright (c) 2014 SwissImpulse. All rights reserved.
//

#ifndef QHTV_Globals_h
#define QHTV_Globals_h

#pragma mark -
#pragma mark - Utils


typedef enum{
    FrameCaseStradaMini =0,
    FrameCaseStrada1 =1,
    FrameCaseStrada2 =2,
    FrameCasePliabila =3,
}Category;


#define IS_IPHONE5              (([[UIScreen mainScreen] bounds].size.height-568)?NO:YES)
#define IS_OS_6_OR_LATER        ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0)
#define IS_OS_7_OR_LATER        ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
#define IS_IPAD                 (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#define NULL_TO_NIL(obj)        ({ __typeof__ (obj) __obj = (obj); __obj == [NSNull null] ? nil : obj; })

#define kFirstNavVCId  @"kFirstNavVCId"
#define kSecondNavVCId  @"kSecondNavVCId"
#define kThirdNavVCId  @"kThirdNavVCId"
#define kSubcategoryVCId  @"kSubcategory"
#define kKeyKeyPath  @"key"
#define kIsOpenKeyPath  @"isOpen"
#define kChildKeyPath  @"value"

#endif
