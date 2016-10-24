//
//  ViewController.h
//  Face Analysis
//
//  Created by Haluk Seven on 17/10/2016.
//  Copyright © 2016 A Software Company. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <opencv2/highgui/cap_ios.h>
#import <opencv2/imgproc.hpp>
#import <opencv2/objdetect/objdetect.hpp>
using namespace cv;

@interface ViewController : UIViewController<CvVideoCameraDelegate>

@property (strong, nonatomic) IBOutlet UIImageView *imageView;
@property (strong, nonatomic) IBOutlet UIButton *button;

- (IBAction)btnClicked:(id)sender;

@property (nonatomic, retain) CvVideoCamera* videoCamera;

@end

