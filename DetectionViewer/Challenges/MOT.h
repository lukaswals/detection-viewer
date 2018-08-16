/*
* MOT(Multiple-Object Tracking) Dataset class
* More info on <https://motchallenge.net/>
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

class MOT : public Dataset
{
public:
	MOT(char*, char*);
	virtual int read_detections();	// Whether it could/couldn't read the detections+
};

MOT::MOT(char* detn, char* fn) : Dataset("MOT", detn, fn)
{}

// Read all the provided detections on a vector of vectors
int MOT::read_detections()
{
	/* MOT Detections provides 9 values in an CSV style file. 
	* This values represent (in order):
	* Frame #
	* Identity Number (-1 for Detections, cause not Identity has been assigned yet...)
	* Top-Left X Coordinate
	* Top-Left Y Coordinate
	* Width Coordinate
	* Height Coordinate
	* Confidense Score
	* Ignored
	* Ignored
	* Ignored
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
		int det_frame = std::stoi((*loop)[0]);
		//std::cout << "Detected in Frame N " << det_frame << "\n";
		while (actualFrame < det_frame) // Push detections from the previous frame
		{
			actualFrame++;
			detections.push_back(frame_detections);
			frame_detections.clear();
		}
		BoundingBox b = {
			std::stof((*loop)[2]), std::stof((*loop)[3]),
			std::stof((*loop)[4]), std::stof((*loop)[5]),
			std::stof((*loop)[6]) };
		frame_detections.push_back(b);
	}
	// Last detections are not added in the loop, so we add them here
	detections.push_back(frame_detections);
	std::cout << "# of Dets " << detections.size() << std::endl;
	return 1;
}