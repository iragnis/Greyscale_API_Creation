#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
using namespace cv;

nlohmann::json conv(const cv::Mat& img)
{
    // Define the window names
    std::string window_original = "original_image";
    std::string window_gray = "gray_image";

    // Check if the image was loaded successfully
    if(img.empty())
    {
        std::cout << "Error: Could not read image file." << std::endl;
        return -1;
    }
   
    nlohmann::json res;
    res["num"] = nlohmann::json::array();
    for (int i = 1; i <= 5; i++)
    {
        nlohmann::json obj;
        // using initializer list
       

        obj["n"] = {{"x", i}};

        res["num"].push_back(obj);
    }
    //std::cout<<res<<std::endl;
    // Convert the image to grayscale
    cv::Mat graymat;
    cvtColor(img, graymat, cv::COLOR_BGR2GRAY);

    // Show the original image
    cv::namedWindow(window_original, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_original, img);

    // Show the grayscale image
    cv::namedWindow(window_gray, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_gray, graymat);

    // Wait for a key press before closing the windows
    cv::waitKey(0);

    return res;
}
