//
//  UIImage+OpenCVConvertor.h
//  Building Recognition
//
//  Created by stefan on 10/26/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (OpenCVConvertor)
#pragma mark -
#pragma mark Generate UIImage from cv::Mat
+ (UIImage*)fromCVMat:(const cv::Mat&)cvMat;

#pragma mark -
#pragma mark Generate cv::Mat from UIImage
+ (cv::Mat)toCVMat:(UIImage*)image;
- (cv::Mat)toCVMat;


@end
