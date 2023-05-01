#pragma once

struct s_memory_mapped_file_handle
{
#ifdef _WIN32
	void* file;
	void* file_mapping;
	int64_t len;
#else
	int file;
	long len;
#endif

	void* base_address;
};

//s_memory_mapped_file_handle open_memmapped_file(const char* path);
//void close_memmapped_file(s_memory_mapped_file_handle& handle);

struct s_memory_mapped_file
{
	s_memory_mapped_file_handle handle;

	bool valid();
	void* get_data();
	s_binary_view get_binary();

	s_memory_mapped_file(const char* path);
	~s_memory_mapped_file();
};
