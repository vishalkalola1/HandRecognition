//*****************************************************************************
// *   ViewController.h
 

#import <UIKit/UIKit.h>
#import <opencv2/highgui/ios.h>

@interface ViewController : UIViewController<CvPhotoCameraDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate>
{
    CvPhotoCamera* photoCamera;
    UIImageView* resultView;
}

@property (nonatomic, strong) CvPhotoCamera* photoCamera;
@property (nonatomic, strong) IBOutlet UIImageView* imageView;
@property (nonatomic, strong) IBOutlet UIToolbar* toolbar;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* takePhotoButton;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* startCaptureButton;

-(IBAction)takePhotoButtonPressed:(id)sender;
-(IBAction)startCaptureButtonPressed:(id)sender;

@end
