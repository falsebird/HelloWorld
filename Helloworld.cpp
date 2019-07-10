#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>


using namespace std;
using namespace cv;



void helloworld() {
	//����ͼƬ
	Mat img = imread("../data/11.jpg");
	//��ʾͼƬ
	imshow("img", img);
	cv::waitKey(0);
}

//----------------------------��ʴ����------------------------------------
void erode() {
	

	Mat srcImg = imread("../data/12.jpg");
	imshow("[ԭͼ]��ʴ����", srcImg);
	//���и�ʴ����
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat dstImage;
	erode(srcImg, dstImage, element);
	
	imshow("[Ч��ͼ]��ʴ����", dstImage);
	cv::waitKey(0);

}

//----------------------------ģ������------------------------------------
void blur() {
	
	cv::Mat srcImg = imread("../data/12.jpg");
	cv::imshow("[ԭͼ]ģ������", srcImg);
	//����ģ������
	cv::Mat dstImage;
	cv::blur(srcImg, dstImage, Size(5, 5));

	cv::imshow("[Ч��ͼ]ģ������", dstImage);
	
	cv::waitKey(0);
}
//------------------------��Ե���----------------------------------------
void edge_Detection_Canny() {
	
	cv::Mat srcImg = imread("../data/12.jpg");
	cv::imshow("[ԭͼ]canny��Ե���", srcImg);
	Mat edge, grayImg;

	//[1]��ԭʼͼƬת��Ϊ�Ҷ�ͼ��
	cvtColor(srcImg, grayImg ,COLOR_BGR2GRAY);// CV_BGR2GRAYδ�����ʶ����Ҫ��� #include <opencv2\imgproc\types_c.h> �����޸�ΪCOLOR_BGR2GRAY
	//[2]��ʹ��3*3�ں�������
	blur(grayImg, edge, Size(3, 3));
	//[3]����canny����
	Canny(edge, edge, 3, 9, 3);
	//[4]��ʾЧ��ͼ
	cv::imshow("[Ч��ͼ]anny��Ե���", edge);

	cv::waitKey(0);
}

//--------------------------��Ƶ��ȡ--------------------------------------
void vedioCapture() {
	VideoCapture capture("../data/car.avi");
	Mat frame;
	while (true) {
		capture >> frame;//��ȡ��ǰ֡
		if (frame.empty()) {
			break;
		}
		imshow("��ȡ��Ƶ", frame);
		cv::waitKey(30);
	}
}

//--------------------------��ȡ����ͷ���ҽ��б�Ե���--------------------------------------
void vedioCaptureAndEdgeDetection() {
	
	VideoCapture capture(0);//��ȡ����ͷ
	Mat frame;
	Mat edge, grayImg;
	while (true) {
		capture >> frame;//��ȡ��ǰ֡
		if (frame.empty()) {
			break;
		}
		//[1]��ԭʼͼƬת��Ϊ�Ҷ�ͼ��
		cvtColor(frame, grayImg, COLOR_BGR2GRAY);//δ�����ʶ����Ҫ��� #include <opencv2\imgproc\types_c.h>
		//[2]��ʹ��3*3�ں�������
		blur(grayImg, edge, Size(3, 3));
		//[3]����canny����
		Canny(edge, edge, 3, 9, 3);
		//[4]��ʾЧ��ͼ
		cv::imshow("[Ч��ͼ]canny��Ե���", edge);
		imshow("��ȡ��Ƶ", frame);


		cv::waitKey(30);
	}
}

//--------------------------����һ����alphaͨ����Mat--------------------------------------
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

//--------------------------�������ͼƬд��һ���ļ���--------------------------
int writeImgToFile() {
	Mat mat(480, 640, CV_8UC4);//����һ����СΪ480*640ͨ��Ϊ4��ͼ��
	createAlphaMat(mat);

	vector<int>compression_params;//����ͼ��Ĳ���
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("͸��Alphaֵͼ.png", mat, compression_params);
		imshow("���ɵ�pngͼ", mat);
		fprintf(stdout, "PngͼƬ�ļ���alpha���ݱ������~\n�����ڹ���Ŀ¼�²鿴��imwrite���ɵ�ͼƬ\n");
		cv::waitKey(0);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "ͼ��ת����PNG��ʽ��������%s\n",ex.what());
		return 1;
	}

	return 0;
}
int main() {
	//std::cout << "helloworld" << endl;
	writeImgToFile();
	return 0;
}