//
//  DetailsViewController.m
//  ObjectRecognition
//
//  Created by stefan on 11/6/14.
//  Copyright (c) 2014 Silviu Ojog. All rights reserved.
//

#import "DetailsViewController.h"

@interface DetailsViewController  ()

@end

@implementation DetailsViewController


@synthesize fliteController;
@synthesize slt;


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.imageView setImage:self.image];
    [self.descriptionLabel setText:self.descriptionText];
    [self.nameLabel setText:self.name];
    [self.backButton addTarget:self action:@selector(popBack) forControlEvents:UIControlEventTouchUpInside];
    
//    [self.pocketsphinxController startListeningWithLanguageModelAtPath:nil dictionaryAtPath:nil acousticModelAtPath:[AcousticModel pathToModel:@"AcousticModelEnglish"] languageModelIsJSGF:NO];
    
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

-(void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
//    [self.pocketsphinxController  stopListening];

    
//     [self.fliteController say:@"Say it, don't spray it." withVoice:self.slt];
}


-(void)popBack{
    [self.navigationController popViewControllerAnimated:YES];
}


#pragma mark-
#pragma mark- Speech Recognition


- (FliteController *)fliteController {
    if (fliteController == nil) {
        fliteController = [[FliteController alloc] init];
    }
    return fliteController;
}

- (Slt *)slt {
    if (slt == nil) {
        slt = [[Slt alloc] init];
    }
    return slt;
}


- (IBAction)hearTheSpeach:(UIButton *)sender {
    
      [self.fliteController say:self.descriptionText withVoice:self.slt];
}
@end
