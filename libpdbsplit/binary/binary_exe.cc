#define WIN32_LEAN_AND_MEAN
#include <windows.h>

c_binary_format_exe::c_binary_format_exe(const s_binary_view& data)
{
	const IMAGE_DOS_HEADER *dos_header = data.get<IMAGE_DOS_HEADER>();
	const IMAGE_NT_HEADERS32 *nt_headers = data.get<IMAGE_NT_HEADERS32>(dos_header->e_lfanew);

	m_load_address = nt_headers->OptionalHeader.ImageBase;

	size_t section_index;
	PIMAGE_SECTION_HEADER pe_section;
	for (pe_section = IMAGE_FIRST_SECTION(nt_headers), section_index = 0;
		section_index < nt_headers->FileHeader.NumberOfSections;
		pe_section++, section_index++)
	{
		s_binary_linked_section section;

		section.name.print("%.8s", (const char*)pe_section->Name);
		section.length = pe_section->SizeOfRawData;
		section.binary_offset = pe_section->PointerToRawData;
		section.load_address = pe_section->VirtualAddress;

		section.data.m_data = &data[section.binary_offset];
		section.data.m_length = section.length;

		m_sections.push_back(section);
	}
}

c_binary_format_exe::~c_binary_format_exe()
{

}
