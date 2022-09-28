#pragma once
#include <string>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

struct ICallCodeReader;

class CallClassWrapperCodeReader
{
public:
	CallClassWrapperCodeReader(void);
	virtual ~CallClassWrapperCodeReader(void);

private:
	ICallCodeReader* m_pCallCodeReader;

public:
	/**
	 * @brief Dll 초기화
	 * @param bool isMIL MIL 사용시 true, cognex visionpro 사용시 false
	 * @param int sizeX 이미지 X 크기
	 * @param int sizeY 이미지 Y 크기
	 * @param CString path 코그넥스 vpp 파일 경로
	 * @return 초기화 성공시 true, 실패시 false
	 */
	bool Initialize(bool isMIL, int sizeX, int sizeY, CString path);
	/**
	 * @brief Dll 종료
	 * @return 종료 성공 시 true, 실패시 false
	 */
	bool Terminate();
	/**
	 * @brief 2D Matrix code 읽기
	 * @param cv::Mat img 바코드 이미지
	 * @param int width 바코드 이미지 X
	 * @param int height 바코드 이미지 Y
	 * @return 읽은 바코드 문자 없는 경우 string.empty
	 */
	std::string CodeRead(cv::Mat img, int width, int height, bool usePreprocess);
};

