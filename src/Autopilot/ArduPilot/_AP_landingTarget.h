#ifndef OpenKAI_src_Autopilot_AP__AP_landingTarget_H_
#define OpenKAI_src_Autopilot_AP__AP_landingTarget_H_

#include "../../Universe/_Universe.h"
#include "../../Sensor/Distance/_DistSensorBase.h"
#include "../../Utility/util.h"
#include "_AP_move.h"

namespace kai
{
	struct AP_LANDING_TARGET_TAG
	{
		int m_id = -1;
		int m_priority = 0;
	};

	class _AP_landingTarget : public _AP_move
	{
	public:
		_AP_landingTarget();
		~_AP_landingTarget();

		virtual bool init(void *pKiss);
		virtual bool link(void);
		virtual bool start(void);
		virtual int check(void);
		virtual void update(void);
		virtual void console(void *pConsole);

	protected:
		virtual AP_LANDING_TARGET_TAG* getTag(int id);
		virtual bool findTag(void);
		virtual void updateLandingTarget(void);
		static void *getUpdate(void *This)
		{
			((_AP_landingTarget *)This)->update();
			return NULL;
		}

	protected:
		_DistSensorBase* m_pDS;
		_Universe *m_pU;
		_Object m_oTarget;

		vector<AP_LANDING_TARGET_TAG> m_vTags;
		vFloat2 m_vFov; // cam FOV horiz/vert
		vFloat2 m_vPsp; // target set point on screen coord
		mavlink_landing_target_t m_lt;

		// heading adjustment
		bool m_bHdg;
		bool m_bHdgMoving;
		float m_hdgSp;
		float m_hdgDz;
	};
}
#endif
