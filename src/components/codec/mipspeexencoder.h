/*
    
  This file is a part of EMIPLIB, the EDM Media over IP Library.
  
  Copyright (C) 2006-2007  Hasselt University - Expertise Centre for
                      Digital Media (EDM) (http://www.edm.uhasselt.be)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA

*/

/**
 * \file mipspeexencoder.h
 */

#ifndef MIPSPEEXENCODER_H

#define MIPSPEEXENCODER_H

#include "mipconfig.h"

#ifdef MIPCONFIG_SUPPORT_SPEEX

#include "mipcomponent.h"
#include "miptime.h"
#include <speex/speex.h>
#include <list>

class MIPEncodedAudioMessage;

/** Compress audio using the Speex codec.
 *  Using this component, floating point mono raw audio messages and raw 16 bit raw audio
 *  messages can be compressed using the Speex codec. Messages generated by this component 
 *  are encoded audio messages with subtype MIPENCODEDAUDIOMESSAGE_TYPE_SPEEX.
 */
class MIPSpeexEncoder : public MIPComponent
{
public:
	/** Used to select speex encoding type. */
	enum SpeexBandWidth 
	{ 
 		/** Narrow band mode (8000 Hz) */
		NarrowBand,
	 	/** Wide band mode (16000 Hz) */
		WideBand,		
 		/** Ultra wide band mode (32000 Hz) */
		UltraWideBand
	};
	
	MIPSpeexEncoder();
	~MIPSpeexEncoder();

	/** Initialize the Speex encoding component.
	 *  This function initializes the Speex encoding component. 
	 *  \param bandWidth Selects the encoding type. 
	 *  \param quality Speex quality parameter.
	 *  \param complexity Speex complexity parameter.
	 *  \param vad Enable or disable voice activity detection.
	 *  \param dtx Enable or disable discontinuous transmission.
	 */
	bool init(SpeexBandWidth bandWidth = WideBand, int quality = 10, int complexity = 1, bool vad = true, bool dtx = false);

	/** Clean up the Speex encoder. */
	bool destroy();

	bool push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg);
	bool pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg);
private:
	void clearMessages();
	
	bool m_init;
	void *m_pState;
	SpeexBits m_bits;
	int m_sampRate;
	int m_numFrames;
	float *m_pFloatBuffer;
	int64_t m_prevIteration;

	std::list<MIPEncodedAudioMessage *> m_messages;
	std::list<MIPEncodedAudioMessage *>::const_iterator m_msgIt;
};	

#endif // MIPCONFIG_SUPPORT_SPEEX

#endif // MIPSPEEXENCODER_H

