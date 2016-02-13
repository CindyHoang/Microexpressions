#include <opencv3/imgproc/imgproc.hpp>
#include <opencv3/core/core.hpp>
#include <opencv3/highgui/highgui.hpp>
// include <opencv2/imgcodecs.hpp>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

/**
 * Calculate the dodged value of every pixel
 */
int pixel_dodge(uint basePixel, uint topPixel)
{
    if (topPixel == 255.0)
    {
        return 255;
    }
    else
    {
        return std::min(255, int((basePixel << 8)/(255 - topPixel + 1)));
    }
}

/**
 * Apply dodge effect to a grayscale image 
 */
void dodge_layer(cv::Mat & base, cv::Mat & top, cv::Mat & dst) 
{
    int rows = base.rows;
    int cols = base.cols;

    dst = cv::Mat(rows, cols, CV_8UC1); 
    for ( int i = 0; i < rows; i++ ) 
    {
        for ( int j = 0; j < cols; j++) 
        {
            dst.at<uchar>(i,j) = pixel_dodge(base.at<uchar>(i,j),top.at<uchar>(i,j));
        }
    }
}

/**
 * Apply sketch filter to an image
 */
void sketch(cv::Mat & image, cv::Mat & dst, float smoothing_param) {
    
    int LowH = 0;
    int HighH = 255;

    int LowS = 0;
    int HighS = 255;

    int LowV = 0;
    int HighV = 75;

    createTrackbar("LowH", "Control", &LowH, 255);
    createTrackbar("HighH", "Control", &HighH, 255);

    createTrackbar("LowS", "Control", &LowS, 255);
    createTrackbar("HighS", "Control", &HighS, 255);

    createTrackbar("LowV", "Control", &LowV, 75);
    createTrackbar("HighV", "Control", &HighV, 75);

    Mat imgHSV;
    cvtColor(image, imgHSV, COLOR_BGR2HSV);

    Mat imgThresholded;
    inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded);

    Mat imgB = imgThresholded + Scalar(-75, -75, -75);

    Mat imgC;
    imgB.convertTo(imgC, -1, 2, 0);

    // Create grayscale image
    cv::Mat gray_image;
    cv::cvtColor(imgC, gray_image, cv::COLOR_BGR2GRAY);

    Mat img_hist_equalized;
    equalizeHist(gray_image, img_hist_equalized);

    // Invert grayscale image
    cv::Mat inverted_gray = cv::Scalar::all(255) - img_hist_equalized;
    
    // Blur the inverted image
    //cv::GaussianBlur(inverted_gray, inverted_gray, cv::Size(0,0), smoothing_param);
    cv::addWeighted(inverted_gray, 5, imgC, -5, 0, img_hist_equalized);
    // Apply dodge between the gray image and the inverted gray
    //dodge_layer(gray_image, inverted_gray, dst);
}

int main(int argc, char * argv[])
{
    
    if(argc < 4) {
        fprintf(stdout, "Usage: ./sketch source_dir out_dir smoothing_param\n");
        return 0;
    }

    // grab the specified folder
    DIR *dp;
    struct dirent *ep;
    // char folder[100];

    const char* sourcedir = argv[1];
    const char* outdir = argv[2];
    std::string s_dir(sourcedir);
    std::string o_dir(outdir);

    float smoothing_param = atof(argv[3]);

    // open and iterate over source directory
    dp = opendir (sourcedir);
    if ( dp != NULL) {
        while ( (ep = readdir(dp)) )
        { 
            // process every file with jpg extension
            std::string file_name = ep->d_name;
            std::string extension = file_name.substr(file_name.find_last_of(".") + 1);
            if (extension == "jpg" || extension == "JPG" || extension == "jpeg" || extension == "JPEG") {
                struct stat buffer;
                if(stat((o_dir + "/c" + file_name).c_str(), &buffer) != 0){
                    // read and sketch image, write to specified directory with prefix 'c'
                    cv::Mat image = cv::imread(s_dir + "/" + file_name); 
                    sketch(image,image,smoothing_param);
                    cv::imwrite(o_dir + "/c" + file_name,image);
                    fprintf(stdout, "Finished processing image %s\n", file_name.c_str());
                }
            }
        }
        (void) closedir(dp);
    }
    else 
    {
        fprintf(stderr, "Could not open source directory %s\n", s_dir.c_str());
    }
}