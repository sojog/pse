//
//  CameraPreViewController.m
//  BuildingRecognition
//
//  Created by stefan on 11/4/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "CameraPreViewController.h"
#import "MenuViewController.h"
#import "PEHttpClient.h"
#import "SearchPictureRequest.h"
#import "SearchPictureResponse.h"
#import "DetailsViewController.h"


@interface CameraPreViewController (){
    int     cameraOrientation;
    AVCaptureVideoPreviewLayer *newCaptureVideoPreviewLayer;
    NSString *ipNumber;
    UIImage *currentImage;
    NSString *currentName;
    NSString *currrentDescription;
    
}

@end

@implementation CameraPreViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"%@",NSStringFromClass([self class]));
    // Do any additional setup after loading the view.
    
    NSError *error = nil;
    AVCaptureSession *avCaptureSession = [[AVCaptureSession alloc] init];
    AVCaptureDevice *videoCaptureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    AVCaptureDeviceInput *videoInput = [AVCaptureDeviceInput deviceInputWithDevice:videoCaptureDevice error:&error];
    
    if (videoInput) {
        [avCaptureSession addInput:videoInput];
    }
    else {
        // Handle the failure.
    }
    
    newCaptureVideoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:avCaptureSession];
    
    [[self.view layer] setMasksToBounds:YES];
    [newCaptureVideoPreviewLayer setFrame:[self.view bounds]];
    [newCaptureVideoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    
    if ([[newCaptureVideoPreviewLayer connection] isVideoOrientationSupported])
    {//TODO silviu: support both orientations
        [[newCaptureVideoPreviewLayer connection] setVideoOrientation:1];
    }
    [newCaptureVideoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    [[self.view layer] insertSublayer:newCaptureVideoPreviewLayer below:[[[self.view layer] sublayers] objectAtIndex:0]];
    
    [self setPreviewLayer:newCaptureVideoPreviewLayer];
    
    [avCaptureSession setSessionPreset:AVCaptureSessionPresetHigh];
    [avCaptureSession startRunning];
    
    [self setCaptureSession:avCaptureSession];
    
    
    UISwipeGestureRecognizer *swipeGesture= [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(revealMenu:)];
    [self.view addGestureRecognizer:swipeGesture];
    
    UITapGestureRecognizer *singleFingerTap =
    [[UITapGestureRecognizer alloc] initWithTarget:self
                                            action:@selector(requestPictureIdentification:)];
    
    [self.view addGestureRecognizer:singleFingerTap];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        [self.slidingViewController setAnchorRightRevealAmount:710.0f];
    }
    else
    {
        [self.slidingViewController setAnchorRightRevealAmount:275.0f];
    }
    self.slidingViewController.underLeftWidthLayout = ECFullWidth;
    
    if (![self.slidingViewController.underLeftViewController isKindOfClass:[MenuViewController class]]) {
        self.slidingViewController.underLeftViewController  = [self.storyboard instantiateViewControllerWithIdentifier:@"LeftMenu"];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"ipNumber"]!=nil) {
        NSLog(@"ip Number %@",[[NSUserDefaults standardUserDefaults] objectForKey:@"ipNumber"]);
        ipNumber=[[NSUserDefaults standardUserDefaults] objectForKey:@"ipNumber"];
    }
    else
    {
    ipNumber=@"192.168.0.11:8000";
        
           NSLog(@"ip Number %@",ipNumber);
    }
    
}

#pragma mark - Navigation

- (IBAction)revealMenu:(id)sender
{
    //Tutorial future added
    
    
    [self.slidingViewController anchorTopViewTo:ECRight];
    
}

#pragma mark - Services
-(void)requestPictureIdentification:(UIGestureRecognizer*)recognizer{
    
    if ([recognizer state] == UIGestureRecognizerStateEnded) {
        NSInteger nr= [recognizer numberOfTouches];
        
        CGPoint lastPoint= [recognizer locationOfTouch:nr-1 inView:self.view];
        NSLog(@"last point %@", NSStringFromCGPoint(lastPoint));
        NSString *xCoordinate = [NSString stringWithFormat:@"%f",2*lastPoint.x ];
        NSString *yCoordinate = [NSString stringWithFormat:@"%f",2*lastPoint.y ];
        
        //        UIGraphicsBeginImageContext(appDelegate.window.bounds.size);
        UIGraphicsBeginImageContext(self.view.bounds.size);
        
        [newCaptureVideoPreviewLayer.presentationLayer renderInContext:UIGraphicsGetCurrentContext()];
        
        
        //    [appDelegate.window.layer renderInContext:UIGraphicsGetCurrentContext()];
        
        UIImage *screenshot = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        NSLog(@"%@ image", screenshot);
        /*
        if (screenshot) {
            [PEHttpClient getImageInformationWithBaseURL:@"http://192.168.10.117:8000/" Request:[[SearchPictureRequest alloc] initWithPhoto:screenshot xCoordinate:xCoordinate yCoordinate:yCoordinate]  andResponseBlock:^(PEBaseResponse *response, NSError *error) {
               
              [self performSegueWithIdentifier:NSStringFromClass([DetailsViewController class]) sender:nil];
                NSLog(@"response ..%@",response);
                NSLog(@"error ..%@", error);
            }];
         
          }
            */
        
        
            //other test
            
            if (screenshot) {
                [PEHttpClient getImageInformationWithBaseURL:@"http://192.168.10.117:8000/" image:screenshot Request:[[SearchPictureRequest alloc] initWithPhoto:screenshot xCoordinate:xCoordinate yCoordinate:yCoordinate] andResponseBlock:^(PEBaseResponse *response, NSError *error) {
                    [self performSegueWithIdentifier:NSStringFromClass([DetailsViewController class]) sender:nil];
                    NSLog(@"response ..%@",response);
                    NSLog(@"error ..%@", error);
                }];
            }
            
            
            
            
       

        
        
        
    }
    
}

+ (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo {
}

#pragma mark -
#pragma mark Device Orientation

- (void)currentDeviceOrientation
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
    if (orientation != UIDeviceOrientationUnknown && orientation != UIDeviceOrientationFaceUp && orientation != UIDeviceOrientationFaceDown) {
        switch (orientation) {
            case UIDeviceOrientationLandscapeLeft:
                cameraOrientation = AVCaptureVideoOrientationLandscapeRight;
                break;
            case UIDeviceOrientationLandscapeRight:
                cameraOrientation = AVCaptureVideoOrientationLandscapeLeft;
                break;
            case UIDeviceOrientationPortraitUpsideDown:
                cameraOrientation = AVCaptureVideoOrientationPortraitUpsideDown;
                break;
            case UIDeviceOrientationPortrait:
                cameraOrientation = AVCaptureVideoOrientationPortrait;
                break;
            default:
                break;
        }
    }
}


#pragma mark-
#pragma mark- Navigation
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if ([segue.destinationViewController isKindOfClass:[DetailsViewController class]]) {
        DetailsViewController *detailsVC= (DetailsViewController*)segue.destinationViewController;
        
        
        //testing
        detailsVC.image=[UIImage imageNamed:@"mona"];
        detailsVC.name=@"some text";
        detailsVC.descriptionText=@"Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";
        
       
        /*
         
         detailsVC.image=currentImage;
         detailsVC.name=currentName;
         detailsVC.descriptionText=currrentDescription;
         */
    }
    
    
}




@end
