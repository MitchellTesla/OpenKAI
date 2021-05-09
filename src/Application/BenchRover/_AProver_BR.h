#ifndef OpenKAI_src_Autopilot_AP__AProver_BR_H_
#define OpenKAI_src_Autopilot_AP__AProver_BR_H_

#include "../Autopilot/ArduPilot/_AP_base.h"
#include "../../Control/_Drive.h"
#include "../../Utility/RC.h"

namespace kai
{

struct BENCHROVER_STATE   
{
	int8_t MANUAL;
	int8_t FORWARD;
	int8_t BACKWARD;

	bool bValid(void)
	{
		IF_F(MANUAL < 0);
		IF_F(FORWARD < 0);
		IF_F(BACKWARD < 0);

		return true;
	}
	
	bool assign(_StateControl* pMC)
    {
        NULL_F(pMC);
        MANUAL = pMC->getStateIdxByName ("MANUAL");
        FORWARD = pMC->getStateIdxByName ("FORWARD");
        BACKWARD = pMC->getStateIdxByName ("BACKWARD");
        
        return bValid();
    }
};

class _AProver_BR: public _StateBase
{
public:
	_AProver_BR();
	~_AProver_BR();

	bool init(void* pKiss);
	bool start(void);
	int check(void);
	void update(void);
	void console(void* pConsole);

private:
	void updateMode(void);
	static void* getUpdate(void* This)
	{
		((_AProver_BR*) This)->update();
		return NULL;
	}

public:
	_AP_base* m_pAP;
    _Drive*    m_pD;
    
    RC_CHANNEL m_rcMode;
    BENCHROVER_STATE m_iMode;
     
    RC_CHANNEL m_rcStickV;
    RC_CHANNEL m_rcStickH;
};

}
#endif
