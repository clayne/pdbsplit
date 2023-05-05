#pragma once

struct s_pdb_object_library_path_pair
{
	string_t object_file_name;
	string_t library_file_name;
};

class c_binary_format_pdb :
	public c_binary_debug_format
{
protected:
	vec_t<s_pdb_object_library_path_pair> m_file_names;
public:
	GETTER(file_names);

	explicit c_binary_format_pdb(const s_binary_view& data);
	~c_binary_format_pdb() override;
};
