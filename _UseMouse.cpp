#include <opencv2/opencv.hpp>

using namespace cv;

//-----------------------------------【宏定义部分】--------------------------------------------
//  描述：定义一些辅助宏 
//------------------------------------------------------------------------------------------------ 
#define WINDOW_NAME "【程序窗口】"        //为窗口标题定义的宏 

//-----------------------------------【全局函数声明部分】------------------------------------
//		描述：全局函数的声明
//------------------------------------------------------------------------------------------------
void on_MouseHandle(int event, int x, int y, int flags, void* param);//指定窗口中每次发生鼠标事件的时候，被调用
void DrawRectangle(cv::Mat& img, cv::Rect box);//在图像上画正方形


//-----------------------------------【全局变量声明部分】-----------------------------------
//		描述：全局变量的声明
//-----------------------------------------------------------------------------------------------
Rect g_rectangle;
bool g_bDrawingBox = false;//是否进行绘制
RNG g_rng(12345);

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//【0】改变console字体颜色
	system("color 9F");

	//【1】准备参数
	g_rectangle = Rect(-1, -1, 0, 0);
	Mat srcImage(600, 800, CV_8UC3), tempImage;
	srcImage.copyTo(tempImage);//将原始图拷贝入临时图中
	g_rectangle = Rect(-1, -1, 0, 0);//？？为什么有两个相同的rect
	srcImage = Scalar::all(0);
	//【2】设置鼠标操作回调函数
	namedWindow(WINDOW_NAME);
	setMouseCallback(WINDOW_NAME, on_MouseHandle, (void*)& srcImage);//？？？

	//【3】程序主循环，当进行绘制的标识符为真时，进行绘制
	while (true) {
		srcImage.copyTo(tempImage);//拷贝源图到临时变量
		if (g_bDrawingBox)DrawRectangle(tempImage, g_rectangle);//当进行绘制的标识符为真，则进行绘制
		imshow(WINDOW_NAME, tempImage);
		if (cv::waitKey(10) == 27) break;//按下ESC键，程序退出
	}
	return 0;
}

//--------------------------------【on_MouseHandle( )函数】-----------------------------
//		描述：鼠标回调函数，根据不同的鼠标事件进行不同的操作
//-----------------------------------------------------------------------------------------------
void on_MouseHandle(int event, int x, int y, int flags, void* param) {
	Mat& image = *(cv::Mat*) param;
	switch (event) {
		//鼠标移动消息
		case EVENT_MOUSEMOVE: {
			if (g_bDrawingBox) {
				g_rectangle.width = x - g_rectangle.x;
				g_rectangle.height = y - g_rectangle.y;
			}
		}
			break;
		case EVENT_LBUTTONDOWN: {
			g_bDrawingBox = true;
			g_rectangle = Rect(x, y, 0, 0);
		}
			break;
		case EVENT_LBUTTONUP: {
			g_bDrawingBox = false;
			if (g_rectangle.width < 0) {
				g_rectangle.x += g_rectangle.width;
				g_rectangle.width *= -1;
			}
			if (g_rectangle.height < 0) {
				g_rectangle.y += g_rectangle.height;
				g_rectangle.height *= -1;
			}
			DrawRectangle(image, g_rectangle);
		}
			break;

	}
}
//-----------------------------------【DrawRectangle( )函数】------------------------------
//		描述：自定义的矩形绘制函数
//-----------------------------------------------------------------------------------------------
void DrawRectangle(cv::Mat& img, cv::Rect box) {
	rectangle(img, box.tl(), box.br(), Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));//随机颜色
}
