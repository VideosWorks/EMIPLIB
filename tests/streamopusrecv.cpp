#include <mipconfig.h>
#include <mipaudiosession.h>
#include <mippainputoutput.h>
#include <iostream>
#include <jrtplib3/rtpipv4address.h>

using namespace jrtplib;

void checkRet(bool ret,const MIPErrorBase &obj)
{
	if (!ret)
	{
		std::cerr << obj.getErrorString() << std::endl;
		exit(-1);
	}
}

class MyAudioSession : public MIPAudioSession
{
protected:
	void onInputThreadExit(bool err, const std::string &compName, const std::string &errStr)
	{
		if (err)
		{
			std::cerr << "Input chain thread exited due to an error" << std::endl;
			std::cerr << "Component: " << compName << std::endl;
			std::cerr << "Error: " << errStr << std::endl;
		}
	}
	void onOutputThreadExit(bool err, const std::string &compName, const std::string &errStr)
	{
		if (err)
		{
			std::cerr << "Output chain thread exited due to an error" << std::endl;
			std::cerr << "Component: " << compName << std::endl;
			std::cerr << "Error: " << errStr << std::endl;
		}
	}	
};

int main(void)
{
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
	std::string errStr;

	if (!MIPPAInputOutput::initializePortAudio(errStr))
	{
		std::cerr << "Can't initialize PortAudio: " << errStr << std::endl;
		return -1;
	}
#endif // MIPCONFIG_SUPPORT_PORTAUDIO
	
	MIPAudioSessionParams Aparams;
	MyAudioSession audioSess;
	bool ret;

	int audioPort = 8002;
	
	Aparams.setPortbase(audioPort);
	Aparams.setCompressionType(MIPAudioSessionParams::Opus);
	Aparams.setOpusIncomingPayloadType(96);
	Aparams.setSpeexIncomingPayloadType(97);
	
	std::cout << "Starting audio session at portbase " << audioPort << std::endl;

	ret = audioSess.init(&Aparams);
	checkRet(ret, audioSess);

	uint8_t ip[4] = {224, 2, 36, 42 };
	//ret = audioSess.addDestination(RTPIPv4Address(ip, audioPort));
	audioSess.joinMulticastGroup(RTPIPv4Address(ip));

	std::string dummy;
	std::cout << "Type something to exit" << std::endl;
	std::cin >> dummy;

	std::cout << "Exiting..." << std::endl;

	audioSess.destroy();

#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
	MIPPAInputOutput::terminatePortAudio();
#endif // MIPCONFIG_SUPPORT_PORTAUDIO
#ifdef WIN32
	WSACleanup();
#endif // WIN32
	
	return 0;
}

