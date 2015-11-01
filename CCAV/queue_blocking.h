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


#ifndef CCAV_BLOCKINGQUEUE_H
#define CCAV_BLOCKINGQUEUE_H

#include <queue>//deque
#include <condition_variable>
#include <memory>
#include <mutex>
#include <atomic>

namespace CCAV {

	template <typename T>
	class QueueBlocking
	{
	public:
		QueueBlocking();

		void set_capacity(int max); 
		void set_threshold(int min); 

		void put(const T&& t);
		bool pop(T& data);
		void set_blocking(bool block);
		void block_empty(bool block);
		void block_full(bool block);

		inline void clear();
		inline bool is_empty() const;
		inline int size() const;
		inline int threshold() const;
		inline int capacity() const;

	private:
		std::atomic<bool> block_empty_, block_full_;
		unsigned int cap, thres; 
		
		std::queue<T> queue_;
		std::condition_variable full_;
		std::condition_variable empty_;
		std::mutex mutex_;
	};


	template <typename T>
	QueueBlocking<T>::QueueBlocking()
		:block_empty_(true), block_full_(true), cap(1024), thres(128)
	{
	}

	template <typename T>
	void QueueBlocking<T>::set_capacity(int max)
	{
		cap = max;
	}

	template <typename T>
	void QueueBlocking<T>::set_threshold(int min)
	{
		thres = min;
	}

	template <typename T>
	void QueueBlocking<T>::put(const T&& t)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		
		if (block_full_ && queue_.size() >= cap)
			full_.wait(lock);
		queue_.push(std::move(t));
		empty_.notify_all();
	}

	template <typename T>
	bool QueueBlocking<T>::pop(T& data)
	{
		
		std::unique_lock<std::mutex> lock(mutex_);
		if (queue_.size() < thres)
			full_.notify_all();

		if (block_empty_ && queue_.empty())
			empty_.wait(lock);

		if (queue_.empty()) {
			qWarning("Queue is still empty");
			return false;
		}
		data = std::move(queue_.front());
		queue_.pop();
		return true;
	}

	template <typename T>
	void QueueBlocking<T>::set_blocking(bool block)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		block_empty_ = block_full_ = block;
		if (!block) {
			empty_.notify_all();
			full_.notify_all();
		}
	}

	template <typename T>
	void QueueBlocking<T>::block_empty(bool block)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		block_empty_ = block;
		if (!block) {
			empty_.notify_all();
		}
	}

	template <typename T>
	void QueueBlocking<T>::block_full(bool block)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		block_full_ = block;
		if (!block) {
			full_.notify_all();
		}
	}

	template <typename T>
	void QueueBlocking<T>::clear()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		full_.notify_all();
		while (!queue_.empty())
		{
			queue_.pop();
		}
	}

	template <typename T>
	bool QueueBlocking<T>::is_empty() const
	{
		return queue_.empty();
	}

	template <typename T>
	int QueueBlocking<T>::size() const
	{
		return queue_.size();
	}

	template <typename T>
	int QueueBlocking<T>::threshold() const
	{
		return thres;
	}

	template <typename T>
	int QueueBlocking<T>::capacity() const
	{
		return cap;
	}

} //namespace CCAV
#endif // CCAV_BLOCKINGQUEUE_H
