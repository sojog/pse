//
//  ViewController.m
//  BuilduingsRecognition
//
//  Created by stefan on 10/26/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "CameraViewController.h"
#import <AudioToolbox/AudioToolbox.h>

#import "AppDelegate.h"


#import "VideoSource.h"
#import "PatternDetector.h"


#import "UIImage+OpenCVConvertor.h"
#import "CameraCalibration.h"




@interface CameraViewController ()<VideoSourceDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>{

    PatternDetector * m_detector;
    NSTimer * m_trackingTimer;
    NSTimer * m_sampleTimer;
    
    CameraCalibration m_calibration;
    CGFloat m_targetViewWidth;
    CGFloat m_targetViewHeight;
    
   UIImagePickerController *imagePickerController;
}

// Data Properties
@property (nonatomic, strong) VideoSource * videoSource;

// Transition Closures
@property (nonatomic, copy) void (^transitioningTrackerComplete)(void);
@property (nonatomic, copy) void (^transitioningTrackerCompleteResetScore)(void);

@end

@implementation CameraViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    // Configure Video Source
    self.videoSource = [[VideoSource alloc] init];
    self.videoSource.delegate = self;
    [self.videoSource startWithDevicePosition:AVCaptureDevicePositionBack];
    
     [_matchingLabel setHidden:YES];

    
    // Configure Pattern Detector
    _trackerImage  = [UIImage imageNamed:@"target.jpg"];
    m_detector = new PatternDetector([_trackerImage toCVMat]);
    
    // Start the Tracking Timer
    m_trackingTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0f/20.0f)
                                                       target:self
                                                     selector:@selector(updateTracking:)
                                                     userInfo:nil
                                                      repeats:YES];
    
}

#pragma mark -
#pragma mark Tracking Methods
- (void)updateTracking:(NSTimer*)timer {
    // Tracking Success
    if ( m_detector->isTracking() ) {
    
        
        // Begin tracking the bullseye target
        cv::Point2f matchPoint = m_detector->matchPoint();
//        NSLog(@"matching point is %@", NSStringFromCGPoint(matchPoint) );
        [_matchingLabel setHidden:NO];
    }
    
    // Tracking Failure
    else {
        
        [_matchingLabel setHidden:YES];
        // Stop tracking
     
    }
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
#pragma mark -
#pragma mark VideoSource Delegate
- (void)frameReady:(VideoFrame)frame {
    __weak typeof(self) _weakSelf = self;
    dispatch_sync( dispatch_get_main_queue(), ^{
        // (1) Construct CGContextRef from VideoFrame
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef newContext = CGBitmapContextCreate(frame.data,
                                                        frame.width,
                                                        frame.height,
                                                        8,
                                                        frame.stride,
                                                        colorSpace,
                                                        kCGBitmapByteOrder32Little |
                                                        kCGImageAlphaPremultipliedFirst);
        
        // (2) Construct CGImageRef from CGContextRef
        CGImageRef newImage = CGBitmapContextCreateImage(newContext);
        CGContextRelease(newContext);
        CGColorSpaceRelease(colorSpace);
        
        // (3) Construct UIImage from CGImageRef
        UIImage * image = [UIImage imageWithCGImage:newImage];
        CGImageRelease(newImage);
        [[_weakSelf backgroundImageView] setImage:image];
    });
    
    m_detector->scanFrame(frame);
}


- (IBAction)chooseTrackingPicture:(UIButton *)sender {
    if(_trackerImage){
        
        _trackerImage=nil;
    }
    
    imagePickerController = [[UIImagePickerController alloc] init];
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeSavedPhotosAlbum])
    {
        [imagePickerController setSourceType:UIImagePickerControllerSourceTypeSavedPhotosAlbum];
    }
    
    // image picker needs a delegate,
    [imagePickerController setDelegate:self];
    
    imagePickerController.allowsEditing=YES;
    //       imagePickerController.wantsFullScreenLayout = YES;
    
    
    // Place image picker on the screen
    //        [self presentModalViewController:imagePickerController animated:YES];
    [self presentViewController:imagePickerController animated:NO completion:nil];

}
#pragma  mark-
#pragma  mark- Image Picker
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{

    _trackerImage = [info objectForKey:UIImagePickerControllerEditedImage];
   [self finishAndUpdate];
 
    
}

- (void)finishAndUpdate
{
    [self dismissViewControllerAnimated:YES completion:NULL];
    
    imagePickerController = nil;
    
      m_detector = new PatternDetector([_trackerImage toCVMat]);
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return interfaceOrientation == UIInterfaceOrientationPortrait;
}

@end
