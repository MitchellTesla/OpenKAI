#ifndef OpenKAI_src_Application_CamCalib__CamCalib_H_
#define OpenKAI_src_Application_CamCalib__CamCalib_H_

#ifdef USE_OPENCV
#include "../../Base/_ModuleBase.h"

namespace kai
{
	namespace
	{
		int CHECKERBOARD[2]{6, 9};
	}

	class _CamCalib : public _ModuleBase
	{
	public:
		_CamCalib();
		~_CamCalib();

		bool init(void *pKiss);
		bool start(void);

		bool calibRGB(const char *pPath);
		Mat camMatrix(void);
		Mat distCoeffs(void);

	protected:
		void update(void);
		static void *getUpdate(void *This)
		{
			((_CamCalib *)This)->update();
			return NULL;
		}

	public:
		string m_path;
		Mat m_mCam;
		Mat m_mDistCoeffs;
	};

}
#endif
#endif
