/*
 * _Livox.h
 *
 *  Created on: Aug 21, 2019
 *      Author: yankai
 */

#ifndef OpenKAI_src_Sensor__Livox_H_
#define OpenKAI_src_Sensor__Livox_H_

#include "../../PointCloud/_PCbase.h"

#ifdef USE_LIVOX
#include "lds_lidar.h"

namespace kai
{

class _Livox: public _PCbase
{
public:
    _Livox();
    ~_Livox();

    virtual bool init ( void* pKiss );
    virtual int check ( void );
    virtual bool start ( void );
    virtual void draw ( void );

protected:
    static void CbRecvData(LivoxEthPacket* pData, void* pLivox);
    bool open ( void );
    void close ( void );
    bool updateLidar ( void );
    void update ( void );
    static void* getUpdate ( void* This )
    {
        ( ( _Livox* ) This )->update();
        return NULL;
    }
    
public:
    bool m_bOpen;
    string m_broadcastCode;
    LdsLidar* m_pL;
    uint32_t m_iTransformed;

};

}
#endif
#endif