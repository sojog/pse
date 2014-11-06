//
//  DetailsViewController.m
//  ObjectRecognition
//
//  Created by stefan on 11/6/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "DetailsViewController.h"

@interface DetailsViewController ()

@end

@implementation DetailsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.imageView setImage:self.image];
    [self.descriptionLabel setText:self.descriptionText];
    [self.nameLabel setText:self.name];
    [self.backButton addTarget:self action:@selector(popBack) forControlEvents:UIControlEventTouchUpInside];
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(void)popBack{
    [self.navigationController popViewControllerAnimated:YES];
}

@end
