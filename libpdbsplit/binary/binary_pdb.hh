#pragma once

class c_binary_format_pdb :
	public c_binary_debug_format
{
public:
	explicit c_binary_format_pdb(const s_binary_view& data);
	~c_binary_format_pdb() override;
};
