//
//  BaseTableViewCell.m
//  testingAdonis
//
//  Created by Corlade Silviu on 21/03/14.
//
//

#import "BaseTableViewCell.h"

@implementation BaseTableViewCell
{
	NSString *_overriddenReuseIdentifier;
	NSString *_overriddenNibName;
}
#pragma mark -
#pragma mark - Init

-(id)initWithDefaultNibNameAndDefaultReuseIdentifier
{
    return [self initWithNibName:self.class.defaultNibName
                  reuseIdentifier:self.class.defaultReuseIdentifier];
}


- (id)initWithNibName:(NSString *)nibName
       reuseIdentifier:(NSString *)reuseIdentifier
{
    NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:nibName
                                                             owner:nil
                                                           options:nil];
    
	for (id currentObject in topLevelObjects)
    {
		if ([currentObject isKindOfClass:[self class]])
        {
			self = currentObject;
            _overriddenReuseIdentifier = reuseIdentifier;
            _overriddenNibName = nibName;
			break;
		}
	}
    
	return self ? : nil;
}

#pragma mark -
#pragma mark - Lifecycle

- (void)awakeFromNib
{
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

#pragma mark -
#pragma mark - Helpers

+(NSString *)defaultNibName
{
    return NSStringFromClass([self class]);
}

+ (NSString *)defaultReuseIdentifier
{
	return NSStringFromClass([self class]);
}

- (NSString *)reuseIdentifier
{
	return _overriddenReuseIdentifier ? _overriddenReuseIdentifier : self.class.defaultReuseIdentifier;
}

-(NSString *)nibName
{
    return _overriddenNibName ? _overriddenNibName : self.class.defaultNibName;
}

@end
