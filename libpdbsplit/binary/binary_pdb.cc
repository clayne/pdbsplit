#include <PDB.h>
#include <PDB_RawFile.h>
#include <PDB_DBIStream.h>

static inline uint32_t binary_search_get_public_symbol_rva(
	const PDB::CodeView::DBI::Record* data,
	uint32_t search_middle,
	const PDB::ImageSectionStream* userdata)
{
	return data[search_middle].data.S_PUB32.offset;
}

// naive as all hell
void fill_names_for_contribution(
	const vec_t<const PDB::CodeView::DBI::Record*>& public_names,
	const PDB::DBI::SectionContribution* section_contribution,
	const PDB::ImageSectionStream* image_section_stream,
	vec_t<const PDB::CodeView::DBI::Record*>& public_names_out)
{
	const PDB::CodeView::DBI::Record* head = nullptr;

	uint32_t rva = image_section_stream->ConvertSectionOffsetToRVA(
		section_contribution->section,
		section_contribution->offset);

	uint32_t end = rva + section_contribution->size;
	
	for (const auto public_name : public_names)
	{
		uint32_t public_name_rva = image_section_stream->ConvertSectionOffsetToRVA(
			public_name->data.S_PUB32.section,
			public_name->data.S_PUB32.offset);

		if (public_name_rva >= rva && public_name_rva < end)
		{
			public_names_out.push_back(public_name);
		}
	}
}

c_binary_format_pdb::c_binary_format_pdb(
	const s_binary_view& data)
{
	assert(PDB::ValidateFile(data.data()) == PDB::ErrorCode::Success);
	PDB::RawFile raw_pdb = PDB::CreateRawFile(data.data());
	PDB::DBIStream dbi_stream = PDB::CreateDBIStream(raw_pdb);
	PDB::ImageSectionStream image_section_stream = dbi_stream.CreateImageSectionStream(raw_pdb);
	PDB::SectionContributionStream section_contribution_stream = dbi_stream.CreateSectionContributionStream(raw_pdb);
	PDB::CoalescedMSFStream symbol_record_stream = dbi_stream.CreateSymbolRecordStream(raw_pdb);
	PDB::PublicSymbolStream public_symbol_stream = dbi_stream.CreatePublicSymbolStream(raw_pdb);
	PDB::ModuleInfoStream module_info_stream = dbi_stream.CreateModuleInfoStream(raw_pdb);

	for (const auto& pdb_section : image_section_stream.GetImageSections())
	{
		s_binary_debug_section section;
		section.name.print("%.8s", (const char*)pdb_section.Name);
		section.length = pdb_section.SizeOfRawData;
		section.binary_offset = pdb_section.PointerToRawData;
		section.load_address = pdb_section.VirtualAddress;

		m_sections.push_back(section);
	}

	for (const auto& module : module_info_stream.GetModules())
	{
		s_pdb_object_library_path_pair pair;
		pair.object_file_name.set(module.GetName().Decay());
		pair.library_file_name.set(module.GetObjectName().Decay());

		m_file_names.push_back(pair);
	}

	vec_t<const PDB::CodeView::DBI::Record*> public_names;
	for (const auto& hash_record : public_symbol_stream.GetRecords())
	{
		const PDB::CodeView::DBI::Record* record = public_symbol_stream.GetRecord(
			symbol_record_stream, hash_record);

		// end of data? THIS SHOULD NOT BE HAPPENING
		if (!record)
		{
			break;
		}

		uint32_t rva = image_section_stream.ConvertSectionOffsetToRVA(
			record->data.S_PUB32.section, record->data.S_PUB32.offset);

		// certain symbols (e.g. control-flow guard symbols) don't have a valid RVA, ignore those
		if (rva != 0)
		{
			public_names.push_back(record);
		}
	}

	for (const auto& contribution : section_contribution_stream.GetContributions())
	{
		s_binary_debug_symbol new_symbol;

		va_t rva = image_section_stream.ConvertSectionOffsetToRVA(
			contribution.section, contribution.offset);

		new_symbol.address = rva;
		new_symbol.length = contribution.size;
		new_symbol.object_id = contribution.moduleIndex;

		vec_t<const PDB::CodeView::DBI::Record*> associated_public_names;
		fill_names_for_contribution(public_names, &contribution, &image_section_stream, associated_public_names);

		for (auto& public_name : associated_public_names)
		{
			va_t name_rva = image_section_stream.ConvertSectionOffsetToRVA(
				public_name->data.S_PUB32.section, public_name->data.S_PUB32.offset);

			s_name_offset_pair pair;
			pair.name.print("%.*s",
				public_name->data.S_PUB32.name.vc60.length,
				public_name->data.S_PUB32.name.vc60.string);
			pair.offset = name_rva - rva;
			new_symbol.names.push_back(pair);
		}

		m_symbols.push_back(new_symbol);
	}
}

c_binary_format_pdb::~c_binary_format_pdb()
{

}
