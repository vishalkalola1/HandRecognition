# HandRecognition Using OpenCV FrameWork

## Requirements ##

Xcode 8+

iOS 8+

ARC

## Basic usage ##


### Detect Hand ###

```
-(void)handDetect:(UIImage *)image1 {
    
    if(image1) {
        cvSetErrMode(CV_ErrModeParent);
        
        IplImage *image = [self
                           CreateIplImageFromUIImage:image1];
        IplImage *result = cvCreateImage(cvGetSize(image), 8, 1);
        
        cvSmooth(image, image, CV_MEDIAN, 9, 9, 0, 0);
        
        int height = image->height;
        int width = image->width;
        int step = image->widthStep;
        int channels = image->nChannels;
        uchar *data = (uchar *)image->imageData;
        int stepr = result->widthStep;
        int channelsr = result->nChannels;
        uchar *datar = (uchar *)result->imageData;
        
        NSLog(@"search for redder pixels");
        for(int i=0;i<(height);i++) {
            for(int j=0;j<(width);j++) {
                if(((data[i*step+j*channels+2])>(29>data[i*step+j*channels])) &&
                   ((data[i*step+j*channels+2])>(29+data[i*step+j*channels+1]))) {
                    datar[i*stepr+j*channelsr]=255;
                } else {
                    datar[i*stepr+j*channelsr]=0;
                }
            }
        }
        
        NSLog(@"redder pixels found");
        IplImage *img_8uc1 = result;
        IplImage *img_edge = cvCreateImage(cvGetSize(img_8uc1), 8,
                                           1);
        IplImage *img_8uc3 = cvCreateImage(cvGetSize(img_8uc1), 8,
                                           3);
        cvThreshold(img_8uc1, img_edge, 128, 255, CV_THRESH_BINARY);
        CvMemStorage *storage = cvCreateMemStorage(0);
        CvSeq* first_contour = NULL;
        IplImage *bg = cvCreateImage(cvGetSize(result), 8, 3);
        cvFindContours(img_edge, storage, &first_contour,
                       sizeof(CvContour),
                       CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
        CvSeq *contours2 = NULL;
        double Result, Result2;
        Result = 0, Result2 = 0;
        
        while (first_contour) {
            Result = fabs(cvContourArea(first_contour,
                                        CV_WHOLE_SEQ));
            
            if (Result > Result2) {
                Result2 = Result;
                contours2 = first_contour;
                
            }
            
            first_contour = first_contour->h_next;
        }
        CvSeq *hull;
        CvSeq *defect;
        
        if(contours2) {
            CvMemStorage *minStorage = cvCreateMemStorage(0);
            CvMemStorage *dftStorage = cvCreateMemStorage(0);
            CvRect rect = cvBoundingRect(contours2, 0);
            cvRectangle(bg, cvPoint(rect.x, rect.y +rect.height),
                        cvPoint(rect.x +
                                rect.width, rect.y), CV_RGB(200,0,200), 1, 8, 0);
            int checkcxt = cvCheckContourConvexity(contours2);
            NSLog(@"contour convexity checked");
            hull = cvConvexHull2(contours2, 0, CV_CLOCKWISE, 0);
            defect = cvConvexityDefects(contours2, hull,
                                        dftStorage);
            CvBox2D box = cvMinAreaRect2(contours2, minStorage);
            cvCircle(bg, cvPoint(box.center.x, box.center.y), 3,
                     CV_RGB(200,0,200), 2
                     , 8, 0);
            cvEllipse(bg, cvPoint(box.center.x, box.center.y),
                      cvSize(box.size.height
                             /2, box.size.width/2), box.angle, 0, 360, CV_RGB(220, 0, 220), 1, 8, 0);
        }
        NSLog(@"stuff drawn");
        
        NSLog(@"drawing contours");
        cvDrawContours(bg, contours2, CV_RGB(255,255,255),
                       CV_RGB(0,0,0), 1, -1, 8, cvPoint(0,0));
        
        int hulltotal = hull->total;
        int defecttotal = defect->total;
        
        NSLog(@"%d, %d", hulltotal, defecttotal);
        
        CvConvexityDefect *defects = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*defecttotal);
        cvCvtSeqToArray(defect, defects, CV_WHOLE_SEQ);
        
        for(int i = 0; i < defecttotal; i++) {
            cvLine(bg, *(defects[i].start),*(defects[i].depth_point), CV_RGB(0,0,255), 2, 8, 0);
            cvLine(bg, *(defects[i].depth_point),*(defects[i].end), CV_RGB(0,0,255),2, 8, 0);
            
            CvPoint startPoint = *(defects[i].start);
            CvPoint depthPoint = *(defects[i].depth_point);
            CvPoint endPoint = *(defects[i].end);
            
                  NSLog(@"%d, %d", startPoint.x, startPoint.y);
                  NSLog(@"%d, %d", depthPoint.x, depthPoint.y);
                  NSLog(@"%d, %d", endPoint.x, endPoint.y);
                  }
                  UIImage *image1 = [self UIImageFromIplImage:bg];
                    imageView.image = image1;
          }
  }
  ```


### Convert IPLImage To UIImage ###

```
- (UIImage*)UIImageFromIplImage:(IplImage*)image {
    CGColorSpaceRef colorSpace;
    if (image->nChannels == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    }
    else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
        cvCvtColor(image, image, CV_BGR2RGB);
    }
    NSData *data = [NSData dataWithBytes:image->imageData length:image->imageSize];
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)data);
    CGImageRef imageRef = CGImageCreate(image->width,
                                        image->height,
                                        image->depth,
                                        image->depth * image->nChannels,
                                        image->widthStep,
                                        colorSpace,
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,
                                        provider,
                                        NULL,
                                        false,
                                        kCGRenderingIntentDefault
                                        );
    UIImage *ret = [UIImage imageWithCGImage:imageRef];
    
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return ret;
}
```

### Convert UIimage To IPLImage ###

```
- (IplImage *)CreateIplImageFromUIImage:(UIImage *)image {
    // Getting CGImage from UIImage
    CGImageRef imageRef = image.CGImage;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    // Creating temporal IplImage for drawing
    IplImage *iplimage = cvCreateImage(
                                       cvSize(image.size.width,image.size.height), IPL_DEPTH_8U, 4
                                       );
    // Creating CGContext for temporal IplImage
    CGContextRef contextRef = CGBitmapContextCreate(
                                                    iplimage->imageData, iplimage->width, iplimage->height,
                                                    iplimage->depth, iplimage->widthStep,
                                                    colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault
                                                    );
    // Drawing CGImage to CGContext
    CGContextDrawImage(
                       contextRef,
                       CGRectMake(0, 0, image.size.width, image.size.height),
                       imageRef
                       );
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
    
    // Creating result IplImage
    IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, 3);
    cvCvtColor(iplimage, ret, CV_RGBA2BGR);
    cvReleaseImage(&iplimage);
    
    return ret;
}
```
