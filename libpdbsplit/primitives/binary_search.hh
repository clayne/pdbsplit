#pragma once

template<typename T, typename userdata>
using binary_search_function_t = uintmax_t(T*, uintmax_t, userdata*);

template<typename T, typename userdata = void>
static inline void binary_search(
	T* data,
	uintmax_t count,
	binary_search_function_t<T, userdata> get_key_function,
	uintmax_t search_key,
	T* result_out,
	userdata* user_data)
{
	uintmax_t search_index = 0;
	uintmax_t search_end = count - 1;

	while (search_index <= search_end)
	{
		uintmax_t search_middle = (search_index + search_end) / 2;
		uintmax_t key = get_key_function(data, search_middle, user_data);
		if (key < search_key)
		{
			assert(search_middle != UINT32_MAX);
			search_index = search_middle + 1;
		}
		else if (key > search_key)
		{
			assert(search_middle != 0);
			search_end = search_middle - 1;
		}
		else /*(key == search_key)*/
		{
			result_out = data + search_middle;
			break;
		}
	}
}
