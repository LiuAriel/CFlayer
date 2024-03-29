/******************************************************************************
	CCAV:  Media play library based on Qt(UI),c/c++11 and FFmpeg
	Copyright (C) 2015-2016 Juno <junowendy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef CCAV_PACKET_H
#define CCAV_PACKET_H

#include <queue>
#include <CCAV/queue_blocking.h>
#include <CCAV/compat_common.h>
#include <stdint.h>
#include "master_array.h"

namespace CCAV {
class Q_EXPORT PacketData
{
public:
    PacketData();
	inline bool is_valid() const;
    double pts, duration;
	ByteArray date;
};

bool PacketData::is_valid() const
{
	return  pts >= 0 && duration >= 0/* || !date.isEmpty()*/; //!data.isEmpty()?
}

using packet_queue = QueueBlocking<PacketData>;

} //namespace CCAV

#endif // CCAV_PACKET_H
