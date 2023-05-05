#include "libpdbsplit_public_pch.hh"

int main(int argc, char* argv[])
{
	s_memory_mapped_file exe_bin("cachebeta.exe");
	c_binary_format_exe exe(exe_bin.get_binary());

	s_memory_mapped_file xbe_bin("cachebeta.xbe");
	c_binary_format_xbe xbe(xbe_bin.get_binary());

	s_memory_mapped_file pdb_bin("cachebeta.pdb");
	c_binary_format_pdb pdb(pdb_bin.get_binary());

	c_analysis_provider_cachebeta_2342 provider(exe, xbe, pdb);
	c_analysis_chunk_reader chunk_reader(provider);

	auto& symbols = pdb.get_symbols();
	for (auto& symbol : symbols)
	{
		for (auto& pair : symbol.names)
		{
			printf("%s\n", pair.name.get_string());
		}
	}

	return 0;
}
