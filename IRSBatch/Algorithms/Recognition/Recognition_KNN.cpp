#include "stdafx.h"

#include "Recognition_KNN.h"

#include "Reporter.h"

#include "cv_utils/cv_utils.h"

#include <string>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace Algorithms
{
	using namespace cv_utils;
	static constexpr int MIN_CONTOUR_AREA = 50;
	static constexpr int RESIZED_IMAGE_WIDTH = 20;
	static constexpr int RESIZED_IMAGE_HEIGHT = 30;

	namespace cv_impl
	{
		void Trace(cv::Exception& e, std::shared_ptr<Reporters::Reporter> reporter)
		{
			if (reporter) {
				reporter->ReportMultiple(L"Exception", Reporters::NewLine,
										 L"File: ", e.file.c_str(), Reporters::NewLine,
										 L"Func: ", e.func.c_str(), Reporters::NewLine,
										 L"Line: ", e.line, Reporters::NewLine,
										 L"Code: ", e.code, Reporters::NewLine,
										 L"Message: ", e.msg.c_str(), Reporters::NewLine);
			}
		}

		// possible chars we are interested in are digits 0 through 9 and capital letters A through Z, put these in vector intValidChars
		std::vector<int> intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V', 'W', 'X', 'Y', 'Z' };
		cv::Mat imgTrainingNumbers;         // input image
		cv::Mat imgGrayscale;               // 
		cv::Mat imgBlurred;                 // declare various images
		cv::Mat imgThresh;                  //
		cv::Mat imgThreshCopy;              //


		void TrainOnImage(cv::Mat& matTrainingImagesAsFlattenedFloats, cv::Mat& matClassificationInts, const std::string& im_path, int sym)
		{
			using Contours = std::vector<std::vector<cv::Point>>;
			std::unique_ptr<Contours> ptContoursPtr(new Contours{});
			Contours ptContours;
			std::vector<cv::Vec4i> v4iHierarchy;                    // declare contours hierarchy

			imgTrainingNumbers = cv::imread(im_path);          // read in training numbers image

			if (imgTrainingNumbers.empty()) {                               // if unable to open image
				std::cout << "error: image not read from file\n\n";         // show error message on command line
				return;                                                  // and exit program
			}

			cv::cvtColor(imgTrainingNumbers, imgGrayscale, CV_BGR2GRAY);        // convert to grayscale
			cv::threshold(imgGrayscale, imgThresh, 127, 255, cv::THRESH_BINARY);
			imgThreshCopy = imgGrayscale.clone();          // make a copy of the thresh image, this in necessary b/c findContours modifies the image
			cv::findContours(imgThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
							 ptContours,                             // output contours
							 v4iHierarchy,                           // output hierarchy
							 cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
							 cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

			std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
			std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly

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

			if (validContoursWithData.size() > 1u)
			{
				return;
			}

			cv::Rect boundingRect = cv::boundingRect(validContoursWithData[0].ptContour);                // get the bounding rect

			cv::Mat matROI = imgGrayscale(boundingRect);           // get ROI image of bounding rect

			cv::Mat matROIResized;
			cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

			matClassificationInts.push_back(sym);       // append classification char to integer list of chars
			cv::Mat matImageFloat;                          // now add the training image (some conversion is necessary first) . . .
			matROIResized.convertTo(matImageFloat, CV_32FC1);       // convert Mat to float
			cv::Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);       // flatten
			matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);       // add to Mat as though it was a vector, this is necessary due to the
																						// data types that KNearest.train accepts
		}

		void Train(std::shared_ptr<Reporters::Reporter> reporter, const Recognition_Parameters& parameters)
		{
			//std::string path_to_data = "G:/Development/Neural/input/A_Z_Handwritten_Data/output/";
			const std::string path_to_data = convert_to_string(parameters.path_to_samples);
			const std::string path_to_classes = convert_to_string(parameters.classification_file);
			const std::string path_to_images = convert_to_string(parameters.images_file);

			// these are our training images, due to the data types that the KNN object KNearest requires, we have to declare a single Mat,
			// then append to it as though it's a vector, also we will have to perform some conversions before writing to file later
			cv::Mat matTrainingImagesAsFlattenedFloats;
			cv::Mat matClassificationInts;      // these are our training classifications, note we will have to perform some conversions before writing to file later
			for (auto& p : fs::directory_iterator(path_to_data))
			{
				if (fs::is_directory(p))
				{
					std::wstring dir_name = p.path().filename().c_str();
					int sym = dir_name[0];

					if (std::find(intValidChars.begin(), intValidChars.end(), sym) == intValidChars.end())
						continue;

					int count = 0;
					for (auto& f_name : fs::directory_iterator(p))
					{
						if (count >= parameters.max_samples_for_symbol)
							break;
						try
						{
							TrainOnImage(matTrainingImagesAsFlattenedFloats, matClassificationInts, f_name.path().generic_string(), sym);
							++count;
						}
						catch (cv::Exception& e)
						{
							Trace(e, reporter);
						}
					}
				}
			} // complete training

			cv::FileStorage fsClassifications(path_to_classes, cv::FileStorage::WRITE);           // open the classifications file

			if (fsClassifications.isOpened() == false) {
				if (reporter) {
					reporter->ReportMultiple("error, unable to open training classifications file, exiting program", Reporters::NewLine);
				}
				return;
			}

			fsClassifications << "classifications" << matClassificationInts;        // write classifications into classifications section of classifications file
			fsClassifications.release();                                            // close the classifications file

			cv::FileStorage fsTrainingImages(path_to_images, cv::FileStorage::WRITE);         // open the training images file

			if (fsTrainingImages.isOpened() == false) {            
				reporter->ReportMultiple("error, unable to open training images file, exiting program", Reporters::NewLine);
				return;
			}

			fsTrainingImages << "images" << matTrainingImagesAsFlattenedFloats;         // write training images into images section of images file
			fsTrainingImages.release();                                                 // close the training images file
			//cv::imwrite("test_images.png", matTrainingImagesAsFlattenedFloats);
		}
	}

	Recognition_KNN::Recognition_KNN(const std::wstring i_analyzer_name, int i_available_int)
		: IAnalyzer<Recognition_Parameters>(i_analyzer_name)
		, m_mode(i_available_int)
	{
	}

	Recognition_KNN::~Recognition_KNN()
	{

	}


	bool Recognition_KNN::IsAvailableInMode(int i_mode) const
	{
		return i_mode == m_mode;
	}

	std::string RecognizeData(const std::string& file_name, const std::string& classification_file, const std::string& image_file,
							  std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter, bool show_debug_image)
	{
		if (i_reporter)
		{
			i_reporter->ReportMultiple(L"Begin recognition: ", file_name.c_str(), Reporters::NewLine,
									   L"Classes: ", classification_file.c_str(), Reporters::NewLine,
									   L"Images: ", image_file.c_str(), Reporters::NewLine);
		}


		cv::Mat matClassificationInts;      // we will read the classification numbers into this variable as though it is a vector

		cv::FileStorage fsClassifications(classification_file, cv::FileStorage::READ);        // open the classifications file

		if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
			if (i_reporter)
			{
				i_reporter->ReportMultiple(L"Error: unable to open training classifications file, exiting program", Reporters::NewLine);
			}
			return std::string{};
		}

		fsClassifications["classifications"] >> matClassificationInts;      // read classifications section into Mat classifications variable
		fsClassifications.release();                                        // close the classifications file

																			// read in training images ////////////////////////////////////////////////////////////

		cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector

		cv::FileStorage fsTrainingImages(image_file, cv::FileStorage::READ);          // open the training images file

		if (fsTrainingImages.isOpened() == false) {
			if (i_reporter) {
				i_reporter->ReportMultiple(L"Error: unable to open training images file, exiting program", Reporters::NewLine);
			}
			return std::string{};
		}

		fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
		fsTrainingImages.release();                                                 // close the traning images file

																					// train //////////////////////////////////////////////////////////////////////////////

		cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());            // instantiate the KNN object

																					// finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
																					// even though in reality they are multiple images / numbers
		kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

		// test ///////////////////////////////////////////////////////////////////////////////

		cv::Mat matTestingNumbers = cv::imread(file_name);            // read in the test numbers image

		if (matTestingNumbers.empty()) {
			if (i_reporter) {
				i_reporter->ReportMultiple(L"Error: image not read from file", Reporters::NewLine);
			}
			return std::string{};
		}

		cv::Mat prepared = cv_utils::prepare_image(matTestingNumbers);
		std::vector<cv_utils::ContourWithData> validContoursWithData = cv_utils::find_contours(prepared);

		std::string strFinalString;         // declare final string, this will have the final number sequence by the end of the program

		cv::Scalar colors[7] = {
			cv::Scalar(255, 0, 0),
			cv::Scalar(0, 255, 0),
			cv::Scalar(0, 0, 255),
			cv::Scalar(255, 255, 0),
			cv::Scalar(255, 0, 255),
			cv::Scalar(0, 255, 255),
			cv::Scalar(255, 255, 255),
		};

		for (int i = 0; i < validContoursWithData.size(); i++) {            // for each contour

																			// draw a green rect around the current char
			cv::rectangle(matTestingNumbers,                            // draw rectangle on original image
						  validContoursWithData[i].boundingRect,        // rect to draw
						  colors[i % 6],                        // green
						  2);                                           // thickness

			cv::Mat matROI = prepared(validContoursWithData[i].boundingRect);          // get ROI image of bounding rect

			cv::Mat matROIResized;
			cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

			cv::Mat matROIFloat;
			matROIResized.convertTo(matROIFloat, CV_32FC1);             // convert Mat to float, necessary for call to find_nearest

			cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

			cv::Mat matCurrentChar(0, 0, CV_32F);

			kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

			float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

			strFinalString = strFinalString + char(int(fltCurrentChar));        // append current char to full string
		}

		if (i_reporter)
		{
			i_reporter->ReportMultiple(L"Result: ", strFinalString.c_str(), Reporters::NewLine);
			i_reporter->ReportMultiple(L"End recognition", Reporters::NewLine, L"==========================", Reporters::NewLine);
		}

		if (show_debug_image)
		{
			cv::imshow("matTestingNumbers", matTestingNumbers);
			cv::waitKey(0);
		}

		return strFinalString;
	}

	bool Recognition_KNN::Analyze(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
	{
		Recognition_Parameters& params = GetParameters();

		cv::Mat source_image = cv_utils::create_image(params.m_points);
		bool success = cv::imwrite("test.png", source_image);
		if (!success) {
			i_reporter->ReportMultiple(L"Cannot save temporary file", Reporters::NewLine);
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
		try
		{
			const std::string path_to_classes = convert_to_string(GetParameters().classification_file);
			const std::string path_to_images = convert_to_string(GetParameters().images_file);
			RecognizeData("test.png", path_to_classes, path_to_images, i_reporter, params.m_show_contours);
		}
		catch (cv::Exception& e)
		{
			cv_impl::Trace(e, i_reporter);
		}
		catch (...)
		{
			if (i_reporter) {
				i_reporter->ReportMultiple(L"Unknown error", Reporters::NewLine);
			}
		}

		return true;
	}

	void Recognition_KNN::Train(std::shared_ptr<Algorithms::Reporters::Reporter> i_reporter /*= nullptr*/)
	{
		try
		{
			cv_impl::Train(i_reporter, GetParameters());
		}
		catch (cv::Exception& e)
		{
			cv_impl::Trace(e, i_reporter);
		}
		catch (...)
		{
			if (i_reporter) {
				i_reporter->ReportMultiple(L"Unknown error", Reporters::NewLine);
			}
		}
	}

} // Algorithms