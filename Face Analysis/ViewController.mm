//
//  ViewController.m
//  Face Analysis
//
//  Created by Haluk Seven on 17/10/2016.
//  Copyright © 2016 A Software Company. All rights reserved.
//

#import "ViewController.h"
#import "GameLogic.h"
#include <atomic>

@interface ViewController () {
    CascadeClassifier haar_cascade;
    GameLogic gameLogic;
    int64 lastTick;
    std::atomic<bool> startRequested;
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
        NSLog(@"Failed to load haar cascade");
    }
    
    startRequested = false;
    lastTick = cv::getTickCount();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)btnClicked:(id)sender {
     [self.videoCamera start];
     startRequested = true;
}

#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void)processImage:(Mat&)image;
{
    if (startRequested) {
        gameLogic.start();
        lastTick = cv::getTickCount();
        startRequested = false;
    }

    int64 currentTick = cv::getTickCount();
    double dt = (double)(currentTick - lastTick) / cv::getTickFrequency();
    lastTick = currentTick;

    // Clamp dt to avoid huge jumps if paused
    if (dt > 1.0) dt = 0.0;

    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    
    vector< Rect_<int> > faces;
    haar_cascade.detectMultiScale(gray, faces);
    
    bool faceDetected = !faces.empty();

    // Update Game Logic
    gameLogic.update(dt, faceDetected);

    // Draw Game UI
    std::string timeStr = gameLogic.getTimeString();
    std::string statusStr = gameLogic.getStatusString();

    int r, g, b;
    gameLogic.getStatusColor(r, g, b);

    // Draw Text
    cv::putText(image, "Time: " + timeStr, cv::Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(255, 255, 255), 2);
    cv::putText(image, statusStr, cv::Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(r, g, b), 2);

    // Draw Faces
    for(int i = 0; i < faces.size(); i++) {
        cv::Rect face_i = faces[i];
        rectangle(image, face_i, CV_RGB(r, g, b), 2);
    }
    
}
#endif

@end
