/*
 * _Line.h
 *
 *  Created on: April 23, 2019
 *      Author: yankai
 */

#ifndef OpenKAI_src_Detector__Line_H_
#define OpenKAI_src_Detector__Line_H_

#include "_DetectorBase.h"

#ifdef USE_OPENCV

namespace kai
{

class _Line : public _DetectorBase
{
public:
	_Line();
	virtual ~_Line();

	bool init(void* pKiss);
	bool start(void);
	int check(void);
	void draw(void);

private:
	void detect(void);
	void update(void);
	static void* getUpdate(void* This)
	{
		((_Line*) This)->update();
		return NULL;
	}

public:
	_VisionBase* m_pV;
	float	m_wSlide;
	float	m_minPixLine;
	float	m_line;
	Mat		m_mIn;

};

}
#endif
#endif
