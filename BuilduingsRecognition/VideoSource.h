//
//  VideoSource.h
//  Building Recognition
//
//  Created by stefan on 10/26/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "VideoFrame.h"


#pragma mark -
#pragma mark VideoSource Delegate
@protocol VideoSourceDelegate <NSObject>

@required
- (void)frameReady:(VideoFrame)frame;

@end

@interface VideoSource : NSObject

@property (nonatomic, strong) AVCaptureSession * captureSession;
@property (nonatomic, strong) AVCaptureDeviceInput * deviceInput;
@property (nonatomic, weak) id<VideoSourceDelegate> delegate;

- (BOOL)startWithDevicePosition:(AVCaptureDevicePosition)devicePosition;

@end
