﻿/******************************************************************************
* A simple viewer for Detections provided by different tracking datasets.
* Written in C++ and making use of openCV
*
* Author: Lucas Wals
******************************************************************************/
#pragma once
#include <fstream>
#include <cstdio>
#include <string>
#include <iomanip>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Challenges/UA-DETRAC.h"
#include "Challenges/MOT.h"
#include "Challenges/OTB.h"
#include "dirent.h"

/******************************************************************************
* EXTRA FUNCTIONS FOR DIFFERENT IMAGES TYPES
******************************************************************************/
int pgm_select(const struct dirent *entry)
{
	return strstr(entry->d_name, ".pgm") != NULL;
}

int ppm_select(const struct dirent *entry)
{
	return strstr(entry->d_name, ".ppm") != NULL;
}

int jpg_select(const struct dirent *entry)
{
	return strstr(entry->d_name, ".jpg") != NULL;
}


/******************************************************************************
* MAIN FUNCTION
******************************************************************************/
int main(int argc, char *argv[])
{
	// General variables
	std::string imageFolder;
	std::string saveFolder;
	std::string sequence;
	char* dataset;
	char* detector_name;
	char* detections_file;
	bool saveImages = true;
	// Image files variables
	struct dirent **filelist;
	int fCount = -1;
	//std::vector< std::vector<BoundingBox> > detections;	// list of detections

	saveFolder = ".";
	// executable_name dataset sequence imageFolder det_name det_file
	if (argc >= 5)
	{
		dataset = argv[1];
		sequence = argv[2];
		imageFolder = argv[3];
		detector_name = argv[4];
		detections_file = argv[5];
	}
	else
	{
		std::cout << "Incorrect arguments. See example below: " << std::endl;
		std::cout << "DetectionViewer dataset sequence image_folder detector_name detections_file" << std::endl;
		std::cout << "DetectionViewer UADETRAC MVI_20011 C:/Detrac/MVI_20011 R-CNN C:/Detrac/Detections/MVI_20011.txt" << std::endl;
		return 0;
	}

	// Ejemplo de Ejecucion!
	// DetectionViewer UADETRAC MVI_20011 "C:\Users\win10\Downloads\Datasets\UA-DETRAC\DETRAC-Train-Images\MVI_20011" R-CNN "C:\Users\win10\Downloads\Datasets\UA-DETRAC\DETRAC-Train-Detections\R-CNN\MVI_20011_Det_R-CNN.txt"
	// DetectionViewer MOT MOT16-02 "C:\Users\win10\Downloads\Datasets\MOT16\train\MOT16-02\img1" DPM "C:\Users\win10\Downloads\Datasets\MOT16\train\MOT16-02\det\det.txt"
	// DetectionViewer OTB Ironman "C:\Users\win10\Downloads\Datasets\OTB-100\Ironman\img" "Ground Truth" "C:\Users\win10\Downloads\Datasets\OTB-100\Ironman\groundtruth_rect.txt"
	// Ground Truth must be loaded differently!
	// DetectionViewer MOT MOT16-02 "C:\Users\win10\Downloads\Datasets\MOT16\train\MOT16-02\img1" "Ground Truth" "C:\Users\win10\Downloads\Datasets\MOT16\train\MOT16-02\gt\gt.txt"

	// Dataset class definition
	// TODO > Make a selector of class depending on the "dataset" variable
	//UADETRAC data = UADETRAC(detector_name, detections_file);
	//MOT data = MOT(detector_name, detections_file);
	OTB data = OTB(detector_name, detections_file);
	data.read_detections();
	// TODO > We need to control errors here!!

	// Look for the images files and load them if they exist
	fCount = scandir(imageFolder.c_str(), &filelist, jpg_select, alphasort);
	if (fCount <= 0)
	{
		std::cout << "ERROR -> Input images directory not found or empty" << std::endl;
		return 0;
	}
	else
		std::cout << "Found " << fCount << " images" << std::endl;

	std::cout << "Displaying detections on window..." << std::endl;
	// Show detections on Window
	char filename[255];
	char saveFln[255];
	//std::string window_title[255] = sequence + ' | ' + detector_name;
	cv::Mat image;

	if (saveImages)
		std::cout << "Activated saving of images..." << std::endl;

	cv::namedWindow(sequence + ' ' + detector_name, cv::WINDOW_AUTOSIZE);
	for (int frame = 0; frame < fCount; frame++)
	{
		// Load the current image
		sprintf_s(filename, "%s/%s", imageFolder.c_str(), filelist[frame]->d_name);
		image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		//std::cout << "Filename = " << filelist[frame]->d_name << std::endl;

		std::vector<BoundingBox> frame_detections = data.get_frame_detections(frame);
		
		for (auto b : frame_detections)
			cv::rectangle(image, cv::Point(b.x, b.y), cv::Point(b.x + b.w, b.y + b.h), cv::Scalar(0, 0, 255), 2);

		imshow(sequence + ' ' + detector_name, image);
		cv::waitKey(50);

		if (saveImages)
		{ 
			//cv::imwrite("../../images/Gray_Image.jpg", image);
			sprintf_s(saveFln, "%s/%s", saveFolder, filelist[frame]->d_name);
			cv::imwrite(saveFln , image);
		}
	}
}