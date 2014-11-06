//
//  DetailsViewController.h
//  ObjectRecognition
//
//  Created by stefan on 11/6/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DetailsViewController : UIViewController


@property (strong, nonatomic) UIImage *image;
@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *descriptionText;
@property (strong, nonatomic) IBOutlet UIImageView *imageView;
@property (strong, nonatomic) IBOutlet UILabel *nameLabel;
@property (strong, nonatomic) IBOutlet UIButton *backButton;
@property (strong, nonatomic) IBOutlet UILabel *descriptionLabel;

@end
