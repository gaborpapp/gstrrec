
#include "ofxCvContourFinder.h"



//--------------------------------------------------------------------------------
bool sort_carea_compare( const CvSeq* a, const CvSeq* b) {
	// use opencv to calc size, then sort based on size
	float areaa = fabs(cvContourArea(a, CV_WHOLE_SEQ));
	float areab = fabs(cvContourArea(b, CV_WHOLE_SEQ));

    //return 0;
	return (areaa > areab);
}




//--------------------------------------------------------------------------------
ofxCvContourFinder::ofxCvContourFinder() {
    _width = 0;
    _height = 0;
	myMoments = (CvMoments*)malloc( sizeof(CvMoments) );
	reset();
}

//--------------------------------------------------------------------------------
ofxCvContourFinder::~ofxCvContourFinder() {
	free( myMoments );
}

//--------------------------------------------------------------------------------
void ofxCvContourFinder::reset() {
    cvSeqBlobs.clear();
    blobs.clear();
    nBlobs = 0;
}

//--------------------------------------------------------------------------------
int ofxCvContourFinder::findContours( ofxCvGrayscaleImage&  input,
									  int minArea,
									  int maxArea,
									  int nConsidered,
									  bool bFindHoles,
                                      bool bUseApproximation) {

    // get width/height disregarding ROI
    IplImage* ipltemp = input.getCvImage();
    _width = ipltemp->width;
    _height = ipltemp->height;

	reset();

	// opencv will clober the image it detects contours on, so we want to
    // copy it into a copy before we detect contours.  That copy is allocated
    // if necessary (necessary = (a) not allocated or (b) wrong size)
	// so be careful if you pass in different sized images to "findContours"
	// there is a performance penalty, but we think there is not a memory leak
    // to worry about better to create mutiple contour finders for different
    // sizes, ie, if you are finding contours in a 640x480 image but also a
    // 320x240 image better to make two ofxCvContourFinder objects then to use
    // one, because you will get penalized less.

	if( inputCopy.getWidth() == 0 ) {
		inputCopy.allocate( _width, _height );
	} else if( inputCopy.getWidth() != _width || inputCopy.getHeight() != _height ) {
        // reallocate to new size
        inputCopy.clear();
        inputCopy.allocate( _width, _height );
	}

    inputCopy = input;
    inputCopy.setROI( input.getROI() );

	CvSeq* contour_list = NULL;
	contour_storage = cvCreateMemStorage( 1000 );
	storage	= cvCreateMemStorage( 1000 );
	CvMemStorage *defects_storage = cvCreateMemStorage(64);

	CvContourRetrievalMode  retrieve_mode
        = (bFindHoles) ? CV_RETR_LIST : CV_RETR_EXTERNAL;
	cvFindContours( inputCopy.getCvImage(), contour_storage, &contour_list,
                    sizeof(CvContour), retrieve_mode, bUseApproximation ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE );
	CvSeq* contour_ptr = contour_list;

	// put the contours from the linked list, into an array for sorting
	while( (contour_ptr != NULL) ) {
		float area = fabs( cvContourArea(contour_ptr, CV_WHOLE_SEQ) );
		if( (area > minArea) && (area < maxArea) ) {
            cvSeqBlobs.push_back(contour_ptr);
		}
		contour_ptr = contour_ptr->h_next;
	}

	// sort the pointers based on size
	if( cvSeqBlobs.size() > 1 ) {
        sort( cvSeqBlobs.begin(), cvSeqBlobs.end(), sort_carea_compare );
	}

	// now, we have cvSeqBlobs.size() contours, sorted by size in the array
    // cvSeqBlobs let's get the data out and into our structures that we like
	for( int i = 0; i < MIN(nConsidered, (int)cvSeqBlobs.size()); i++ ) {
		blobs.push_back( ofxCvBlob() );
		float area = cvContourArea( cvSeqBlobs[i], CV_WHOLE_SEQ );
		CvRect rect	= cvBoundingRect( cvSeqBlobs[i], 0 );
		cvMoments( cvSeqBlobs[i], myMoments );

		blobs[i].area                     = fabs(area);
		blobs[i].hole                     = area < 0 ? true : false;
		blobs[i].length				      = cvArcLength(cvSeqBlobs[i]);
		blobs[i].boundingRect.x           = rect.x;
		blobs[i].boundingRect.y           = rect.y;
		blobs[i].boundingRect.width       = rect.width;
		blobs[i].boundingRect.height      = rect.height;
		blobs[i].centroid.x				  = (myMoments->m10 / myMoments->m00);
		blobs[i].centroid.y				  = (myMoments->m01 / myMoments->m00);

		// get the points for the blob:
		CvPoint           pt;
		CvSeqReader       reader;
		cvStartReadSeq( cvSeqBlobs[i], &reader, 0 );

		for( int j=0; j < cvSeqBlobs[i]->total; j++ ) {
			CV_READ_SEQ_ELEM( pt, reader );
            blobs[i].pts.push_back( ofPoint((float)pt.x, (float)pt.y) );
		}
		blobs[i].nPts = blobs[i].pts.size();

		// convex hull
		CvSeq *seq_hull = NULL;
		seq_hull = cvConvexHull2(cvSeqBlobs[i], 0, CV_COUNTER_CLOCKWISE, 0);

		for (int j = 0; j < seq_hull->total; j++)
		{
			CvPoint *pt = *CV_GET_SEQ_ELEM(CvPoint*, seq_hull, j);
            blobs[i].hull.push_back(ofPoint((float)pt->x, (float)pt->y));
		}

		// find defects of contour
		CvSeq *defects = cvConvexityDefects(cvSeqBlobs[i], seq_hull, defects_storage);
		float depth_thr = rect.height * .1;
		for (int j = 0; j < defects->total; j++)
		{
			CvConvexityDefect *def = CV_GET_SEQ_ELEM(CvConvexityDefect, defects, j);
			if (def->depth > depth_thr)
				blobs[i].defects.push_back(ofConvDefect(def));
		}

		cvClearSeq(defects);
		cvClearMemStorage(defects_storage);

		cvClearSeq(seq_hull);
	}

    nBlobs = blobs.size();

	// Free the storage memory.
	// Warning: do this inside this function otherwise a strange memory leak
	if( contour_storage != NULL ) { cvReleaseMemStorage(&contour_storage); }
	if( storage != NULL ) { cvReleaseMemStorage(&storage); }
	if( defects_storage != NULL ) { cvReleaseMemStorage(&defects_storage); }

	return nBlobs;
}

//--------------------------------------------------------------------------------
void ofxCvContourFinder::draw( float x, float y, float w, float h, unsigned flags ) {

    float scalex = 0.0f;
    float scaley = 0.0f;
    if( _width != 0 ) { scalex = w/_width; } else { scalex = 1.0f; }
    if( _height != 0 ) { scaley = h/_height; } else { scaley = 1.0f; }

    if(bAnchorIsPct){
        x -= anchor.x * w;
        y -= anchor.y * h;
    }else{
        x -= anchor.x;
        y -= anchor.y;
    }

	// ---------------------------- draw the bounding rectangle
	ofSetColor(0xDD00CC);
    glPushMatrix();
    glTranslatef( x, y, 0.0 );
    glScalef( scalex, scaley, 0.0 );

	ofNoFill();
	if (flags & DRAW_BBOX)
		for( int i=0; i<(int)blobs.size(); i++ ) {
			ofRect( blobs[i].boundingRect.x, blobs[i].boundingRect.y,
					blobs[i].boundingRect.width, blobs[i].boundingRect.height );
		}

	// ---------------------------- draw the blobs
	ofSetColor(0x00FFFF);

	if (flags & DRAW_BLOB)
		for( int i=0; i<(int)blobs.size(); i++ ) {
			ofNoFill();
			ofBeginShape();
			for( int j=0; j<blobs[i].nPts; j++ ) {
				ofVertex( blobs[i].pts[j].x, blobs[i].pts[j].y );
			}
			ofEndShape();
		}

	// ---------------------------- draw the convex hull
	ofSetColor(0x00FF00);

	if (flags & DRAW_HULL)
		for (unsigned i = 0; i < blobs.size(); i++)
		{
			ofNoFill();
			ofBeginShape();
			for (unsigned j = 0; j < blobs[i].hull.size(); j++)
			{
				ofVertex(blobs[i].hull[j].x, blobs[i].hull[j].y);
			}
			ofEndShape();
		}

	// ---------------------------- draw convexity defects

	ofSetColor(0xFF1111);
	if (flags & DRAW_CONVDEFECT)
		for (unsigned i = 0; i < blobs.size(); i++)
		{
			ofFill();
			//cerr << blobs[i].defects.size() << endl;
			for (unsigned j = 0; j < blobs[i].defects.size(); j++)
			{
				ofCircle(blobs[i].defects[j].depth_point.x, blobs[i].defects[j].depth_point.y, 6);
				/*
				ofLine(blobs[i].defects[j].depth_point.x, blobs[i].defects[j].depth_point.y,
						blobs[i].defects[j].start_point.x, blobs[i].defects[j].start_point.y);
				ofLine(blobs[i].defects[j].depth_point.x, blobs[i].defects[j].depth_point.y,
						blobs[i].defects[j].end_point.x, blobs[i].defects[j].end_point.y);
						*/

				ofPoint d(blobs[i].defects[j].depth_point.x, blobs[i].defects[j].depth_point.y);
				ofLine(d.x , d.y, d.x + blobs[i].defects[j].depth * cos(blobs[i].defects[j].angle),
					d.y + blobs[i].defects[j].depth * sin(blobs[i].defects[j].angle));
			}
		}

	glPopMatrix();
}

//--------------------------------------------------------------------------------
void ofxCvContourFinder::setAnchorPercent( float xPct, float yPct ){
    anchor.x = xPct;
    anchor.y = yPct;
    bAnchorIsPct = true;
}

//--------------------------------------------------------------------------------
void ofxCvContourFinder::setAnchorPoint( int x, int y ){
    anchor.x = x;
    anchor.y = y;
    bAnchorIsPct = false;
}

//--------------------------------------------------------------------------------
void ofxCvContourFinder::resetAnchor(){
    anchor.set(0,0);
    bAnchorIsPct = false;
}


