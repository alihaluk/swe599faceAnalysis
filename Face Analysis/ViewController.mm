//
//  ViewController.m
//  Face Analysis
//
//  Created by Haluk Seven on 17/10/2016.
//  Copyright © 2016 A Software Company. All rights reserved.
//

#import "ViewController.h"

@interface ViewController () {
    CascadeClassifier haar_cascade;
}

@end

@implementation ViewController

@synthesize videoCamera;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.videoCamera = [[CvVideoCamera alloc] initWithParentView:_imageView];
    self.videoCamera.delegate = self;
    self.videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
    self.videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    self.videoCamera.defaultFPS = 60;

    NSString *xmlPath = [[NSBundle mainBundle] pathForResource:@"haarcascade_frontalface_default" ofType:@"xml"];
    
    if (!haar_cascade.load([xmlPath UTF8String]))
    {
        // can't load xml.
    }
    
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)btnClicked:(id)sender {
     [self.videoCamera start];
}

#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void)processImage:(Mat&)image;
{
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    
    vector< Rect_<int> > faces;
    haar_cascade.detectMultiScale(gray, faces);
    
    for(int i = 0; i < faces.size(); i++) {
        cv::Rect face_i = faces[i];
        Mat face = gray(face_i);
        rectangle(image, face_i, CV_RGB(0, 255,0), 1);
    }
    
}
#endif

@end
