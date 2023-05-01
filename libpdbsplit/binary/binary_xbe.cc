namespace xbe
{
	#pragma pack(1)
	struct header
	{
		uint32_t m_magic;                         // magic number [should be "XBEH"]
		uint8_t m_digsig[256];                   // digital signature
		uint32_t m_base;                          // base address
		uint32_t m_sizeof_headers;                // size of headers
		uint32_t m_sizeof_image;                  // size of image
		uint32_t m_sizeof_image_header;           // size of image header
		uint32_t m_timedate;                      // timedate stamp
		uint32_t m_certificate_addr;              // certificate address
		uint32_t m_sections;                      // number of sections
		uint32_t m_section_headers_addr;          // section headers address

		uint32_t m_init_flags;

		uint32_t m_entry;                         // entry point address
		uint32_t m_tls_addr;                      // thread local storage directory address
		uint32_t m_pe_stack_commit;               // size of stack commit
		uint32_t m_pe_heap_reserve;               // size of heap reserve
		uint32_t m_pe_heap_commit;                // size of heap commit
		uint32_t m_pe_base_addr;                  // original base address
		uint32_t m_pe_sizeof_image;               // size of original image
		uint32_t m_pe_checksum;                   // original checksum
		uint32_t m_pe_timedate;                   // original timedate stamp
		uint32_t m_debug_pathname_addr;           // debug pathname address
		uint32_t m_debug_filename_addr;           // debug filename address
		uint32_t m_debug_unicode_filename_addr;   // debug unicode filename address
		uint32_t m_kernel_image_thunk_addr;       // kernel image thunk address
		uint32_t m_nonkernel_import_dir_addr;     // non kernel import directory address
		uint32_t m_library_versions;              // number of library versions
		uint32_t m_library_versions_addr;         // library versions address
		uint32_t m_kernel_library_version_addr;   // kernel library version address
		uint32_t m_xapi_library_version_addr;     // xapi library version address
		uint32_t m_logo_bitmap_addr;              // logo bitmap address
		uint32_t m_logo_bitmap_size;              // logo bitmap size
	};

	struct certificate
	{
		uint32_t m_size;                          // size of certificate
		uint32_t m_timedate;                      // timedate stamp
		uint32_t m_titleid;                       // title id
		uint16_t m_title_name[40];                // title name (unicode)
		uint32_t m_alt_title_id[0x10];            // alternate title ids
		uint32_t m_allowed_media;                 // allowed media types
		uint32_t m_game_region;                   // game region
		uint32_t m_game_ratings;                  // game ratings
		uint32_t m_disk_number;                   // disk number
		uint32_t m_version;                       // version
		uint8_t m_lan_key[16];                   // lan key
		uint8_t m_sig_key[16];                   // signature key
		uint8_t m_title_alt_sig_key[16][16];     // alternate signature keys
	};

	struct section_header
	{
		uint32_t m_flags;
		uint32_t m_virtual_addr;                  // virtual address
		uint32_t m_virtual_size;                  // virtual size
		uint32_t m_raw_addr;                      // file offset to raw data
		uint32_t m_sizeof_raw;                    // size of raw data
		uint32_t m_section_name_addr;             // section name addr
		uint32_t m_section_reference_count;       // section reference count
		uint32_t m_head_shared_ref_count_addr;    // head shared page reference count address
		uint32_t m_tail_shared_ref_count_addr;    // tail shared page reference count address
		uint8_t  m_section_digest[20];            // section digest
	};
}

c_binary_format_xbe::c_binary_format_xbe(const s_binary_view& data)
{
	const xbe::header* xbe_header = data.get<xbe::header>();
	m_load_address = xbe_header->m_base;

	s_array_view<xbe::section_header> section_headers(
		data.get<xbe::section_header>(xbe_header->m_section_headers_addr - m_load_address),
		xbe_header->m_sections);

	for (auto& section_header : section_headers)
	{
		s_binary_linked_section section;

		section.name.set(data.get<char>(section_header.m_section_name_addr - m_load_address));
		section.length = section_header.m_sizeof_raw;
		section.binary_offset = section_header.m_raw_addr;
		section.load_address = section_header.m_virtual_addr;

		section.data.m_data = &data[section.binary_offset];
		section.data.m_length = section.length;

		m_sections.push_back(section);
	}
}

c_binary_format_xbe::~c_binary_format_xbe()
{

}
