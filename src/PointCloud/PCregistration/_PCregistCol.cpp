/*
 * _PCregistCol.cpp
 *
 *  Created on: Sept 6, 2020
 *      Author: yankai
 */

#ifdef USE_OPEN3D
#include "_PCregistCol.h"

namespace kai
{

    _PCregistCol::_PCregistCol()
    {
        m_rVoxel = 0.1;
        m_rNormal = 0.2;
        m_maxNNnormal = 30;
        m_rFitness = 1e-6;
        m_rRMSE = 1e-6;
        m_maxIter = 30;
        m_lastFit = 0.0;

        m_pSrc = NULL;
        m_pTgt = NULL;
        m_pTf = NULL;
    }

    _PCregistCol::~_PCregistCol()
    {
    }

    bool _PCregistCol::init(void *pKiss)
    {
        IF_F(!_ModuleBase::init(pKiss));
        Kiss *pK = (Kiss *)pKiss;

        pK->v("rVoxel", &m_rVoxel);
        pK->v("rNormal", &m_rNormal);
        pK->v("maxNNnormal", &m_maxNNnormal);
        pK->v("rFitness", &m_rFitness);
        pK->v("rRMSE", &m_rRMSE);
        pK->v("maxIter", &m_maxIter);

        string n;

        n = "";
        pK->v("_PCbaseSrc", &n);
        m_pSrc = (_PCframe *)(pK->getInst(n));
        IF_Fl(!m_pSrc, n + ": not found");

        n = "";
        pK->v("_PCbaseTgt", &n);
        m_pTgt = (_PCframe *)(pK->getInst(n));
        IF_Fl(!m_pTgt, n + ": not found");

        n = "";
        pK->v("_PCtransform", &n);
        m_pTf = (_PCtransform *)(pK->getInst(n));
        IF_Fl(!m_pTf, n + ": not found");

        return true;
    }

    bool _PCregistCol::start(void)
    {
        NULL_F(m_pT);
        return m_pT->start(getUpdate, this);
    }

    int _PCregistCol::check(void)
    {
        NULL__(m_pSrc, -1);
        NULL__(m_pTgt, -1);
        NULL__(m_pTf, -1);

        return _ModuleBase::check();
    }

    void _PCregistCol::update(void)
    {
        while (m_pT->bRun())
        {
            m_pT->autoFPSfrom();

            updateRegistration();

            m_pT->autoFPSto();
        }
    }

    void _PCregistCol::updateRegistration(void)
    {
        IF_(check() < 0);

        PointCloud pcSrc;
        m_pSrc->getPC ( &pcSrc );
        PointCloud pcTgt;
        m_pTgt->getPC ( &pcTgt );

        IF_(pcSrc.IsEmpty());
        IF_(pcTgt.IsEmpty());

        pcSrc.EstimateNormals(KDTreeSearchParamHybrid(m_rNormal, m_maxNNnormal ));
        pcTgt.EstimateNormals(KDTreeSearchParamHybrid(m_rNormal, m_maxNNnormal ));

        m_RR = RegistrationColoredICP
                (
                    pcSrc,
                    pcTgt,
                    m_rVoxel,
                    m_RR.transformation_,
                    TransformationEstimationForColoredICP(),
                    ICPConvergenceCriteria(m_rFitness,
                                           m_rRMSE,
                                           m_maxIter)
                );

        IF_(m_RR.fitness_ < m_lastFit);
        m_lastFit = m_RR.fitness_;

        m_pTf->setTranslationMatrix ( m_RR.transformation_ );
    }

    void _PCregistCol::draw(void)
    {
        this->_ModuleBase::draw();
        addMsg("Fitness = " + f2str((float)m_RR.fitness_) +
               ", Inliner_rmse = " + f2str((float)m_RR.inlier_rmse_));
    }

}
#endif
