/*
 * _IsingSolver.h
 *
 *  Created on: Feb 2, 2024
 *      Author: yankai
 */

#ifndef OpenKAI_src_Science_IsingSolver_H_
#define OpenKAI_src_Science_IsingSolver_H_

#include "../../BASE/_ModuleBase.h"

namespace kai
{
	class _IsingSolver : public _ModuleBase
	{
	public:
		_IsingSolver();
		virtual ~_IsingSolver();

		virtual bool init(void *pKiss);
		virtual bool link(void);
		virtual bool start(void);
		virtual int check(void);
		virtual void console(void *pConsole);

	private:
		void updateSolver(void);
		void update(void);
		static void *getUpdate(void *This)
		{
			((_IsingSolver *)This)->update();
			return NULL;
		}

	protected:
		// post processing thread
		_Thread *m_pTPP;
	};

}
#endif
