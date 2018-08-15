/******************************************************************************
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
#include "dirent.h"

/******************************************************************************
* EXTRA FUNCTIONS
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
	std::string image_folder;
	std::string sequence;
	char* dataset;
	char* detector_name;
	char* detections_file;
	// Image files variables
	struct dirent **filelist;
	int fcount = -1;
	//std::vector< std::vector<BoundingBox> > detections;	// list of detections

	//std::cout << "Hello there" << std::endl;

	// executable_name dataset sequence image_folder det_name det_file
	if (argc >= 5)
	{
		dataset = argv[1];
		sequence = argv[2];
		image_folder = argv[3];
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
	
	// Dataset class definition
	UADETRAC data = UADETRAC(detector_name, detections_file);
	data.read_detections();
	// TODO > We need to control errors here!!

	// Look for the images files and load them if they exist
	fcount = scandir(image_folder.c_str(), &filelist, jpg_select, alphasort);
	if (fcount <= 0)
	{
		std::cout << "ERROR -> Input images directory not found or empty" << std::endl;
		return 0;
	}
	else
		std::cout << "Found " << fcount << " images" << std::endl;

	std::cout << "Displaying detections on window..." << std::endl;
	// Show detections on Window
	char filename[255];
	//std::string window_title[255] = sequence + ' ' + detector_name;
	cv::Mat image;


	cv::namedWindow(sequence + ' ' + detector_name, cv::WINDOW_AUTOSIZE);
	for (int frame = 0; frame < fcount; frame++)
	{
		// Load the current image
		sprintf_s(filename, "%s/%s", image_folder.c_str(), filelist[frame]->d_name);
		image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		//std::cout << "Frame " << frame << " of " << fcount << " / Name = " << filelist[frame]->d_name << std::endl;

		std::vector<BoundingBox> frame_detections = data.get_frame_detections(frame);
		
		for (auto b : frame_detections)
			cv::rectangle(image, cv::Point(b.x, b.y), cv::Point(b.x + b.w, b.y + b.h), cv::Scalar(0, 0, 255), 2);

		imshow(sequence + ' ' + detector_name, image);
		cv::waitKey(50);
	}
}