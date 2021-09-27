#include "_SBus.h"

namespace kai
{
	_SBus::_SBus()
	{
		m_pTr = NULL;
		m_pIO = NULL;
		m_bSend = false;

		reset();
	}

	_SBus::~_SBus()
	{
	}

	bool _SBus::init(void *pKiss)
	{
		IF_F(!this->_ModuleBase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		pK->v("bSend", &m_bSend);

		string n;
		n = "";
		F_ERROR_F(pK->v("_IOBase", &n));
		m_pIO = (_IOBase *)(pK->getInst(n));
		NULL_Fl(m_pIO, "_IOBase not found");

		Kiss *pKt = pK->child("threadR");
		IF_F(pKt->empty());

		m_pTr = new _Thread();
		if (!m_pTr->init(pKt))
		{
			DEL(m_pTr);
			return false;
		}

		return true;
	}

	bool _SBus::start(void)
	{
		NULL_F(m_pT);
		NULL_F(m_pTr);

		if(m_bSend)
		{
			IF_F(!m_pT->start(getUpdateW, this));
		}
		
		return m_pTr->start(getUpdateR, this);
	}

	void _SBus::updateW(void)
	{
		while (m_pT->bRun())
		{
			if (!m_pIO)
			{
				m_pT->sleepT(SEC_2_USEC);
				continue;
			}

			if (!m_pIO->isOpen())
			{
				if (!m_pIO->open())
				{
					m_pT->sleepT(SEC_2_USEC);
					continue;
				}
			}

			m_pT->autoFPSfrom();

			send();

			m_pT->autoFPSto();
		}
	}

	void _SBus::send(void)
	{
		IF_(!m_pIO->isOpen());

//		m_pIO->write(m_pB, 16);
	}

	void _SBus::updateR(void)
	{
		while (m_pTr->bRun())
		{
			m_pTr->autoFPSfrom();

			if (recv())
			{
				decode();
				reset();
			}

			m_pTr->autoFPSto();
		}
	}

	bool _SBus::recv(void)
	{
		IF_F(check() < 0);

		uint8_t B;
		while (m_pIO->read(&B, 1) > 0)
		{
			if (m_iB == 0)
			{
				IF_CONT(B != SBUS_HEADER_);
			}

			m_pB[m_iB++] = B;
			IF_CONT(m_iB < SBUS_LEN_);

			m_iB = 0;
			uint8_t cs = checksum(&m_pB[1], 33);
			IF_CONT(m_pB[SBUS_LEN_ - 1] != cs);

			return true;
		}

		return false;
	}

	void _SBus::decode(void)
	{
		for (int i = 0; i < 16; i++)
		{
			int j = i * 2;
			m_pRC[i].set(static_cast<uint16_t>(m_pB[j + 1] << 8 | m_pB[j + 2]));
		}

		m_flag = m_pB[SBUS_LEN_ - 2];
	}

	void _SBus::encode(void)
	{
		for (int i = 0; i < 16; i++)
		{
			int j = i * 2;
			m_pB[j + 1] = static_cast<uint8_t>(m_pRC[i].raw() >> 8);
			m_pB[j + 2] = static_cast<uint8_t>(m_pRC[i].raw() & 0x00FF);
		}

		m_pB[34] = checksum(&m_pB[1], 33);
	}

	uint8_t _SBus::checksum(uint8_t *pB, uint8_t n)
	{
		uint8_t checksum = 0;

		for (int i = 0; i < n; ++i)
			checksum ^= pB[i];

		return checksum;
	}

	void _SBus::reset(void)
	{
		m_iB = 0;
		m_flag = 0;
		m_bLostFrame = false;
		m_bFailSafe = false;
		m_bCh17 = false;
		m_bCh18 = false;
	}

	uint16_t _SBus::raw(int i)
	{
		IF__(i >= SBUS_NCHAN, 0);

		return m_pRC[i].raw();
	}

	float _SBus::v(int i)
	{
		IF__(i >= SBUS_NCHAN, 0);

		return m_pRC[i].v();
	}

	void _SBus::console(void *pConsole)
	{
#ifdef WITH_UI
		NULL_(pConsole);
		this->_ModuleBase::console(pConsole);

		_Console *pC = (_Console *)pConsole;
		if (!m_pIO->isOpen())
			pC->addMsg("Not connected");
		else
			pC->addMsg("Connected");

		pC->addMsg("Raw: " 	+ i2str(m_pRC[0].raw()) + "|"
						   	+ i2str(m_pRC[1].raw()) + "|"
							+ i2str(m_pRC[2].raw()) + "|"
							+ i2str(m_pRC[3].raw()) + "|"
							+ i2str(m_pRC[4].raw()) + "|"
							+ i2str(m_pRC[5].raw()) + "|"
							+ i2str(m_pRC[6].raw()) + "|"
							+ i2str(m_pRC[7].raw()) + "|"
							+ i2str(m_pRC[8].raw()) + "|"
							+ i2str(m_pRC[9].raw()) + "|"
							+ i2str(m_pRC[10].raw()) + "|"
							+ i2str(m_pRC[11].raw()) + "|"
							+ i2str(m_pRC[12].raw()) + "|"
							+ i2str(m_pRC[13].raw()) + "|"
							+ i2str(m_pRC[14].raw()) + "|"
							+ i2str(m_pRC[15].raw()));

		pC->addMsg("v: " 	+ f2str(m_pRC[0].v(),2) + "|"
						   	+ f2str(m_pRC[1].v(),2) + "|"
							+ f2str(m_pRC[2].v(),2) + "|"
							+ f2str(m_pRC[3].v(),2) + "|"
							+ f2str(m_pRC[4].v(),2) + "|"
							+ f2str(m_pRC[5].v(),2) + "|"
							+ f2str(m_pRC[6].v(),2) + "|"
							+ f2str(m_pRC[7].v(),2) + "|"
							+ f2str(m_pRC[8].v(),2) + "|"
							+ f2str(m_pRC[9].v(),2) + "|"
							+ f2str(m_pRC[10].v(),2) + "|"
							+ f2str(m_pRC[11].v(),2) + "|"
							+ f2str(m_pRC[12].v(),2) + "|"
							+ f2str(m_pRC[13].v(),2) + "|"
							+ f2str(m_pRC[14].v(),2) + "|"
							+ f2str(m_pRC[15].v(),2));
#endif
	}

}
