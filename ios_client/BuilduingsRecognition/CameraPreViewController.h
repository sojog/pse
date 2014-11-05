//
//  CameraPreViewController.h
//  BuildingRecognition
//
//  Created by stefan on 11/4/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "ECSlidingViewController.h"

@interface CameraPreViewController : UIViewController <UIGestureRecognizerDelegate>

@property (nonatomic, retain) AVCaptureSession          *captureSession;
@property (nonatomic, retain) AVCaptureVideoPreviewLayer *previewLayer;

@end
