/*
 * _TrackerBase.h
 *
 *  Created on: Aug 28, 2018
 *      Author: yankai
 */

#ifndef OpenKAI_src_Tracker__TrackerBase_H_
#define OpenKAI_src_Tracker__TrackerBase_H_

#include "../Base/_ModuleBase.h"

#ifdef USE_OPENCV
#include "../Detector/_DetectorBase.h"
#include "../Vision/_VisionBase.h"

namespace kai
{

enum TRACK_STATE
{
	track_init,
	track_update,
	track_stop
};

class _TrackerBase: public _ModuleBase
{
public:
	_TrackerBase();
	virtual
	~_TrackerBase();

	bool init(void* pKiss);
	void update(void);
	int check(void);
	virtual void draw(void);

	virtual void createTracker(void);
	virtual bool startTrack(vFloat4& bb);
	virtual void stopTrack(void);
	TRACK_STATE trackState(void);
	vFloat4* getBB(void);

public:
	_VisionBase* m_pV;
	Rect2d m_rBB;
	vFloat4 m_bb;
	float m_margin;

	Rect2d	m_newBB;
	uint64_t	m_iSet;
	uint64_t	m_iInit;

	string m_trackerType;
	TRACK_STATE	m_trackState;

};

}
#endif
#endif
