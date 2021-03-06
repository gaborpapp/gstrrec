/*
* ofxCvContourFinder.h
*
* Finds white blobs in binary images and identifies
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of ofxCvBlob objects.
*
*/

#ifndef OFX_CV_CONTOUR_FINDER
#define OFX_CV_CONTOUR_FINDER



#include "ofxCvConstants.h"
#include "ofxCvBlob.h"
#include "ofxCvGrayscaleImage.h"
#include <algorithm>

class ofxCvContourFinder : public ofBaseDraws {

  public:

	enum {
		DRAW_BBOX = 1,
		DRAW_BLOB = 2,
		DRAW_HULL = 4,
		DRAW_CONVDEFECT = 8
	};

    vector<ofxCvBlob>  blobs;
    int                nBlobs;    // DEPRECATED: use blobs.size() instead


    ofxCvContourFinder();
    virtual  ~ofxCvContourFinder();

	virtual float getWidth() { return _width; };    //set after first findContours call
	virtual float getHeight() { return _height; };  //set after first findContours call

    virtual int  findContours( ofxCvGrayscaleImage& input,
                               int minArea, int maxArea,
                               int nConsidered, bool bFindHoles,
                               bool bUseApproximation = true);
                               // approximation = don't do points for all points 
                               // of the contour, if the contour runs
                               // along a straight line, for example...

	virtual void  draw() { draw(0, 0, _width, _height); };
	virtual void  draw( float x, float y) { draw(x, y, _width, _height); };
	virtual void  draw( float x, float y, float w, float h) { draw(x, y, w, h, 65535); };

	virtual void  draw( float x, float y, unsigned flags ) { draw(x, y, _width, _height, flags); };
	virtual void  draw( float x, float y, float w, float h, unsigned flags );

	virtual void setAnchorPercent(float xPct, float yPct);
	virtual void setAnchorPoint(int x, int y);
	virtual void resetAnchor();
	//virtual ofxCvBlob  getBlob(int num);



  protected:

    int  _width;
    int  _height;
    ofxCvGrayscaleImage     inputCopy;
    CvMemStorage*           contour_storage;
    CvMemStorage*           storage;
    CvMoments*              myMoments;
    vector<CvSeq*>          cvSeqBlobs;  //these will become blobs

    ofPoint  anchor;
    bool  bAnchorIsPct;

    virtual void reset();

};



#endif
