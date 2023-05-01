#pragma once

#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))

template<size_t k_maximum_count, size_t k_storage_count = ((k_maximum_count - 1) >> 5) + 1>
struct s_static_flags
{
	bool test(uint32_t index)
	{
		assert(VALID_INDEX(index, k_maximum_count));

		return (m_storage[index >> 5] & 1 << (index & 31)) != 0;
	}

	bool test(uint32_t index) const
	{
		assert(VALID_INDEX(index, k_maximum_count));

		return (m_storage[index >> 5] & 1 << (index & 31)) != 0;
	}

	void clear()
	{
		memset(m_storage, 0, sizeof(m_storage));
	}

	void set(uint32_t index, bool enable)
	{
		if (enable)
			m_storage[index >> 5] |= (1 << (index & 31));
		else
			m_storage[index >> 5] &= ~(1 << (index & 31));
	}

	uint32_t m_storage[k_storage_count];
};
