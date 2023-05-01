#pragma once

template <typename T>
struct s_array_view
{
	const T* m_data;
	size_t m_length;

	inline constexpr const T* data(void) const noexcept
	{
		return m_data;
	}

	inline constexpr size_t length(void) const noexcept
	{
		return m_length;
	}

	inline const T& operator[](size_t i) const noexcept
	{
		assert(i < length());
		return m_data[i];
	}

	template<typename T>
	inline constexpr const T* get(size_t offset = 0) const noexcept
	{
		assert(offset < length());
		return (const T*)&m_data[offset];
	}

	inline const T* begin(void) const noexcept
	{
		return m_data;
	}

	inline const T* end(void) const noexcept
	{
		return m_data + m_length;
	}

	inline constexpr explicit s_array_view(
		const T* array,
		size_t length) :
		m_data(array),
		m_length(length)
	{

	}

	inline constexpr explicit s_array_view() :
		s_array_view(nullptr, 0)
	{

	}
};

using s_binary_view = s_array_view<uint8_t>;
