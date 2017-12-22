#pragma once

class Max1000Packer
{
public:
	Max1000Packer(unsigned char num)
	{
		m_cmd[0] = 0xB0 + num;
	};

	~Max1000Packer(void)	{};

	void SetDeviceNum(unsigned char num)
	{
		m_cmd[0] = 0xB0 + num;
	}

	const unsigned char* PanRight(unsigned char speed)
	{
		m_cmd[1] = 0x52;
		m_cmd[2] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* PanLeft(unsigned char speed)
	{
		m_cmd[1] = 0x4C;
		m_cmd[2] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* TiltUp(unsigned char speed)
	{
		m_cmd[1] = 0x44;
		m_cmd[2] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* TiltDown(unsigned char speed)
	{
		m_cmd[1] = 0x55;
		m_cmd[2] = speed;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* ZoomIn()
	{
		m_cmd[1] = 0x49;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* ZoomOut()
	{
		m_cmd[1] = 0x4F;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* FocusNear()
	{
		m_cmd[1] = 0x4E;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* FocusFar()
	{
		m_cmd[1] = 0x46;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}


	const unsigned char* IrisOpen()
	{
		m_cmd[1] = 0x42;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* IrisClose()
	{
		m_cmd[1] = 0x41;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopPan()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopTilt()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x02;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopFocus()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x03;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopZoom()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x04;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopIris()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x05;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopAll()
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x00;
		CalcCheck();

		return m_cmd;
	}


	const unsigned char* StartAuxiliary(unsigned char num)
	{
		m_cmd[1] = 0x50 + num;
		m_cmd[2] = 0x01;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* StopAuxiliary(unsigned char num)
	{
		m_cmd[1] = 0x48;
		m_cmd[2] = 0x05+num;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* SetPreset(unsigned char presetNum)
	{
		m_cmd[1] = 0x53;
		m_cmd[2] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* UsePreset(unsigned char presetNum)
	{
		m_cmd[1] = 0x56;
		m_cmd[2] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* DelPreset(unsigned char presetNum)
	{
		m_cmd[1] = 0x53;
		m_cmd[2] = presetNum;
		CalcCheck();

		return m_cmd;
	}

	const unsigned char* GetRaw()
	{
		return m_cmd;
	}

private:
	unsigned char   m_cmd[4];
private:
	void CalcCheck()
	{
		m_cmd[3] = (m_cmd[0]+m_cmd[1]+m_cmd[2])&0x7F;
	}
};
