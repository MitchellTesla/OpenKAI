/*
 * _PCstream.cpp
 *
 *  Created on: May 24, 2020
 *      Author: yankai
 */

#include "_PCstream.h"

namespace kai
{

    _PCstream::_PCstream()
    {
        m_type = pc_stream;

        m_pP = NULL;
        m_nP = 256;
        m_iP = 0;
    }

    _PCstream::~_PCstream()
    {
        m_iP = 0;
        m_nP = 0;
        DEL(m_pP);
    }

    bool _PCstream::init(void *pKiss)
    {
        IF_F(!this->_GeometryBase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

        pK->v("nP", &m_nP);
        IF_F(m_nP <= 0);

        return initBuffer();
    }

    bool _PCstream::start(void)
    {
        NULL_F(m_pT);
        return m_pT->start(getUpdate, this);
    }

    void _PCstream::update(void)
    {
        while (m_pT->bAlive())
        {
            m_pT->autoFPSfrom();

            updatePCstream();

            m_pT->autoFPSto();
        }
    }

    int _PCstream::check(void)
    {
        NULL__(m_pP, -1);

        return this->_GeometryBase::check();
    }

    void _PCstream::updatePCstream(void)
    {
        IF_(check() < 0);

        readFromSharedMem();
        writeToSharedMem();
    }

    bool _PCstream::initBuffer(void)
    {
        mutexLock();

        m_pP = new GEOMETRY_POINT[m_nP];
        NULL_F(m_pP);
        m_iP = 0;

        for (int i = 0; i < m_nP; i++)
            m_pP[i].clear();

        mutexUnlock();

        return true;
    }

    void _PCstream::clear(void)
    {
        mutexLock();

        for (int i = 0; i < m_nP; i++)
            m_pP[i].clear();

        m_iP = 0;

        mutexUnlock();
    }

    void _PCstream::getPCstream(void *p, const uint64_t& tExpire)
    {
        IF_(check() < 0);
        NULL_(p);
        _PCstream *pS = (_PCstream *)p;

        mutexLock();

        uint64_t tNow = getApproxTbootUs();

        for (int i = 0; i < pS->m_nP; i++)
        {
            GEOMETRY_POINT* pP = &pS->m_pP[i];
            IF_CONT(bExpired(pP->m_tStamp, tExpire, tNow));

            m_pP[m_iP] = *pP;
            m_iP = iInc(m_iP, m_nP);
        }

        mutexUnlock();
    }

    void _PCstream::copyTo(PointCloud *pPC, const uint64_t& tExpire)
    {
        IF_(check() < 0);
        NULL_(pPC);

        pPC->Clear();
        pPC->points_.clear();
        pPC->colors_.clear();

        uint64_t tNow = getApproxTbootUs();

        for (int i = 0; i < m_nP; i++)
        {
            GEOMETRY_POINT* pP = &m_pP[i];
            IF_CONT(bExpired(pP->m_tStamp, tExpire, tNow));

        	pPC->points_.push_back(pP->m_vP);
        	pPC->colors_.push_back(pP->m_vC.cast<double>());
        }
    }

    void _PCstream::add(const Vector3d &vP, const Vector3f &vC, const uint64_t& tStamp)
    {
        GEOMETRY_POINT *pP = &m_pP[m_iP];
        pP->m_vP = vP;
        pP->m_vC = vC;
        pP->m_tStamp = tStamp;

        m_iP = iInc(m_iP, m_nP);
    }

    GEOMETRY_POINT* _PCstream::get(int i)
    {
        IF_N(i >= m_nP);

        return &m_pP[i];
    }

    int _PCstream::nP(void)
    {
        return m_nP;
    }

    int _PCstream::iP(void)
    {
        return m_iP;
    }

    void _PCstream::writeToSharedMem(void)
    {
        IF_(!m_bWriteSharedMem);
        NULL_(m_psmG);
        IF_(!m_psmG->bOpen());

		memcpy(m_psmG->p(), m_pP, m_nP * sizeof(GEOMETRY_POINT));
    }

    void _PCstream::readFromSharedMem(void)
    {
        IF_(!m_bReadSharedMem);
        NULL_(m_psmG);
        IF_(!m_psmG->bOpen());

		memcpy(m_pP, m_psmG->p(), m_nP * sizeof(GEOMETRY_POINT));
    }

}
