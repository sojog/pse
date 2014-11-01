//
//  VideoViewController.h
//  BuildingRecognition
//
//  Created by sghiaus@leaptools.com on 11/1/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageViewController : UIViewController<UIImagePickerControllerDelegate,
    UINavigationControllerDelegate>

@property (strong, nonatomic) IBOutlet UIImageView *imageView;

@end
