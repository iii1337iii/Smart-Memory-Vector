//  ____  __  ____  __ 
// / ___)(  \/  \ \/ /  Smart Memory Vector
// \___ \ )    ( \  /   version 1.0.0
// (____/(_/\/\_) \/    https://github.com/iii1337iii/Smart-Memory-Vector


// ### C++17 REQUIRED ###



#pragma once
#include <memory>
#include <initializer_list>
#include <new>
#include <cmath>
#include <stdexcept>


// #include "smvector.hpp"


namespace smv
{
	template<typename T>
	class smvector
	{
	private:
		// Pointers
		T* _Pstart;
		T* _Pfirst;
		T* _Plast;
		T* _Pend;
		// Memory control
		std::allocator<T> _alloc;
		float _growth_factor = 1.5f;

	public:
		//====================CONSTRUCTORS====================
		// smvector<T> smv;
		smvector() : _Pstart(nullptr), _Pfirst(nullptr), _Plast(nullptr), _Pend(nullptr) {}

		// smvector<T> smv(size);
		explicit smvector(size_t cap) :
			_Pstart(_alloc.allocate(cap)),
			_Pfirst(_Pstart),
			_Plast(_Pfirst + cap),
			_Pend(_Pstart + cap)
		{
			std::uninitialized_value_construct(_Pfirst, _Plast);
		}

		// smvector<T> smv(size, value);
		smvector(size_t cap, const T& value) :
			_Pstart(_alloc.allocate(cap)),
			_Pfirst(_Pstart),
			_Plast(_Pstart + cap),
			_Pend(_Pstart + cap)
		{
			std::uninitialized_fill(_Pfirst, _Plast, value);
		}

		// smvector<T> smv = {x1, x2, ...};   or   smvector<T> smv{x1, x2, ...};
		smvector(std::initializer_list<T> list) :
			_Pstart(_alloc.allocate(list.size())),
			_Pfirst(_Pstart),
			_Plast(_Pstart + list.size()),
			_Pend(_Pstart + list.size())
		{
			std::uninitialized_copy(list.begin(), list.end(), _Pfirst);
		}

		// smvector<T> smv2 = smv1;   or   smvector<T> smv2(smv1);
		smvector(const smvector& vec_origin) :
			_alloc(vec_origin._alloc), // Use the same memory allocation mechanism as in the original smv
			_Pstart(_alloc.allocate(vec_origin.capacity())),
			_Pfirst(_Pstart + (vec_origin._Pfirst - vec_origin._Pstart)),
			_Plast(_Pfirst + vec_origin.size()),
			_Pend(_Pstart + vec_origin.capacity())
		{
			std::uninitialized_copy(vec_origin._Pfirst, vec_origin._Plast, _Pfirst);
		}

		// smvector<T> smv2(std::move(smv1));
		smvector(smvector&& vec_origin) noexcept :
			_alloc(std::move(vec_origin._alloc)), // Use the same memory allocation mechanism as in the original smv
			_Pstart(vec_origin._Pstart),
			_Pfirst(vec_origin._Pfirst),
			_Plast(vec_origin._Plast),
			_Pend(vec_origin._Pend)
		{
			vec_origin._Pstart = vec_origin._Pfirst = vec_origin._Plast = vec_origin._Pend = nullptr;
		}

		//====================DESTRUCTOR====================
		~smvector() noexcept
		{
			if (_Pstart)
			{
				std::destroy(_Pfirst, _Plast); // Call object destructor
				_alloc.deallocate(_Pstart, capacity()); // Set memory free
				_Pstart = _Pfirst = _Plast = _Pend = nullptr;
			}
		}



		//====================OPERATORS====================
		//---copy assignment operator---
		smvector& operator=(smvector other) noexcept
		{
			this->swap(other);
			return *this;
		}

		//---index operator []---
		// read
		const T& operator[](size_t index) const { return *(_Pfirst + index); }
		// write
		T& operator[](size_t index) { return *(_Pfirst + index); }



		//====================STANDART METHODS====================
		//---size and capacity values---
		size_t size() const noexcept { return (_Plast - _Pfirst); }
		size_t capacity() const noexcept { return (_Pend - _Pstart); }



		//---first value and last value---
		T* begin() noexcept { return _Pfirst; }
		T* end() noexcept { return _Plast; }
		const T* begin() const noexcept { return _Pfirst; }
		const T* end() const noexcept { return _Plast; }
		const T* cbegin() const noexcept { return _Pfirst; }
		const T* cend() const noexcept { return _Plast; }



		//---swap---
		void swap(smvector& other) noexcept
		{
			std::swap(this->_Pstart, other._Pstart);
			std::swap(this->_Pfirst, other._Pfirst);
			std::swap(this->_Plast, other._Plast);
			std::swap(this->_Pend, other._Pend);
			std::swap(this->_alloc, other._alloc);
		}



		//---reserve---
		// back
		void reserve_back(size_t cap)
		{
			if (cap > capacity())
			{
				T* PstartNew = _alloc.allocate(cap);
				size_t current_size = size();
				size_t current_cap = capacity();
				size_t distance_SF = _Pfirst - _Pstart;

				if (_Pstart)
				{
					std::uninitialized_move(_Pfirst, _Plast, PstartNew + distance_SF); // Move or copy old data

					std::destroy(_Pfirst, _Plast); // Call object destructor
					_alloc.deallocate(_Pstart, current_cap); // Set memory free
				}
				_Pstart = PstartNew;
				_Pfirst = _Pstart + distance_SF;
				_Plast = _Pfirst + current_size;
				_Pend = _Pstart + cap;
			}
		}
		// front
		void reserve_front(size_t cap)
		{
			if (cap > capacity())
			{
				T* PstartNew = _alloc.allocate(cap);
				T* PfirstNew = PstartNew + cap - size() - (_Pend - _Plast);
				size_t current_size = size();
				size_t current_cap = capacity();

				if (_Pstart)
				{
					std::uninitialized_move(_Pfirst, _Plast, PfirstNew); // Move or copy old data

					std::destroy(_Pfirst, _Plast); // Call object destructor
					_alloc.deallocate(_Pstart, current_cap); // Set memory free
				}
				_Pstart = PstartNew;
				_Pfirst = PfirstNew;
				_Plast = _Pfirst + current_size;
				_Pend = _Pstart + cap;
			}
		}

		// r-value
		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void push_back(U&& value)
		{
			if (_Plast == _Pend)
			{
				reserve_back(capacity() == 0 ? 1 : static_cast<size_t>(std::ceil(capacity() * _growth_factor)));
			}
			// Memory has been allocated but nothing has been initialized within it
			new (_Plast) T(std::forward<U>(value)); // Call object constructor
			++_Plast;
		}
		// list
		void push_back(std::initializer_list<T> list)
		{
			if (list.size() > _Pend - _Plast)
			{
				size_t new_cap = std::max(capacity() + list.size(),
					static_cast<size_t>(std::ceil(capacity() * _growth_factor)));

				reserve_back(new_cap == 0 ? list.size() : new_cap);
			}
			// Memory has been allocated but nothing has been initialized within it
			_Plast = std::uninitialized_copy(list.begin(), list.end(), _Plast);
		}



		//---pop_back---
		void pop_back()
		{
			if (_Pfirst != _Plast)
			{
				--_Plast;
				std::destroy_at(_Plast);
			}
		}
		//---pop_front---
		void pop_front()
		{
			if (_Pfirst != _Plast)
			{
				std::destroy_at(_Pfirst);
				++_Pfirst;
			}
		}



		//---emplace_back---
		template<typename... Args>
		// Args&&... passes the value in its original state(as an l-value or r-value)
		void emplace_back(Args&&... args)
		{
			if (_Plast == _Pend)
			{
				reserve_back(capacity() == 0 ? 1 : static_cast<size_t>(std::ceil(capacity() * _growth_factor)));
			}
			// Memory has been allocated but nothing has been initialized within it
			// Perfect Forwarding ->
			// Storing the variables status (copy or move method)
			new (_Plast) T(std::forward<Args>(args)...); // Call object constructor 
			++_Plast;
		}



		//---erase---
		// single index
		void erase(size_t index)
		{
			T* Ptarget = _Pfirst + index;

			// right side
			if (index < size() / 2)
			{
				if (Ptarget > _Pfirst)
				{
					std::move_backward(_Pfirst, Ptarget, Ptarget + 1);
				}
				std::destroy_at(_Pfirst);
				++_Pfirst;
			}
			// left side
			else
			{
				if (Ptarget + 1 < _Plast)
				{
					std::move(Ptarget + 1, _Plast, Ptarget);
				}
				--_Plast;
				std::destroy_at(_Plast);
			}
		}
		// range
		void erase(size_t start, size_t end)
		{
			if (start >= end) { return; }

			size_t num_to_erase = end - start;
			size_t tail_size = size() - end;
			size_t head_size = start;

			T* Pstart = _Pfirst + start;
			T* Pend = _Pfirst + end;

			// right side
			if (head_size < tail_size)
			{
				std::move_backward(_Pfirst, Pstart, Pend);
				std::destroy(_Pfirst, _Pfirst + num_to_erase);
				_Pfirst += num_to_erase;
			}
			// left side
			else
			{
				T* Plast_new = std::move(Pend, _Plast, Pstart);
				std::destroy(Plast_new, _Plast);
				_Plast = Plast_new;
			}
		}


		// r-value
		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void insert(size_t index, U&& value)
		{
			if (_Pfirst == _Pstart && _Plast == _Pend)
			{
				reserve_back(capacity() == 0 ? 1 : static_cast<size_t>(std::ceil(capacity() * _growth_factor)));
			}
			// right side
			if (((_Plast < _Pend) && (index > size() / 2)) || _Pfirst == _Pstart)
			{
				T* Ptarget = _Pfirst + index;

				if (Ptarget < _Plast)
				{
					new (_Plast) T(std::move(*(_Plast - 1)));
					
					if (_Plast - 1 > Ptarget) 
					{
						std::move_backward(Ptarget, _Plast - 1, _Plast);
					}

					*Ptarget = std::forward<U>(value);
				}
				else
				{
					new (Ptarget) T(std::forward<U>(value));
				}

				++_Plast;
			}
			// left side
			else
			{
				--_Pfirst;
				T* Ptarget = _Pfirst + index;

				if (Ptarget > _Pfirst)
				{
					new (_Pfirst) T(std::move(*(_Pfirst + 1)));

					if (Ptarget > _Pfirst + 1) 
					{
						std::move(_Pfirst + 2, Ptarget + 1, _Pfirst + 1);
					}

					*Ptarget = std::forward<U>(value);
				}
				else
				{
					new (Ptarget) T(std::forward<U>(value));
				}
			}
		}

		// r-value

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void insert(size_t index, U&& value, size_t cap)
		{
			if (_Pfirst == _Pstart && _Plast == _Pend)
			{
				reserve_back(capacity() == 0 ? 1 : capacity() + cap);
			}
			// right side
			if (((_Plast < _Pend) && (index > size() / 2)) || _Pfirst == _Pstart)
			{
				T* Ptarget = _Pfirst + index;

				if (Ptarget < _Plast)
				{
					new (_Plast) T(std::move(*(_Plast - 1)));
					
					if (_Plast - 1 > Ptarget) 
					{
						std::move_backward(Ptarget, _Plast - 1, _Plast);
					}

					*Ptarget = std::forward<U>(value);
				}
				else
				{
					new (Ptarget) T(std::forward<U>(value));
				}

				++_Plast;
			}
			// left side
			else
			{
				--_Pfirst;
				T* Ptarget = _Pfirst + index;

				if (Ptarget > _Pfirst)
				{
					new (_Pfirst) T(std::move(*(_Pfirst + 1)));

					if (Ptarget > _Pfirst + 1) 
					{
						std::move(_Pfirst + 2, Ptarget + 1, _Pfirst + 1);
					}

					*Ptarget = std::forward<U>(value);
				}
				else
				{
					new (Ptarget) T(std::forward<U>(value));
				}
			}
		}



		//---clear---
		void clear() noexcept
		{
			if (_Pfirst)
			{
				std::destroy(_Pfirst, _Plast); // Call object destructor
				_Plast = _Pfirst = _Pstart + std::ceil(capacity() / 2);
			}
		}



		//---back and front values---
		T& back() noexcept { return *(_Plast - 1); }
		const T& back() const noexcept { return *(_Plast - 1); }
		T& front() noexcept { return *(_Pfirst); }
		const T& front() const noexcept { return *(_Pfirst); }



		//---empty---
		bool empty() noexcept { return _Pfirst == _Plast; }



		//====================UNIQUE METHODS====================
		//---growth factor control---
		float get_growth_factor() const { return _growth_factor; }
		void set_growth_factor(float fac)
		{
			if (fac >= 1.1f && fac <= 10.0f)
			{
				_growth_factor = fac;
			}
		}


		// r-value + memory control
		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void push_back(U&& value, size_t cap)
		{
			if (cap == 0) { cap = 1; }

			if (_Plast == _Pend)
			{
				reserve_back(capacity() == 0 ? 1 : capacity() + cap);
			}
			// Memory has been allocated but nothing has been initialized within it
			new (_Plast) T(std::forward<U>(value)); // Call object constructor
			++_Plast;
		}

		// list + memory control
		void push_back(std::initializer_list<T> list, size_t cap)
		{
			if (list.size() > _Pend - _Plast)
			{
				size_t new_cap = std::max(capacity() + list.size(), capacity() + cap);

				reserve_back(new_cap == 0 ? list.size() : new_cap);
			}
			// Memory has been allocated but nothing has been initialized within it
			_Plast = std::uninitialized_copy(list.begin(), list.end(), _Plast);
		}


		// r-value
		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void push_front(U&& value)
		{
			if (_Pfirst == _Pstart)
			{
				reserve_front(_Pfirst == nullptr ? 1 : static_cast<size_t>(std::ceil(capacity() * _growth_factor)));
			}

			--_Pfirst;
			// Memory has been allocated but nothing has been initialized within it
			new (_Pfirst) T(std::forward<U>(value)); // Call object constructor
		}


		// r-value + memory control
		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void push_front(U&& value, size_t cap)
		{
			if (cap == 0) { cap = 1; }

			if (_Pfirst == _Pstart)
			{
				reserve_front(_Pfirst == nullptr ? 1 : capacity() + cap);
			}

			--_Pfirst;
			// Memory has been allocated but nothing has been initialized within it
			new (_Pfirst) T(std::forward<U>(value)); // Call object constructor
		}

		// list
		void push_front(std::initializer_list<T> list)
		{
			size_t count = list.size();
			if (count == 0) { return; }

			if (count > _Pfirst - _Pstart)
			{
				reserve_front(std::max(size() + count, static_cast<size_t>(std::ceil(capacity() * _growth_factor))));
			}
			// Memory has been allocated but nothing has been initialized within it
			_Pfirst -= count;
			std::uninitialized_copy(list.begin(), list.end(), _Pfirst);
		}

		// list + memory control
		void push_front(std::initializer_list<T> list, size_t cap)
		{
			size_t count = list.size();
			if (count == 0) { return; }

			if (count > _Pfirst - _Pstart)
			{
				reserve_front(std::max(size() + count, capacity() + cap));
			}
			// Memory has been allocated but nothing has been initialized within it
			_Pfirst -= count;
			std::uninitialized_copy(list.begin(), list.end(), _Pfirst);
		}



		//---emplace_front---
		template<typename... Args>
		// Args&&... passes the value in its original state(as an l-value or r-value)
		void emplace_front(Args&&... args)
		{
			if (_Pfirst == _Pstart)
			{
				reserve_front(capacity() == 0 ? 1 : static_cast<size_t>(std::ceil(capacity() * _growth_factor)));
			}
			--_Pfirst;
			// Perfect Forwarding ->
			// Storing the variables status (copy or move method)
			new (_Pfirst) T(std::forward<Args>(args)...);
		}


	};
}

//  ____  __  ____  __ 
// / ___)(  \/  \ \/ /  Smart Memory Vector
// \___ \ )    ( \  /   version 1.0.0
// (____/(_/\/\_) \/    https://github.com/iii1337iii/Smart-Memory-Vector
