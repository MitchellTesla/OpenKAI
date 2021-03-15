/*
 * PCbase.h
 *
 *  Created on: May 24, 2020
 *      Author: yankai
 */

#ifndef OpenKAI_src_PointCloud__PCframe_H_
#define OpenKAI_src_PointCloud__PCframe_H_

#include "../Base/_ModuleBase.h"
#include "_PCviewer.h"

#ifdef USE_OPEN3D
using namespace open3d;
using namespace open3d::geometry;
using namespace open3d::visualization;

namespace kai
{

class _PCframe: public _ModuleBase
{
public:
	_PCframe();
	virtual ~_PCframe();

	virtual bool init(void* pKiss);
	virtual int size(void);
	virtual int check(void);
	virtual void draw(void);

    //frame
	virtual void getPC(PointCloud* pPC);
	virtual void updatePC(void);
    
    //ring buf
    virtual void addP(Eigen::Vector3d& vP, Eigen::Vector3d& vC, uint64_t& tStamp);
    
    virtual void setTranslation(vDouble3& vT);
	virtual void setRotation(vDouble3& vR);
	virtual vDouble3 getTranslation(void);
	virtual vDouble3 getRotation(void);
    
protected:
    virtual void updateTransformMatrix(void);
    virtual void paintPC(PointCloud* pPC);
    
    //frame buf
	_PCframe* m_pPCB;
	vSwitch<PointCloud> m_sPC;
    pthread_mutex_t m_mutexPC;
    
    //ring buf
    PC_RING_BUF m_ringB;
    
    //dynamics
    bool m_bTransform;
   	vDouble3 m_vT;	//translation
	vDouble3 m_vR;	//rotation
	Eigen::Affine3d m_A;

    //visualization
	_PCviewer* m_pViewer;
	int m_iV;
    vFloat3 m_vColOvrr;
};

}
#endif
#endif