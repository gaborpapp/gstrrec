/*
* ofxCvBlob.h
* openFrameworks
*
* A blob is a homogenous patch represented by a polygonal contour.
* Typically a blob tracker uses the contour to figure out the blob's
* persistence and "upgrades" it with ids and other temporal
* information.
*
*/


#ifndef OFX_CV_BLOB_H
#define OFX_CV_BLOB_H


#include "ofxCvConstants.h"

class ofConvDefect {
	public:
		ofConvDefect(CvConvexityDefect *def)
		{
			depth_point = ofPoint(def->depth_point->x, def->depth_point->y);
			start_point = ofPoint(def->start->x, def->start->y);
			end_point = ofPoint(def->end->x, def->end->y);
			depth = def->depth;
			ofPoint t = ((start_point + end_point) / 2) - depth_point;
			angle = atan2(t.y, t.x);
		}

		ofPoint depth_point;
		ofPoint start_point;
		ofPoint end_point;
		float depth;
		float angle;
};

class ofxCvBlob {

    public:
		~ofxCvBlob()
		{
			pts.clear();
			hull.clear();
			defects.clear();
		}

        float               area;
        float               length;
        ofRectangle         boundingRect;
        ofPoint             centroid;
        bool                hole;

        vector <ofPoint>    pts;    // the contour of the blob
        int                 nPts;   // number of pts;

		vector <ofPoint>	hull;	// points of convex hull

		vector <ofConvDefect> defects;

        //----------------------------------------
        ofxCvBlob() {
            area		= 0.0f;
            length		= 0.0f;
            hole		= false;
            nPts        = 0;
        }

        //----------------------------------------
        void draw(float x = 0, float y = 0){
            ofNoFill();
            ofSetColor(0x00FFFF);
            ofBeginShape();
            for (int i = 0; i < nPts; i++){
                   ofVertex(x + pts[i].x, y + pts[i].y);
            }
            ofEndShape(true);
            ofSetColor(0xff0099);
            ofRect(x + boundingRect.x, y + boundingRect.y, boundingRect.width, boundingRect.height);
        }
};


#endif


