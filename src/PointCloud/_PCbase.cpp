/*
 * _PCbase.cpp
 *
 *  Created on: May 24, 2020
 *      Author: yankai
 */

#ifdef USE_OPEN3D
#include "_PCbase.h"

namespace kai
{

    _PCbase::_PCbase()
    {
        m_type = pc_unknown;

        m_vToffset.init(0);
        m_vRoffset.init(0);
        m_mToffset = Matrix4d::Identity();
        m_Aoffset = Matrix4d::Identity();

        m_vT.init(0);
        m_vR.init(0);
        m_mT = Matrix4d::Identity();
        m_A = Matrix4d::Identity();

        m_vToffsetRGB.init(0);
        m_vRoffsetRGB.init(0);
        m_mToffsetRGB = Matrix4d::Identity();

        m_vAxisIdx.init(0,1,2);
        m_vAxisK.init(1.0);
        m_unitK = 1.0;
        m_vRange.init(0.0, 1000.0);
        
        m_shade = pcShade_original;
        m_vShadeCol.init(1.0);
        m_rShadePosCol = 1000.0;

        m_pInCtx.init();
        m_pV = NULL;
    }

    _PCbase::~_PCbase()
    {
    }

    bool _PCbase::init(void *pKiss)
    {
        IF_F(!this->_ModuleBase::init(pKiss));
        Kiss *pK = (Kiss *)pKiss;

        pK->v("shade", (int*)&m_shade);
        pK->v("vShadeCol", &m_vShadeCol);
        pK->v("rShadePosCol", &m_rShadePosCol);
        m_rShadePosCol = 1.0/m_rShadePosCol;

        pK->v("vAxisIdx", &m_vAxisIdx);
        pK->v("vAxisK", &m_vAxisK);
        pK->v("unitK", &m_unitK);
        m_vAxisK *= m_unitK;

        pK->v("vRange", &m_vRange);
        m_vRange.x *= m_vRange.x;
        m_vRange.y *= m_vRange.y;

        //origin offset
        pK->v("vToffset", &m_vToffset);
        pK->v("vRoffset", &m_vRoffset);
        setOffset(m_vToffset, m_vRoffset);

        //transform
        pK->v("vT", &m_vT);
        pK->v("vR", &m_vR);
        setTranslation(m_vT, m_vR);

        //RGB offset
        pK->v("vToffsetRGB", &m_vToffsetRGB);
        pK->v("vRoffsetRGB", &m_vRoffsetRGB);
        m_mToffsetRGB = getTranslationMatrix(m_vToffsetRGB, m_vRoffsetRGB);

        //pipeline ctx
        pK->v("ctxdT", &m_pInCtx.m_dT);

        string n;
        n = "";
        pK->v("_PCbase", &n);
        m_pInCtx.m_pPCB = (_PCbase *)(pK->getInst(n));

        m_nPread = 0;

        n = "";
        pK->v("_Remap", &n);
        m_pV = (_Remap *)(pK->getInst(n));

        return true;
    }

    int _PCbase::check(void)
    {
        return this->_ModuleBase::check();
    }

    PC_TYPE _PCbase::getType(void)
    {
        return m_type;
    }

    Matrix4d _PCbase::getTranslationMatrix(const vDouble3 &vT, const vDouble3 &vR)
    {
        Matrix4d mT = Matrix4d::Identity();
        Vector3d eR(vR.x, vR.y, vR.z);
        mT.block(0, 0, 3, 3) = Geometry3D::GetRotationMatrixFromXYZ(eR);
        mT(0, 3) = vT.x;
        mT(1, 3) = vT.y;
        mT(2, 3) = vT.z;

        return mT;
    }

    void _PCbase::setOffset(const vDouble3 &vT, const vDouble3 &vR)
    {
        m_vToffset = vT;
        m_vRoffset = vR;
        m_mToffset = getTranslationMatrix(vT, vR);
        m_Aoffset = m_mToffset;
    }

    void _PCbase::setRGBoffset(const vDouble3 &vT, const vDouble3 &vR)
    {
        m_vToffsetRGB = vT;
        m_vRoffsetRGB = vR;
        m_mToffsetRGB = getTranslationMatrix(vT, vR);
    }

    void _PCbase::updateRGBmatrix(void)
    {
        NULL_(m_pV);
        IF_(m_pV->getType() != vision_remap);

        Mat mC =  m_pV->mC();
        Matrix4d mRGB = Matrix4d::Zero();
		mRGB(0,0) = mC.at<double>(0,0);
		mRGB(1,1) = mC.at<double>(1,1);
		mRGB(0,2) = mC.at<double>(0,2);
		mRGB(1,2) = mC.at<double>(1,2);

        m_mRGB = mRGB * m_mToffsetRGB;
    }

    void _PCbase::setTranslation(const vDouble3 &vT, const vDouble3 &vR)
    {
        m_vT = vT;
        m_vR = vR;
        m_mT = getTranslationMatrix(m_vT, m_vR) * m_mToffset;
        m_A = m_mT;
    }

    void _PCbase::setTranslation(const Matrix4d &mT)
    {
        m_mT = mT * m_mToffset;
        m_A = m_mT;
    }

    void _PCbase::readPC(void *pPC)
    {
        NULL_(pPC);

        PC_TYPE t = ((_PCbase *)pPC)->getType();

        if (t == pc_stream)
            getStream(pPC);
        else if (t == pc_frame)
            getFrame(pPC);
        else if (t == pc_lattice)
            getLattice(pPC);
    }

    int _PCbase::nPread(void)
    {
        return m_nPread;
    }

    void _PCbase::clear(void)
    {
    }

    void _PCbase::getStream(void *p)
    {
    }

    void _PCbase::getFrame(void *p)
    {
    }

    void _PCbase::getLattice(void *p)
    {
    }

    Vector3d _PCbase::getColor(const Vector3d &vP)
    {
        Vector3d vN(0,0,0);
        NULL__(m_pV, vN);
        IF__(m_pV->BGR(), vN);
        IF__(m_pV->BGR()->bEmpty(), vN);

        Vector4d vPt{vP[0], vP[1], vP[2], 1};
        Vector4d vPrgb = m_mRGB * vPt;
        Vec3f vC = m_pV->BGR()->m()->at<Vec3f>(vPrgb[0], vPrgb[1]);

        return Vector3d(vC[0], vC[1], vC[2]);
    }

    bool _PCbase::bRange(const Vector3d& vP)
    {
        double ds = vP[0]*vP[0] + vP[1]*vP[1] + vP[2]*vP[2];
        IF_F(ds < m_vRange.x);
        IF_F(ds > m_vRange.y);

        return true;
    }

}
#endif
