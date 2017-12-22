#pragma once

class PelcopPacker
{
public:
	PelcopPacker(unsigned char num)
	{
		m_cmd[0] = 0xa0;
		m_cmd[1] = num;
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x00;
		m_cmd[4] = 0x00;
		m_cmd[5] = 0x00;
		m_cmd[6] = 0xaf;
	};

	~PelcopPacker(void)	{};

	void SetDeviceNum(unsigned char num)
	{
		m_cmd[1] = num;
	}

	const unsigned char* PanRight(unsigned char speed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x02;
		m_cmd[4] = speed;
		m_cmd[5] = 0x00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* PanLeft(unsigned char speed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x04;
		m_cmd[4] = speed;
		m_cmd[5] = 0x00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* TiltUp(unsigned char speed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x08;
		m_cmd[4] = 0x00;
		m_cmd[5] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* TiltDown(unsigned char speed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x10;
		m_cmd[4] = 0x00;
		m_cmd[5] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* LeftUp(unsigned char pspeed,unsigned char tspeed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x0c;
		m_cmd[4] = pspeed;
		m_cmd[5] = tspeed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* RightUp(unsigned char pspeed,unsigned char tspeed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x0a;
		m_cmd[4] = pspeed;
		m_cmd[5] = tspeed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* LeftDown(unsigned char pspeed,unsigned char tspeed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x14;
		m_cmd[4] = pspeed;
		m_cmd[5] = tspeed;
		CalcCheck();

		return m_cmd;
	}


	const unsigned char* RightDown(unsigned char pspeed,unsigned char tspeed)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x12;
		m_cmd[4] = pspeed;
		m_cmd[5] = tspeed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* ZoomIn()
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x20;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* ZoomOut()
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x40;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}
	const unsigned char* FocusNear()
	{
		m_cmd[2] = 0x20;
		m_cmd[3] = 0x00;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* FocusFar()
	{
		m_cmd[2] = 0x01;
		m_cmd[3] = 0x00;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}


	const unsigned char* IrisOpen()
	{
		m_cmd[2] = 0x08;
		m_cmd[3] = 0x00;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* IrisClose()
	{
		m_cmd[2] = 0x04;
		m_cmd[3] = 0x00;
		m_cmd[4] = 00;
		m_cmd[5] = 00;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopPan()
	{
		m_cmd[3] &= 0xf8;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopTilt()
	{
		m_cmd[3] &= 0xe6;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopFocus()
	{
		m_cmd[2] &= 0x7c;
		CalcCheck();

		return m_cmd;
	}
	const unsigned char* StopZoom()
	{
		m_cmd[3] &= 0x9e;
		CalcCheck();

		return m_cmd;
	}
	const unsigned char* StopIris()
	{
		m_cmd[2] &= 0x73;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopAll()
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x00;
		CalcCheck();

		return m_cmd;
	}


	const unsigned char* StartAuxiliary(unsigned char num)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x09;
		m_cmd[4] = 0x00;
		m_cmd[5] = num;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopAuxiliary(unsigned char num)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x0b;
		m_cmd[4] = 0x00;
		m_cmd[5] = num;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* SetPreset(unsigned char presetNum)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x03;
		m_cmd[4] = 0x00;
		m_cmd[5] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* UsePreset(unsigned char presetNum)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x07;
		m_cmd[4] = 0x00;
		m_cmd[5] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* DelPreset(unsigned char presetNum)
	{
		m_cmd[2] = 0x00;
		m_cmd[3] = 0x05;
		m_cmd[4] = 0x00;
		m_cmd[5] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* GetRaw()
	{
		return m_cmd;
	}

private:
	unsigned char   m_cmd[8];
private:
	void CalcCheck()
	{
		m_cmd[7] = m_cmd[1]^m_cmd[2]^m_cmd[3]^m_cmd[4]^m_cmd[5];
	}
};
