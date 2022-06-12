#include "_ProtocolBase.h"

namespace kai
{

	_ProtocolBase::_ProtocolBase()
	{
		m_pTr = NULL;
		m_pIO = NULL;
		m_pBuf = NULL;
		m_nBuf = 256;
		m_nCMDrecv = 0;
	}

	_ProtocolBase::~_ProtocolBase()
	{
	}

	bool _ProtocolBase::init(void *pKiss)
	{
		IF_F(!this->_ModuleBase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		pK->v("nBuf", &m_nBuf);
		m_pBuf = new uint8_t[m_nBuf];
		m_recvMsg.init(m_nBuf);

		Kiss *pKt = pK->child("threadR");
		IF_d_F(pKt->empty(), LOG_E("threadR not found"));

		m_pTr = new _Thread();
		if (!m_pTr->init(pKt))
		{
			DEL(m_pTr);
			return false;
		}
		pKt->m_pInst = m_pTr;

		string n;
		n = "";
		F_ERROR_F(pK->v("_IOBase", &n));
		m_pIO = (_IOBase *)(pK->getInst(n));
		NULL_Fl(m_pIO, n + ": not found");

		return true;
	}

	bool _ProtocolBase::start(void)
	{
		NULL_F(m_pT);
		NULL_F(m_pTr);
		IF_F(!m_pT->start(getUpdateW, this));
		return m_pTr->start(getUpdateR, this);
	}

	int _ProtocolBase::check(void)
	{
		NULL__(m_pIO, -1);
		IF__(!m_pIO->isOpen(), -1);
		NULL_F(m_pBuf);

		return this->_ModuleBase::check();
	}

	void _ProtocolBase::updateW(void)
	{
		while (m_pT->bRun())
		{
			m_pT->autoFPSfrom();

			send();

			m_pT->autoFPSto();
		}
	}

	void _ProtocolBase::send(void)
	{
		IF_(check() < 0);

		//m_pIO->write(m_pB, 16);
	}

	void _ProtocolBase::updateR(void)
	{
		while (m_pTr->bRun())
		{
			if (!m_pIO)
			{
				m_pTr->sleepT(SEC_2_USEC);
				continue;
			}

			if (!m_pIO->isOpen())
			{
				m_pTr->sleepT(SEC_2_USEC);
				continue;
			}

			m_pTr->autoFPSfrom();

			while (readCMD())
			{
				handleCMD();
				m_recvMsg.reset();
				m_nCMDrecv++;
			}

			m_pTr->autoFPSto();
		}
	}

	bool _ProtocolBase::readCMD(void)
	{
		uint8_t b;
		int nB;

		while ((nB = m_pIO->read(&b, 1)) > 0)
		{
			if (m_recvMsg.m_cmd != 0)
			{
				m_recvMsg.m_pB[m_recvMsg.m_iB] = b;
				m_recvMsg.m_iB++;

				if (m_recvMsg.m_iB == 3)
				{
					m_recvMsg.m_nPayload = m_recvMsg.m_pB[2];
				}

				IF_T(m_recvMsg.m_iB == m_recvMsg.m_nPayload + PB_N_HDR);
			}
			else if (b == PB_BEGIN)
			{
				m_recvMsg.m_cmd = b;
				m_recvMsg.m_pB[0] = b;
				m_recvMsg.m_iB = 1;
				m_recvMsg.m_nPayload = 0;
			}
		}

		return false;
	}

	void _ProtocolBase::handleCMD(void)
	{
	}

	void _ProtocolBase::console(void *pConsole)
	{
		NULL_(pConsole);
		this->_ModuleBase::console(pConsole);

		_Console *pC = (_Console *)pConsole;
		if (!m_pIO->isOpen())
		{
			pC->addMsg("Not Connected", 1);
			return;
		}

		pC->addMsg("nCMD = " + i2str(m_nCMDrecv), 1);
	}

}
