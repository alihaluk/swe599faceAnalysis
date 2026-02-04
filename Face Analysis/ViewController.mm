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
    
    // Find largest face
    float fx = -1.0f, fy = -1.0f, fw = 0.0f, fh = 0.0f;
    cv::Rect largestFace;
    bool hasFace = !faces.empty();

    if (hasFace) {
        largestFace = faces[0];
        for(size_t i = 1; i < faces.size(); i++) {
            if (faces[i].area() > largestFace.area()) {
                largestFace = faces[i];
            }
        }

        fx = (float)largestFace.x / image.cols;
        fy = (float)largestFace.y / image.rows;
        fw = (float)largestFace.width / image.cols;
        fh = (float)largestFace.height / image.rows;
    }

    // Update Game Logic
    gameLogic.update(dt, fx, fy, fw, fh);

    // Draw Game UI
    std::string timeStr = gameLogic.getTimeString();
    std::string statusStr = gameLogic.getStatusString();

    int r, g, b;
    gameLogic.getStatusColor(r, g, b);

    // Draw Target Zone
    NormalizedRect zone = gameLogic.getTargetZone();
    cv::Rect targetRect(zone.x * image.cols, zone.y * image.rows, zone.width * image.cols, zone.height * image.rows);
    cv::rectangle(image, targetRect, CV_RGB(200, 200, 200), 2);

    // Draw Text
    cv::putText(image, "Time: " + timeStr, cv::Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(255, 255, 255), 2);
    cv::putText(image, statusStr, cv::Point(10, 70), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(r, g, b), 2);

    // Draw Faces
    for(int i = 0; i < faces.size(); i++) {
        cv::Rect face_i = faces[i];
        cv::rectangle(image, face_i, CV_RGB(r, g, b), 2);
    }

    // Draw Danger Bar
    float danger = gameLogic.getDangerLevel();
    if (danger > 0) {
        int barHeight = 20;
        int filledWidth = (int)(image.cols * danger);
        cv::rectangle(image, cv::Rect(0, image.rows - barHeight, filledWidth, barHeight), CV_RGB(255, 0, 0), -1);
    }
    
}
#endif

@end
