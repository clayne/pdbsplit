#pragma once

using offset_t = int64_t;
using va_t = uint64_t;

struct s_hashed_offset
{
	uint64_t hash;
	offset_t offset;
};

enum e_binary_section_flags
{
	_binary_section_flag_memory_r,
	_binary_section_flag_memory_w,
	_binary_section_flag_memory_x,

	k_number_of_binary_section_flags,
};

struct s_binary_section
{
	short_string_t name;
	uint64_t length;
	s_binary_view data;
	s_static_flags<k_number_of_binary_section_flags> flags;
};

struct s_binary_linked_section :
	public s_binary_section
{
	offset_t binary_offset;
	va_t load_address;
};

class c_binary_executable_format
{
protected:
	va_t m_load_address;
	vec_t<s_binary_linked_section> m_sections;
public:
	GETTER(load_address);
	GETTER(sections);

	explicit c_binary_executable_format(const s_binary_view& data);
	virtual ~c_binary_executable_format();
protected:
	c_binary_executable_format();
};

enum e_binary_debug_symbol_flags
{
	_binary_debug_symbol_flag_memory_r,
	_binary_debug_symbol_flag_memory_w,
	_binary_debug_symbol_flag_memory_x,

	k_number_of_binary_debug_symbol_flags,
};

struct s_binary_debug_symbol
{
	string_t name;
	uint64_t length;
	va_t address;
	s_static_flags<k_number_of_binary_debug_symbol_flags> flags;
};

class c_binary_debug_format
{
protected:
	vec_t<s_binary_debug_symbol> m_symbols;
public:
	GETTER(symbols);

	explicit c_binary_debug_format(const s_binary_view& data);
	virtual ~c_binary_debug_format();
protected:
	c_binary_debug_format();
};

struct s_binary_object_symbol
{
	string_t name;
	uint64_t length;
	s_hashed_offset section_offset;
	s_static_flags<k_number_of_binary_debug_symbol_flags> flags;
};

class c_binary_object_format
{
protected:
	vec_t<s_binary_section> m_sections;
	vec_t<s_binary_debug_symbol> m_symbols;
	vec_t<string_t> m_object_file_names;
public:
	GETTER(sections);
	GETTER(symbols);
	GETTER(object_file_names);

	explicit c_binary_object_format(const s_binary_view& data);
	virtual ~c_binary_object_format();
protected:
	c_binary_object_format();
};
