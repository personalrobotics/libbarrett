/**
 *	Copyright 2009-2014 Barrett Technology <support@barrett.com>
 *
 *	This file is part of libbarrett.
 *
 *	This version of libbarrett is free software: you can redistribute it
 *	and/or modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, either version 3 of the
 *	License, or (at your option) any later version.
 *
 *	This version of libbarrett is distributed in the hope that it will be
 *	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this version of libbarrett.  If not, see
 *	<http://www.gnu.org/licenses/>.
 *
 *
 *	Barrett Technology Inc.
 *	73 Chapel Street
 *	Newton, MA 02458
 *
 */

/*
 * tactile_puck.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: dc
 */

#include <stdexcept>

#include <barrett/os.h>
#include <barrett/bus/abstract/communications_bus.h>
#include <barrett/products/puck.h>
#include <barrett/products/abstract/special_puck.h>
#include <barrett/products/tactile_puck.h>


namespace barrett {


void TactilePuck::setPuck(Puck* puck)
{
	// Call super
	SpecialPuck::setPuck(puck);

	if (p != NULL) {
		bus = &p->getBus();
		id = p->getId();
		propId = p->getPropertyId(Puck::TACT);


		// Check for initialization error
		if (p->getProperty(Puck::TACTID) == -2) {
			(logMessage("TactilePuck::%s(): TACT initialization error for ID = %d. "
					"TACTID = -2")
					% __func__ % id).raise<std::runtime_error>();
		}

		tact = NONE;
		p->setProperty(Puck::TACT, tact);

		tare();
	}
}

void TactilePuck::requestFull()
{
	if (tact == FULL_FORMAT) {
		int ret = Puck::sendGetPropertyRequest(*bus, id, propId);
		if (ret != 0) {
			(logMessage("TactilePuck::%s(): Failed to send request. "
					"Puck::sendGetPropertyRequest() returned error %d.")
					% __func__ % ret).raise<std::runtime_error>();
		}
	} else {
		tact = FULL_FORMAT;
		p->setProperty(Puck::TACT, tact);
	}
}
void TactilePuck::receiveFull(bool realtime)
{
	for (size_t i = 0 ; i < NUM_FULL_MESSAGES; ++i) {
		int ret = Puck::receiveGetPropertyReply<FullTactParser>(*bus, id, propId, &tactile, true, realtime);
		if (ret != 0) {
			const size_t NFM = NUM_FULL_MESSAGES;  // Reserve storage for static const.
			(logMessage("TactilePuck::%s(): Failed to receive reply. "
					"Puck::receiveGetPropertyReply() returned error %d while receiving FULL TACT reply %d of %d from ID=%d.")
					% __func__ % ret % (i+1) % NFM % id).raise<std::runtime_error>();
		}
	}
}

void TactilePuck::requestTop10()
{
	if (tact == TOP10_FORMAT) {
		int ret = Puck::sendGetPropertyRequest(*bus, id, propId);
		if (ret != 0) {
			(logMessage("TactilePuck::%s(): Failed to send request. "
					"Puck::sendGetPropertyRequest() returned error %d.")
					% __func__ % ret).raise<std::runtime_error>();
		}
	} else {
		tact = TOP10_FORMAT;
		p->setProperty(Puck::TACT, tact);
	}
}
void TactilePuck::receiveTop10(bool realtime)
{
	int ret = Puck::receiveGetPropertyReply<Top10TactParser>(*bus, id, propId, &tactile, true, realtime);
	if (ret != 0) {
		(logMessage("TactilePuck::%s(): Failed to receive reply. "
				"Puck::receiveGetPropertyReply() returned error %d while receiving TOP10 TACT reply from ID=%d.")
				% __func__ % ret % id).raise<std::runtime_error>();
	}
}


int TactilePuck::FullTactParser::parse(int id, int propId, result_type* result, const unsigned char* data, size_t len)
{
	if (len != 8) {
		logMessage("%s: expected message length of 8, got message length of %d") % __func__ % len;
		return 1;
	}

	size_t i = data[0] >> 4;  // sequence number
	if (i > NUM_FULL_MESSAGES - 1) {
		logMessage("%s: invalid sequence number: %d") % __func__ % i;
		return 1;
	}
	i *= NUM_SENSORS_PER_FULL_MESSAGE;  // first cell index

	(*result)[i++] = ( (((int)data[0]&0x000F)<<8) | ((int)data[1]&0x00FF) ) / FULL_SCALE_FACTOR;
	(*result)[i++] = ( (((int)data[2]&0x00FF)<<4) | (((int)data[3]&0x00F0)>>4) ) / FULL_SCALE_FACTOR;
	(*result)[i++] = ( (((int)data[3]&0x000F)<<8) | ((int)data[4]&0x00FF) ) / FULL_SCALE_FACTOR;
	(*result)[i++] = ( (((int)data[5]&0x00FF)<<4) | (((int)data[6]&0x00F0)>>4) ) / FULL_SCALE_FACTOR;
    if (i < NUM_SENSORS) {
    	(*result)[i] = ( (((int)data[6]&0x000F)<<8) | ((int)data[7]&0x00FF) ) / FULL_SCALE_FACTOR;
    }

    return 0;
}

int TactilePuck::Top10TactParser::parse(int id, int propId, result_type* result, const unsigned char* data, size_t len)
{
	if (len != 8) {
		logMessage("%s: expected message length of 8, got message length of %d") % __func__ % len;
		return 1;
	}

	// Top10 Example: 
	// 24.....17  16......9   8......1   AAAABBBB   CCCCDDDD   EEEEFFFF   GGGGHHHH   JJJJKKKK 
	// [10011000] [00111010] [10000011] [01100100] [01011110] [01110111] [10110110] [10010011] 
	// Sensors 1, 2, 8, 10, 12, 13, 14, 20, 21, and 24 are reporting the highest pressures. 
	// The pressures are, respectively: 6, 4, 5, 14, 7, 7, 11, 6, 9, 3 (N/cm2)
	
	// I don't know how "data" is aligned, so align the data properly here...
	uint64_t dat = 0; // No initialization required, but compiler complains otherwise
	for(size_t i = 0; i < sizeof(uint64_t); i++){
		dat <<= 8;
		dat |= data[i];
	}
	
	uint32_t map = dat >> 40; // Pick off the top 3 bytes
	uint8_t *byte = (uint8_t*)&dat + 4; // Pointer to the AAAABBBB byte of dat
	
	// Now rotate through the map and shift out each non-zero value from the packed data
	// Note: result is a math::Vector[24]
	for(size_t i = 0; i < NUM_SENSORS; i++){
		if(map & 1){
			(*result)[i] = (*byte & 0xF0) >> 4; // 4-bit N/cm2 (0-15)
			dat <<= 4;
		}else{
			(*result)[i] = 0; // No data returned for this cell, set to zero
		}
		map >>= 1;
	}

    return 0;
}


}
