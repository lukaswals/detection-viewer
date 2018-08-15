/*
* UA-DETRAC Dataset class
* More info on <http://detrac-db.rit.albany.edu/>
*
* Author: Lucas Wals
*/
#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iomanip>

#include "../Dataset.h"

//using namespace std;

class UADETRAC : public Dataset
{
public:
	UADETRAC(char*, char*);
	virtual int read_detections();	// Whether it could/couldn't read the detections
};

UADETRAC::UADETRAC(char* detn, char* fn) : Dataset("UA-DETRAC", detn, fn)
{}

// Read all the provided detections on a vector of vectors
//int UADETRAC::read_detections(std::ifstream& file, std::vector< std::vector<BoundingBox> >& detections)
int UADETRAC::read_detections()
{
	std::vector<BoundingBox> frame_detections;
	int actualFrame = 1;

	// Check if Detection file exist and is not empty
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
		while (actualFrame < det_frame) // We read all detections for previous frame
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
}

