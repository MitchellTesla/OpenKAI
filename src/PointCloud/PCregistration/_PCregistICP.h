/*
 * _PCregistICP.h
 *
 *  Created on: Sept 6, 2020
 *      Author: yankai
 */

#ifndef OpenKAI_src_PointCloud_PCregistICP_H_
#define OpenKAI_src_PointCloud_PCregistICP_H_

#include "../../Base/common.h"

#ifdef USE_OPEN3D
#include "../_PCtransform.h"

using namespace open3d::pipelines::registration;

namespace kai
{
    
enum PCREGIST_ICP_EST
{
    icp_p2point = 0,
    icp_p2plane = 1,
};

class _PCregistICP: public _PCbase
{
public:
	_PCregistICP();
	virtual ~_PCregistICP();

	bool init(void* pKiss);
	bool start(void);
	int check(void);
	void draw(void);

private:
	void updateRegistration(void);
	void update(void);
	static void* getUpdate(void* This)
	{
		(( _PCregistICP *) This)->update();
		return NULL;
	}

public:
	float m_thr;	//ICP threshold
	PCREGIST_ICP_EST m_est;
	_PCbase* m_pSrc;
	_PCbase* m_pTgt;
	_PCtransform* m_pTf;
	int m_iMt;
    RegistrationResult m_RR;
    double m_lastFit;
};

}
#endif
#endif