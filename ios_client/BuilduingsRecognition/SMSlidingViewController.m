//
//  SMSlidingViewController.m
//  SlidingMenuTemplate
//
//  Created by Silviu Ojog on 26/08/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "SMSlidingViewController.h"
#import "Globals.h"

@interface SMSlidingViewController ()

@end

@implementation SMSlidingViewController

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
    
        self.topViewController = [self.storyboard instantiateViewControllerWithIdentifier:kFirstNavVCId];
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

-(NSUInteger)supportedInterfaceOrientations
{
    return [self.topViewController supportedInterfaceOrientations];
    
}

-(BOOL)shouldAutorotate
{
    return [self.topViewController shouldAutorotate];
}


@end
