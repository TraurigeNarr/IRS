#include "stdafx.h"

#include "cv_utils.h"

namespace Algorithms
{
	namespace cv_utils
	{
		// obviously in a production grade program
		bool ContourWithData::checkIfContourIsValid(std::vector<ContourWithData>& contours)
		{                              
			const cv::Point2i br = boundingRect.br();
			const cv::Point2i tl = boundingRect.tl();

			for (auto& contour : contours)
			{
				if (&contour == this)
					continue;
				bool br_contains = contour.boundingRect.contains(br);
				bool tl_contains = contour.boundingRect.contains(tl);
				if (point_in_rect(contour.boundingRect, br) && point_in_rect(contour.boundingRect, tl))
					return false;
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		bool ContourWithData::sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
			return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
		}

		cv::Mat create_image(const std::vector<Vector3D>& points)
		{
			double max_x = 0;
			double max_y = 0;
			for (const Vector3D& point : points)
			{
				max_x = std::max(point[0], max_x);
				max_y = std::max(point[1], max_y);
			}
			cv::Mat source_image(max_y + 1, max_x + 1, CV_8UC1, cv::Scalar(0));
			for (const Vector3D& point : points)
			{
				int x = static_cast<int>(point.GetX());
				int y = static_cast<int>(point.GetY());
				source_image.at<uchar>(y, x) = 255;
			}

			return source_image;
		}

		bool point_in_rect(const cv::Rect& rect, cv::Point2i pt)
		{
			return rect.x <= pt.x && pt.x <= rect.x + rect.width && rect.y <= pt.y && pt.y <= rect.y + rect.height;
		}
		
		cv::Mat prepare_image(const cv::Mat& input)
		{
			cv::Mat matGrayscale;           //
			cv::Mat matBlurred;             // declare more image variables
			cv::Mat matThresh;              //
			cv::Mat matThreshCopy;          //

			cv::cvtColor(input, matGrayscale, CV_BGR2GRAY);         // convert to grayscale

			cv::GaussianBlur(matGrayscale,              // input image
							 matBlurred,                // output image
							 cv::Size(5, 5),            // smoothing window width and height in pixels
							 0);                        // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value


														// filter image from grayscale to black and white
			cv::adaptiveThreshold(matBlurred,                           // input image
								  matThresh,                            // output image
								  255,                                  // make pixels that pass the threshold full white
								  cv::ADAPTIVE_THRESH_MEAN_C,       // use gaussian rather than mean, seems to give better results
								  cv::THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
								  11,                                    // size of a pixel neighborhood used to calculate threshold value
								  2);                                   // constant subtracted from the mean or weighted mean

			return matThresh;
		}

		std::vector<ContourWithData> find_contours(const cv::Mat& input)
		{
			std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
			std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly

			cv::Mat work_copy = input.clone();

			std::vector<std::vector<cv::Point> > ptContours;        // declare a vector for the contours
			std::vector<cv::Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

			cv::findContours(work_copy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
							 ptContours,                             // output contours
							 v4iHierarchy,                           // output hierarchy
							 cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
							 cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points


			for (int i = 0; i < ptContours.size(); i++) {               // for each contour
				ContourWithData contourWithData;                                                    // instantiate a contour with data object
				contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
				contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);         // get the bounding rect
				contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);               // calculate the contour area
				allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
			}

			for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
				if (allContoursWithData[i].checkIfContourIsValid(allContoursWithData)) {                   // check if valid
					validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
				}
			}
			// sort contours from left to right
			std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

			return validContoursWithData;
		}

	}
}