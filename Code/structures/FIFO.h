/**
 * FIFO.h: this file defines a FIFO queue of Notification objects
 */

#ifndef _AUX_FIFO_
#define _AUX_FIFO_

#include "notification.h"
#include <deque>

struct FIFO
{
	std::deque<Notification> m_queue;
	int selected_destination;
	int selected_ac, selected_length;
	bool count_all_for_contention;
	FIFO() : selected_destination(-1), selected_ac(-1), selected_length(-1), count_all_for_contention(false) {}
	bool Matches(const Notification &p) {
		return (selected_destination<0 || p.destination_id==selected_destination)
			&& (selected_ac<0 || p.paper_ac==selected_ac)
			&& (selected_length<0 || p.frame_length==selected_length);
	}
	int WorkSize() { return count_all_for_contention ? TotalQueueSize() : QueueSize(); }
	int AcSize(int ac) {
		int count=0;
		for (unsigned int i=0;i<m_queue.size();++i) if(m_queue[i].paper_ac==ac) ++count;
		return count;
	}
	int TotalQueueSize() { return m_queue.size(); }
	int DestinationSize(int destination) {
		int count=0;
		for (unsigned int i=0; i<m_queue.size(); ++i)
			if (m_queue[i].destination_id == destination) ++count;
		return count;
	}
	int ActiveIndex(int n) {
		for (unsigned int i=0; i<m_queue.size(); ++i) {
			if (Matches(m_queue[i])) {
				if (n-- == 0) return i;
			}
		}
		throw std::out_of_range("Selected STA queue is empty");
	}

	Notification GetFirstPacket();
	Notification GetPacketAt(int n);
	void DelFirstPacket();
	void DeletePacketIn(int i);
	void PutPacket(Notification &packet);
	void PutPacketFront(Notification &packet);
	void PutPacketIn(Notification &packet, int i);
	int QueueSize();
};

Notification FIFO::GetFirstPacket()
{
	return(m_queue.at(ActiveIndex(0)));
}

Notification FIFO::GetPacketAt(int n)
{
	return(m_queue.at(ActiveIndex(n)));
}

void FIFO::DelFirstPacket()
{
	m_queue.erase(m_queue.begin() + ActiveIndex(0));
}

void FIFO::PutPacket(Notification &packet)
{
	m_queue.push_back(packet);
}

void FIFO::PutPacketFront(Notification &packet)
{
	m_queue.push_front(packet);
}

int FIFO::QueueSize()
{
	int count=0;
	for (unsigned int i=0;i<m_queue.size();++i) if (Matches(m_queue[i])) ++count;
	return count;
}

void FIFO::PutPacketIn(Notification &packet, int i)
{
	m_queue.insert(m_queue.begin() + i, packet);
}

void FIFO::DeletePacketIn(int i)
{
	m_queue.erase(m_queue.begin() + i);
}

#endif
