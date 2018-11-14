#pragma once

#include <MathBase/math/Vector3D.h>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

namespace Algorithms
{
	namespace cv_utils
	{

		class ContourWithData
		{
		public:
			std::vector<cv::Point> ptContour;           // contour
			cv::Rect boundingRect;                      // bounding rect for contour
			float fltArea;                              // area of contour

			bool checkIfContourIsValid(std::vector<ContourWithData>& contours);
			static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight);

		};

		cv::Mat create_image(const std::vector<Vector3D>& points);
		bool point_in_rect(const cv::Rect& rect, cv::Point2i pt);
		cv::Mat prepare_image(const cv::Mat& input);
		std::vector<ContourWithData> find_contours(const cv::Mat& input);
	}
}