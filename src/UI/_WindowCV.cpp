/*
 * Window.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: Kai Yan
 */

#ifdef USE_OPENCV
#include "_WindowCV.h"
#include "../Script/Kiss.h"

namespace kai
{

_WindowCV::_WindowCV()
{
	m_waitKey = 30;
	m_bFullScreen = false;
	m_gstOutput = "";
	m_fileRec = "";
	m_vSize.init(1280,720);
	m_bShow = true;
}

_WindowCV::~_WindowCV()
{
	if (m_VW.isOpened())
	{
		m_VW.release();
	}
}

bool _WindowCV::init(void* pKiss)
{
	IF_F(!this->_ModuleBase::init(pKiss));
	Kiss* pK = (Kiss*) pKiss;

	vector<string> vB;
	pK->a("vBASE", &vB);
	for (string p : vB)
	{
		BASE *pB = (BASE*)(pK->getInst(p));
		IF_CONT(!pB);

		m_vpB.push_back(pB);
	}

	pK->v("bShow", &m_bShow);
	pK->v("bFullScreen",&m_bFullScreen);
	pK->v("vSize", &m_vSize);
	m_waitKey = 1000.0f / m_pT->getTargetFPS();

	if (m_vSize.area() <= 0)
	{
		LOG_E("Window size too small");
		return false;
	}

	pK->v("fileRec",&m_fileRec);
	if (!m_fileRec.empty())
	{
		int recFPS = 30;
		string reCodec = "MJPG";
		pK->v("recFPS", &recFPS);
		pK->v("recCodec", &reCodec);

		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char strTime[128];
		strftime(strTime, sizeof(strTime), "%c", tm);
		m_fileRec += strTime;
		m_fileRec += ".avi";

		if (!m_VW.open(m_fileRec,
#if CV_VERSION_MAJOR==3
				CV_FOURCC
#else
				VideoWriter::fourcc
#endif
						(reCodec.at(0),
						reCodec.at(1),
						reCodec.at(2),
						reCodec.at(3)),
						recFPS,
						cv::Size(m_vSize.x, m_vSize.y)))
		{
			LOG_E("Cannot open file recording");
			return false;
		}
	}

	pK->v("gstOutput",&m_gstOutput);
	if (!m_gstOutput.empty())
	{
		if (!m_gst.open(m_gstOutput,
						CAP_GSTREAMER,
						0,
						30,
						cv::Size(m_vSize.x, m_vSize.y),
						true))
		{
			LOG_E("Cannot open GStreamer output");
			return false;
		}
	}

	m_frame.allocate(m_vSize.x, m_vSize.y);

	IF_T(!m_bShow);

	if (m_bFullScreen)
	{
		namedWindow(*this->getName(), WINDOW_NORMAL);
		setWindowProperty(*this->getName(), WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	}
	else
	{
		namedWindow(*this->getName(), WINDOW_AUTOSIZE);
	}

	return true;
}

bool _WindowCV::start(void)
{
    NULL_F(m_pT);
	return m_pT->start(getUpdate, this);
}

void _WindowCV::update(void)
{
	while (m_pT->bRun())
	{
		m_pT->autoFPSfrom();
        
		updateWindow();

		m_pT->autoFPSto();
	}
}

void _WindowCV::updateWindow(void)
{
	for (BASE* pB : m_vpB)
	{
		pB->cvDraw(this);
	}
	
	IF_(m_frame.bEmpty());

	Mat m = *m_frame.m();

	if (m_bShow)
	{
		imshow(*this->getName(), m);
	}

	if(m_VW.isOpened() || m_gst.isOpened())
	{
		m_F.copy(m_frame);
		Size fs = m_F.size();

		if (fs.width != m_vSize.x || fs.height != m_vSize.y)
		{
			m_F2 = m_F.resize(m_vSize.x, m_vSize.y);
			m_F = m_F2;
		}

		if (m_F.m()->type() != CV_8UC3)
		{
			m_F2 = m_F.cvtColor(COLOR_GRAY2BGR);
			m_F = m_F2;
		}

		if (m_VW.isOpened())
			m_VW << *m_F.m();

		if (m_gst.isOpened())
			m_gst << *m_F.m();
	}

	int key = waitKey(m_waitKey);
	*m_frame.m() = Scalar(0,0,0);
}

Frame* _WindowCV::getFrame(void)
{
	return &m_frame;
}

}
#endif
