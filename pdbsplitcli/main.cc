#include "libpdbsplit_public_pch.hh"

int main(int argc, char* argv[])
{
	s_memory_mapped_file xbe_bin("cachebeta.xbe");
	c_binary_format_xbe xbe(xbe_bin.get_binary());

	s_memory_mapped_file pdb_bin("cachebeta.pdb");
	c_binary_format_pdb pdb(pdb_bin.get_binary());

	return 0;
}
