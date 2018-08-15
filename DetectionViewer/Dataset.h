/******************************************************************************
* Dataset and other auxiliary classes definitions are here
*
* Author: Lucas Wals
******************************************************************************/
#pragma once
#include <vector>

#include "CSV.h"

/******************************************************************************
* BoundingBox 
*	Simple struct used for easy drawing. All detections are converted into
*	this format if they don't already come like this.
******************************************************************************/
struct BoundingBox
{
	// x-component of top left coordinate
	float x;
	// y-component of top left coordinate
	float y;
	// width of the box
	float w;
	// height of the box
	float h;
	// score of the box;
	float score;
};


/******************************************************************************
* Dataset
*	Class from which every dataset will inherit.
******************************************************************************/
class Dataset {
protected:
	char* dataset_name;
	char* detector_name;
	char* filename;
	// Detections are stored as a Vector of Vectors, where each vector
	// represents the detections of the correspondent index+1 frame
	std::vector< std::vector<BoundingBox> > detections;
public:
	Dataset(char*, char*, char*);
	virtual int read_detections() = 0;	// Whether it could/couldn't read the detections
	std::vector< BoundingBox > get_frame_detections(int f) { return detections[f]; }
};

Dataset::Dataset(char* datan, char* detn, char* fn)
{
	filename = fn;
	dataset_name = datan;
	detector_name = detn;
}