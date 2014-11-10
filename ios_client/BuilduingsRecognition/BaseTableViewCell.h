//
//  BaseTableViewCell.h
//  testingAdonis
//
//  Created by Corlade Silviu on 21/03/14.
//
//

#import <UIKit/UIKit.h>

@interface BaseTableViewCell : UITableViewCell

-(id)initWithDefaultNibNameAndDefaultReuseIdentifier;

-(id)initWithNibName:(NSString *)nibName
       reuseIdentifier:(NSString *)reuseIdentifier;

-(NSString *)reuseIdentifier;

-(NSString *)nibName;

@end
