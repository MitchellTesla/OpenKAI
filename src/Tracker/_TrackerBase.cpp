/*
 * _TrackerBase.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: yankai
 */

#include "_TrackerBase.h"

#ifdef USE_OPENCV

namespace kai
{

_TrackerBase::_TrackerBase()
{
	m_pV = NULL;
	m_trackerType = "";
	m_trackState = track_stop;
	m_bb.init();
	m_iSet = 0;
	m_iInit = 0;
	m_margin = 0.0;

}

_TrackerBase::~_TrackerBase()
{
}

bool _TrackerBase::init(void* pKiss)
{
	IF_F(!this->_ModuleBase::init(pKiss));
	Kiss* pK = (Kiss*) pKiss;

	pK->v("trackerType",&m_trackerType);
	pK->v("margin",&m_margin);

	string n = "";
	F_ERROR_F(pK->v("_VisionBase", &n));
	m_pV = (_VisionBase*) (pK->getInst(n));

	return true;
}

void _TrackerBase::createTracker(void)
{
}

int _TrackerBase::check(void)
{
	NULL__(m_pV,-1);

	return this->_ModuleBase::check();
}

void _TrackerBase::update(void)
{
}

void _TrackerBase::stopTrack(void)
{
	m_trackState = track_stop;
}

TRACK_STATE _TrackerBase::trackState(void)
{
	return m_trackState;
}

vFloat4* _TrackerBase::getBB(void)
{
	return &m_bb;
}

bool _TrackerBase::startTrack(vFloat4& bb)
{
	NULL_F(m_pV);
	Mat* pM = m_pV->BGR()->m();
	IF_F(pM->empty());

	float mBig = 1.0 + m_margin;
	float mSmall = 1.0 - m_margin;

	bb.x = constrain(bb.x * mSmall, 0.0f, 1.0f);
	bb.y = constrain(bb.y * mSmall, 0.0f, 1.0f);
	bb.z = constrain(bb.z * mBig, 0.0f, 1.0f);
	bb.w = constrain(bb.w * mBig, 0.0f, 1.0f);

	Rect rBB = bb2Rect(bbScale(bb, pM->cols, pM->rows));
	IF_F(rBB.width == 0 || rBB.height == 0);

	m_newBB = rBB;

	m_iSet++;
	m_trackState = track_init;
	return true;
}

void _TrackerBase::draw(void)
{
	this->_ModuleBase::draw();

	string msg = "Stop";
	if(m_trackState == track_init)
		msg = "Init";
	else if(m_trackState == track_update)
		msg = "Update";

	addMsg(msg,1);
	addMsg("Tracking pos = ("
			+ f2str(m_bb.midX()) + ", "
			+ f2str(m_bb.midY()) + ")");

	IF_(!checkWindow());
	_WindowCV* pWin = (_WindowCV*) this->m_pWindow;
	Frame* pFrame = pWin->getFrame();
	Mat* pMat = pFrame->m();

	Scalar col;
	if(m_trackState == track_init)
	{
		col = Scalar(0,255,255);
		rectangle(*pMat, m_newBB, col, 2);
	}
	else if(m_trackState == track_update)
	{
		col = Scalar(0,255,0);
		rectangle(*pMat, m_rBB, col, 2);
	}
}

}
#endif
