//
//  ViewController.h
//  BuilduingsRecognition
//
//  Created by stefan on 10/26/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CameraViewController : UIViewController
- (IBAction)chooseTrackingPicture:(UIButton *)sender;

@property (weak, nonatomic) IBOutlet UIImageView *backgroundImageView;

@property (weak, nonatomic) IBOutlet UILabel *matchingLabel;

@property (nonatomic, assign) BOOL transitioningTracker;
@property (nonatomic, assign) BOOL transitioningSample;

@property (nonatomic, weak)  UIImage * trackerImage;
@end

