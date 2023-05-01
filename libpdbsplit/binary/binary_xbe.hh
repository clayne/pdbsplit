#pragma once

class c_binary_format_xbe :
	public c_binary_executable_format
{
public:
	explicit c_binary_format_xbe(const s_binary_view& data);
	~c_binary_format_xbe() override;
};
