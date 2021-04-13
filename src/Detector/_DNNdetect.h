/*
 * _DNNdetect.h
 *
 *  Created on: Jan 11, 2019
 *      Author: yankai
 */

#ifndef OpenKAI_src_DNN_Darknet__DNNdetect_H_
#define OpenKAI_src_DNN_Darknet__DNNdetect_H_

#include "_DetectorBase.h"

#ifdef USE_OPENCV
using namespace cv::dnn;

namespace kai
{

enum DNN_type
{
	dnn_caffe,
	dnn_yolo,
	dnn_tf,
};

class _DNNdetect: public _DetectorBase
{
public:
	_DNNdetect();
	~_DNNdetect();

	bool init(void* pKiss);
	bool start(void);
	void draw(void);
	int check(void);

private:
	void detectYolo(void);
	void detect(void);
	void update(void);
	static void* getUpdate(void* This)
	{
		((_DNNdetect*) This)->update();
		return NULL;
	}

public:
	DNN_type m_dnnType;
	cv::dnn::Net m_net;
	double	m_thr;
	double	m_nms;
	int		m_nW;
	int		m_nH;
	bool	m_bSwapRB;
	float	m_scale;
	vInt3	m_vMean;
	Mat		m_blob;
	vector<string> m_vLayerName;
	int		m_iClassDraw;

	int m_iBackend;
	int m_iTarget;

};

}
#endif
#endif
