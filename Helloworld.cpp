#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>


using namespace std;
using namespace cv;



void helloworld() {
	//读入图片
	Mat img = imread("../data/11.jpg");
	//显示图片
	imshow("img", img);
	cv::waitKey(0);
}

//----------------------------腐蚀操作------------------------------------
void erode() {
	

	Mat srcImg = imread("../data/12.jpg");
	imshow("[原图]腐蚀操作", srcImg);
	//进行腐蚀操作
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat dstImage;
	erode(srcImg, dstImage, element);
	
	imshow("[效果图]腐蚀操作", dstImage);
	cv::waitKey(0);

}

//----------------------------模糊操作------------------------------------
void blur() {
	
	cv::Mat srcImg = imread("../data/12.jpg");
	cv::imshow("[原图]模糊操作", srcImg);
	//进行模糊操作
	cv::Mat dstImage;
	cv::blur(srcImg, dstImage, Size(5, 5));

	cv::imshow("[效果图]模糊操作", dstImage);
	
	cv::waitKey(0);
}
//------------------------边缘检测----------------------------------------
void edge_Detection_Canny() {
	
	cv::Mat srcImg = imread("../data/12.jpg");
	cv::imshow("[原图]canny边缘检测", srcImg);
	Mat edge, grayImg;

	//[1]将原始图片转换为灰度图像
	cvtColor(srcImg, grayImg ,COLOR_BGR2GRAY);// CV_BGR2GRAY未定义标识符需要添加 #include <opencv2\imgproc\types_c.h> 或者修改为COLOR_BGR2GRAY
	//[2]先使用3*3内核来降噪
	blur(grayImg, edge, Size(3, 3));
	//[3]运行canny算子
	Canny(edge, edge, 3, 9, 3);
	//[4]显示效果图
	cv::imshow("[效果图]anny边缘检测", edge);

	cv::waitKey(0);
}

//--------------------------视频读取--------------------------------------
void vedioCapture() {
	VideoCapture capture("../data/car.avi");
	Mat frame;
	while (true) {
		capture >> frame;//读取当前帧
		if (frame.empty()) {
			break;
		}
		imshow("读取视频", frame);
		cv::waitKey(30);
	}
}

//--------------------------读取摄像头并且进行边缘检测--------------------------------------
void vedioCaptureAndEdgeDetection() {
	
	VideoCapture capture(0);//读取摄像头
	Mat frame;
	Mat edge, grayImg;
	while (true) {
		capture >> frame;//读取当前帧
		if (frame.empty()) {
			break;
		}
		//[1]将原始图片转换为灰度图像
		cvtColor(frame, grayImg, COLOR_BGR2GRAY);//未定义标识符需要添加 #include <opencv2\imgproc\types_c.h>
		//[2]先使用3*3内核来降噪
		blur(grayImg, edge, Size(3, 3));
		//[3]运行canny算子
		Canny(edge, edge, 3, 9, 3);
		//[4]显示效果图
		cv::imshow("[效果图]canny边缘检测", edge);
		imshow("读取视频", frame);


		cv::waitKey(30);
	}
}

//--------------------------创建一个带alpha通道的Mat--------------------------------------
void createAlphaMat(Mat& mat) {
	for (int i = 0; i < mat.rows; i++){
		for (int j = 0; j < mat.cols; j++) {
			Vec4b&rgba = mat.at<Vec4b>(i,j);
			rgba[0] = UCHAR_MAX; //
			rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);
			rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
		}
	}
}

//--------------------------将传入的图片写入一个文件中--------------------------
int writeImgToFile() {
	Mat mat(480, 640, CV_8UC4);//创建一个大小为480*640通道为4的图像
	createAlphaMat(mat);

	vector<int>compression_params;//生成图像的参数
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("透明Alpha值图.png", mat, compression_params);
		imshow("生成的png图", mat);
		fprintf(stdout, "Png图片文件的alpha数据保存完毕~\n可以在工程目录下查看由imwrite生成的图片\n");
		cv::waitKey(0);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "图像转换成PNG格式发生错误：%s\n",ex.what());
		return 1;
	}

	return 0;
}
int main() {
	//std::cout << "helloworld" << endl;
	writeImgToFile();
	return 0;
}