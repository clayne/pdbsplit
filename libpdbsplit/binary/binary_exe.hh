#pragma once

class c_binary_format_exe :
	public c_binary_executable_format
{
public:
	explicit c_binary_format_exe(const s_binary_view& data);
	~c_binary_format_exe() override;
};
