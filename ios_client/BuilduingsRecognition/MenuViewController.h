//
//  MenuViewController.h
//  SlidingMenuTemplate
//
//  Created by Silviu Ojog on 26/08/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FISlidingMenuItem.h"

#import <ECSlidingViewController/ECSlidingViewController.h>



@interface MenuViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate>

@property(nonatomic, strong) IBOutlet UITableView *menuTableView;
@property (nonatomic, strong) NSArray *datasourceArray;
@property (nonatomic, strong) NSArray *firstSectionInTV;
@property (nonatomic, strong) NSMutableArray *arrMenu;
@property (strong, nonatomic) IBOutlet UITextField *ipTextField;
-(void)initSlidingMenuElemnts;

@end
