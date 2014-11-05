//
//  FINavigationViewController.m
//  FarmaciaInimii
//
//  Created by Silviu Ojog on 05/05/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "FINavigationViewController.h"

@interface FINavigationViewController ()

@end

@implementation FINavigationViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

#pragma mark -
#pragma mark - Rotation handlers

//- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
//{
//    return [[self.viewControllers lastObject] preferredInterfaceOrientationForPresentation];
//}

-(NSUInteger)supportedInterfaceOrientations
{
    return [self.topViewController supportedInterfaceOrientations];
    
}

-(BOOL)shouldAutorotate
{
    //    return YES;
    
    return [[self.viewControllers lastObject] shouldAutorotate];
}

@end
