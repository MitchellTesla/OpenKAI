#ifndef OpenKAI_src_Application_DroneSR__AP_SR_H_
#define OpenKAI_src_Application_DroneSR__AP_SR_H_

#include "../../IO/_File.h"
#include "../../Net/_Curl.h"
#include "../../Utility/util.h"
#include "../../Autopilot/ArduPilot/_AP_base.h"
#include "../../Vision/_VisionBase.h"
#include "../../Universe/_Universe.h"

using namespace picojson;

namespace kai
{

	class _AP_SR : public _ModuleBase
	{
	public:
		_AP_SR();
		~_AP_SR();

		bool init(void *pKiss);
		bool link(void);
		bool start(void);
		void update(void);
		int check(void);
		void console(void *pConsole);

	private:
		void updateSR(void);
		static void *getUpdate(void *This)
		{
			((_AP_SR *)This)->update();
			return NULL;
		}

	private:
		_VisionBase *m_pV;
		_Universe *m_pU;
		_AP_base *m_pAP;
		_Curl* m_pCurl;

		string m_fName;
		string m_dir;

		int m_iWP;
		vInt2 m_vWPrange;

		vector<int> m_vClass;
	};

}
#endif
