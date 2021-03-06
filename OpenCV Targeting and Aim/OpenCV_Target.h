#pragma once
class OpenCV_Target
{
public:
	OpenCV_Target(std::string, unsigned char, unsigned char = 0, unsigned char = 80, unsigned char = 80, unsigned char = 200, unsigned char = 200);
	~OpenCV_Target();
	std::string windowName;
	void detectTarget();
	void detectTargetThread();
	char port = 0;
	const std::string imageType[2] = { "haarcascade_frontalface_alt.xml", "test" };
	Mat img;
	CascadeClassifier detector;
	VideoCapture cap;
	//std::vector<Rect> found; 
	std::string cascadeName;
	bool loaded;
	int groundTheshold;
	double scaleStep;
	char minW, minH, maxW, maxH;
	Size *minObjectSize;
	Size *maxObjectSize;
	char xyTargetCenter[2];
	char xyCamCenter[2];
	bool isConnected();

	//points
	Point TL;
	Point BR;
	Point CENTER;
	/*TODO
	centers
	diff images
	thread the update of the detectFunc - done
	*/
private:
	void threadableTargetDetection();
};