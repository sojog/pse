//
//  MenuViewController.m
//  SlidingMenuTemplate
//
//  Created by Silviu Ojog on 26/08/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "MenuViewController.h"
#import "FINavigationViewController.h"
#import "Globals.h"
#import "InsertionTableViewCell.h"


@interface MenuViewController (){

    NSArray *menuTreeArray;
    InsertionTableViewCell *insertionCell;
    NSString *ipNumber;
    
}

@end

@implementation MenuViewController

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
        NSLog(@"%@",NSStringFromClass([self class]));
    
    UIView *v = [[UIView alloc] initWithFrame:CGRectZero];
    v.backgroundColor = [UIColor clearColor];
    [self.menuTableView setTableFooterView:v];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        [self.slidingViewController setAnchorRightRevealAmount:710.0f];
    }
    else
    {
        [self.slidingViewController setAnchorRightRevealAmount:275.0f];
    }
    self.slidingViewController.underLeftWidthLayout = ECFullWidth;

    
     [self initSlidingMenuElemnts];
    
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


-(void)initSlidingMenuElemnts{

    
    FISlidingMenuItem *slidingMenuItem0 = [[FISlidingMenuItem alloc]initWithItemName:@"News"
                                                                     andStoryBoardId:kFirstNavVCId
                                                                        andImagePath:@"menuSanatatea"];
    _arrMenu = [[NSMutableArray alloc]             initWithObjects:slidingMenuItem0, nil];


    self.datasourceArray=@[_arrMenu];
    [self.menuTableView reloadData];

    
}



#pragma mark -
#pragma mark - UITableViewDatasource

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
   
    return 1;
}

//-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
//{
//    return [[self.datasourceArray objectAtIndex:section] count];
//}

#pragma mark -
#pragma mark - UITableViewDelegate

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    insertionCell=[[InsertionTableViewCell alloc] initWithDefaultNibNameAndDefaultReuseIdentifier];
    [insertionCell.ipTextField setDelegate:self];
    insertionCell.selectionStyle=UITableViewCellSelectionStyleNone;
   return insertionCell;
}






-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
   
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return 208;
    }
    else
    {
        return 108;
    }
}


#pragma mark-
#pragma mark- TextField

-(BOOL) textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    ipNumber=textField.text;
    NSLog(@"ipNumber %@",ipNumber);
    
    if(![ipNumber isEqualToString:@""]){
    
    [[NSUserDefaults standardUserDefaults] setObject:ipNumber forKey:@"ipNumber"];
    [[NSUserDefaults standardUserDefaults] synchronize];
     }
     UIViewController *newTopViewController = [self.storyboard instantiateViewControllerWithIdentifier:@"kFirstNavVCId"];
    
    [self.slidingViewController anchorTopViewOffScreenTo:ECRight animations:nil onComplete:^{
        CGRect frame = self.slidingViewController.topViewController.view.frame;
        self.slidingViewController.topViewController = newTopViewController;
        self.slidingViewController.topViewController.view.frame = frame;
        [self.slidingViewController resetTopView];
  
    }];
   
    
    return YES;
}
@end
