/*
 * _PCframe.cpp
 *
 *  Created on: May 24, 2020
 *      Author: yankai
 */

#include "_PCframe.h"

#ifdef USE_OPEN3D

namespace kai
{

_PCframe::_PCframe()
{
    m_pPCB = NULL;
    m_pViewer = NULL;
    m_iV = -1;
    m_vColOvrr.x = -1.0;
    
    m_bTransform = false;
    m_vT.init(0);
	m_vR.init(0);
	m_A = Eigen::Matrix4d::Identity();

    pthread_mutex_init ( &m_mutexPC, NULL );
}

_PCframe::~_PCframe()
{
    pthread_mutex_destroy ( &m_mutexPC );
}

bool _PCframe::init ( void *pKiss )
{
    IF_F ( !this->_ModuleBase::init ( pKiss ) );
    Kiss *pK = ( Kiss* ) pKiss;

    pK->v ( "vColOvrr", &m_vColOvrr );

    //frame
    int nPCreserve = 0;
    pK->v ( "nPCreserve", &nPCreserve );
    if ( nPCreserve > 0 )
    {
        m_sPC.prev()->points_.reserve ( nPCreserve );
        m_sPC.prev()->colors_.reserve ( nPCreserve );
        m_sPC.next()->points_.reserve ( nPCreserve );
        m_sPC.next()->colors_.reserve ( nPCreserve );
    }
    
    //ring buf
    int nP = 0;
    pK->v ( "nP", &nP );
    IF_F(!m_ringB.init(nP));

    //transform
    pK->v("bTransform", &m_bTransform);
    pK->v("vT", &m_vT);
	pK->v("vR", &m_vR);

    string n;

    n = "";
    pK->v ( "_PCframe", &n );
    m_pPCB = ( _PCframe* ) ( pK->getInst ( n ) );

    n = "";
    pK->v ( "_PCviewer", &n );
    m_pViewer = ( _PCviewer* ) ( pK->getInst ( n ) );

    if ( m_pViewer )
    {
        m_iV = m_pViewer->addGeometry();
    }

    return true;
}

int _PCframe::check ( void )
{
    
    return this->_ModuleBase::check();
}

void _PCframe::getPC ( PointCloud* pPC )
{
    NULL_ ( pPC );

    pthread_mutex_lock ( &m_mutexPC );
    *pPC = *m_sPC.prev();
    pthread_mutex_unlock ( &m_mutexPC );
}

void _PCframe::updatePC ( void )
{
    paintPC(m_sPC.next());
    
    pthread_mutex_lock ( &m_mutexPC );
    m_sPC.update();
    m_sPC.next()->points_.clear();
    m_sPC.next()->colors_.clear();
    m_sPC.next()->normals_.clear();
    pthread_mutex_unlock ( &m_mutexPC );
}

void _PCframe::paintPC ( PointCloud* pPC )
{
    NULL_ ( pPC );
    IF_ ( m_vColOvrr.x < 0.0 )

    pPC->PaintUniformColor (
        Eigen::Vector3d (
            m_vColOvrr.x,
            m_vColOvrr.y,
            m_vColOvrr.z
        )
    );
}

int _PCframe::size ( void )
{
    return m_sPC.prev()->points_.size();
}

void _PCframe::addP(Eigen::Vector3d& vP, Eigen::Vector3d& vC, uint64_t& tStamp)
{
    IF_(check() < 0);

    Eigen::Vector3d vPt = m_A * vP;
    m_ringB.addP(vPt, vC, tStamp);    
}

void _PCframe::updateTransformMatrix(void)
{
	Eigen::Matrix4d mT;
	Eigen::Vector3d vR(m_vR.x, m_vR.y, m_vR.z);
    mT.block(0,0,3,3) = Geometry3D::GetRotationMatrixFromXYZ(vR);
	mT(0,3) = m_vT.x;
	mT(1,3) = m_vT.y;
	mT(2,3) = m_vT.z;

    m_A = mT;
}

void _PCframe::setTranslation(vDouble3& vT)
{
	m_vT = vT;
}

vDouble3 _PCframe::getTranslation(void)
{
	return m_vT;
}

void _PCframe::setRotation(vDouble3& vR)
{
	m_vR = vR;
}

vDouble3 _PCframe::getRotation(void)
{
	return m_vR;
}

void _PCframe::draw ( void )
{
    this->_ModuleBase::draw();
}

}
#endif