/*
* OTB(Object Tracking Benchmark) Dataset class
* More info on <http://cvlab.hanyang.ac.kr/tracker_benchmark/datasets.html>
*
* Author: Lucas Wals
*/
#pragma once

#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iomanip>

#include "../Dataset.h"

class OTB : public Dataset
{
public:
	OTB(char*, char*);
	virtual int read_detections();	// Whether it could/couldn't read the detections+
};

OTB::OTB(char* detn, char* fn) : Dataset("OTB", detn, fn)
{}

// Read all the provided detections on a vector of vectors
int OTB::read_detections()
{
	/* OTB Detections provides 4 values in an CSV style file.
	* Top-Left X Coordinate
	* Top-Left Y Coordinate
	* Width Coordinate
	* Height Coordinate
	*/

	std::vector<BoundingBox> frame_detections;
	int actualFrame = 1;

	// Check if Detection file exist and is not empty.
	// TODO > PROBABLY MOVE THIS TO PARENT CLASS
	std::ifstream file(filename);
	if (!file || file.eof())
	{
		std::cout << "ERROR -> Detection file not found or empty" << std::endl;
		return 0;
	}
	else
		std::cout << "Detection file found. Loading detections..." << std::endl;

	
	// If the column number X doesn't exists, it will do nothing
	for (CSVIterator loop(file); loop != CSVIterator(); ++loop)
	{
		BoundingBox b = {
			std::stof((*loop)[0]), std::stof((*loop)[1]),
			std::stof((*loop)[2]), std::stof((*loop)[3]),
			0 }; // No score is given in OTB
		frame_detections.push_back(b);

		// OTB is single object tracking, so we will only have one object per frame
		actualFrame++;
		detections.push_back(frame_detections);
		frame_detections.clear();
	}
	// Last detections are not added in the loop, so we add them here
	detections.push_back(frame_detections);
	std::cout << "# of Dets " << detections.size() << std::endl;
	return 1;
}
